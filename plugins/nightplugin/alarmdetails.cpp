#include "alarmdetails.h"
#include "ui_alarmdetails.h"

#include "../../controlpanel/alarmdaemon.h"

#include "timepage.h"

#include <QtCore/QDebug>

AlarmDetails::AlarmDetails(QWidget *parent, Msg::Alarm *alarm) :
    QWidget(parent),
    _ui(new Ui::AlarmDetails),
    _alarm(alarm)
{
    _ui->setupUi(this);

    if ( _alarm == NULL )
    {
        qDebug() << "NightPlugin/AlarmDetails: creating new alarm!";
        _alarm = new Msg::Alarm("");
        Msg::AlarmDaemon::getInstance().addAlarm(_alarm);
        _alarm->save();
    }

    updateAlarm();

    connect( _ui->timeButton, SIGNAL(clicked()), this, SLOT(editTime()));
    connect( _ui->snoozeButton, SIGNAL(clicked()), this, SLOT(editSnooze()));
    connect( _ui->ringtoneButton, SIGNAL(clicked()), this, SLOT(editSource()));
    connect( _ui->volumeButton, SIGNAL(clicked()), this, SLOT(editVolume()));
    connect( _ui->dayButton, SIGNAL(clicked()), this, SLOT(editDays()));

    connect( _ui->backButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect( _ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteAlarm()));
}

AlarmDetails::~AlarmDetails()
{
    delete _ui;
}

void AlarmDetails::deleteAlarm()
{
    qDebug() << "NightPlugin/AlarmDetails: deleting alarm" << _alarm->getName();
    Msg::AlarmDaemon::getInstance().removeAlarm(_alarm);
    _alarm->remove();
    deleteLater();
}

void AlarmDetails::updateAlarm()
{
    _alarm->remove();
    _ui->timeButton->setText(_alarm->getTime());
    QString rep = "";
    Msg::Weekdays days = _alarm->getDays();
    bool weekend = false, weekdays = false;
    if ( days.monday && days.tuesday && days.wednesday && days.thursday && days.friday )
        weekdays = true;
    if ( days.saturday && days.sunday )
        weekend = true;
    if ( weekdays && weekend )
        rep += "Everyday, ";
    else if ( weekdays )
        rep += "Weekdays, ";
    else if ( weekend )
        rep += "Weekend, ";
    else
    {
        if ( days.monday )
            rep += "Monday, ";
        if ( days.tuesday )
            rep += "Tuesday, ";
        if ( days.wednesday )
            rep += "Wednesday, ";
        if ( days.thursday )
            rep += "Thursday, ";
        if ( days.friday )
            rep += "Friday, ";
        if ( days.saturday )
            rep += "Saturday, ";
        if ( days.sunday )
            rep += "Sunday, ";
    }
    _alarm->setName(rep + _alarm->getTime());
    rep.chop(2);
    _ui->dayButton->setText(rep);
    _ui->ringtoneButton->setText(_alarm->getSource());
    _ui->snoozeButton->setText(QString::number(_alarm->getSnoozeTime()).rightJustified(2, '0'));
    _ui->volumeButton->setText(QString::number(_alarm->getVolume()));
    _ui->nameLabel->setText(_alarm->getName());

    _alarm->save();
}

void AlarmDetails::editTime()
{
    qDebug() << "NightPlugin/Alarmdetails: editTime";
    TimePage *page = new TimePage(_alarm);
    connect(page, SIGNAL(destroyed()), this, SLOT(updateAlarm()));
    page->show();
}

void AlarmDetails::editSnooze()
{
    qDebug() << "NightPlugin/Alarmdetails: editSnooze";
}

void AlarmDetails::editSource()
{
    qDebug() << "NightPlugin/Alarmdetails: editSource";
}

void AlarmDetails::editVolume()
{
    qDebug() << "NightPlugin/Alarmdetails: editVolume";
}

void AlarmDetails::editDays()
{
    qDebug() << "NightPlugin/Alarmdetails: editDays";
}
