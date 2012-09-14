#include "alarmdetails.h"
#include "ui_alarmdetails.h"

#include "alarmwizard.h"
#include "../../controlpanel/alarmdaemon.h"

AlarmDetails::AlarmDetails(QWidget *parent, Msg::Alarm *alarm) :
    QWidget(parent),
    _ui(new Ui::AlarmDetails),
    _alarm(alarm)
{
    _ui->setupUi(this);

    if ( alarm == NULL )
    {
            this->close();
            delete this;
    }

    _ui->detailBrowser->clear();
    _ui->detailBrowser->append(alarm->getName());
    _ui->detailBrowser->append("Time: " + alarm->getTime());
    QString rep = "";
    Msg::Weekdays days = alarm->getDays();
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
    rep.chop(2);
    _ui->detailBrowser->append("Repetitions: " + rep);
    _ui->detailBrowser->append("Ringtone: " + alarm->getSource() );
    _ui->detailBrowser->append("Snoozetime: " + QString::number(alarm->getSnoozeTime()) );

    connect( _ui->editButton, SIGNAL(clicked()), this, SLOT(editAlarm()));
    connect( _ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteAlarm()));
}

AlarmDetails::~AlarmDetails()
{
    delete _ui;
}

void AlarmDetails::editAlarm()
{
        AlarmWizard *wizard = new AlarmWizard(NULL, _alarm);
        connect( wizard, SIGNAL(accepted()), this, SLOT(deleteLater()) );
        connect( wizard, SIGNAL(rejected()), this, SLOT(deleteLater()) );
        wizard->showFullScreen();
}

void AlarmDetails::deleteAlarm()
{
        Msg::AlarmDaemon::getInstance().removeAlarm(_alarm);
        this->close();
        delete this;
}
