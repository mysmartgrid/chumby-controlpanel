#include "alarmform.h"
#include "ui_alarmform.h"

#include "alarmdetails.h"

#include <QtCore/QDebug>
#include <QtGui/QCheckBox>

AlarmForm::AlarmForm(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::AlarmForm)
{
    _ui->setupUi(this);

        refresh();

        //connect(_ui->alarmList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(editAlarm(QListWidgetItem*, QListWidgetItem*)));
        //connect(_ui->alarmList, SIGNAL(itemSelectionChanged()), this, SLOT(editAlarm()));
                connect(_ui->alarmList, SIGNAL(itemSelectionChanged()), this, SLOT(alarmDetails()));
                connect(_ui->alarmList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(alarmChanged(QListWidgetItem*)));
                connect(_ui->newButton, SIGNAL(clicked()), this, SLOT(editAlarm()));
        connect(_ui->doneButton, SIGNAL(clicked()), this, SLOT(close()));
}

AlarmForm::~AlarmForm()
{
    delete _ui;
}

/*void AlarmForm::editAlarm(QListWidgetItem *previous, QListWidgetItem *current)
{
    if ( previous )
        qDebug() << previous->text();
    if ( current )
        qDebug() << "->" << current->text();
}*/

void AlarmForm::refresh()
{
    _ui->alarmList->clear();
    //_ui->alarmList->addItem("New Alarm");
    std::list<Msg::Alarm*> alarms = Msg::AlarmDaemon::getInstance().getAlarms();
    for ( std::list<Msg::Alarm*>::iterator it = alarms.begin(); it != alarms.end(); it++ )
    {
        QListWidgetItem *item = new QListWidgetItem((*it)->getName());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        if ( (*it)->isActive() )
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        item->setSizeHint(QSize(32, 32));
        _ui->alarmList->addItem(item);
    }
}

void AlarmForm::alarmDetails()
{
    if ( _ui->alarmList->selectedItems().count() == 0 )
        return;

    Msg::Alarm *alarm = NULL;
    std::list<Msg::Alarm*> alarms = Msg::AlarmDaemon::getInstance().getAlarms();
    for ( std::list<Msg::Alarm*>::iterator it = alarms.begin(); it != alarms.end(); it++ )
    {
        if ( (*it)->getName().compare(_ui->alarmList->selectedItems().first()->text()) == 0 )
        {
            qDebug() << (*it)->getName() << "==" << _ui->alarmList->selectedItems().first()->text();
            alarm = *it;
        }
    }

    editAlarm(alarm);
}

void AlarmForm::editAlarm(Msg::Alarm *alarm)
{
    AlarmDetails *widget = new AlarmDetails(NULL, alarm);
    connect( widget, SIGNAL(destroyed()), this, SLOT(refresh()) );
    widget->showFullScreen();
}

void AlarmForm::alarmChanged(QListWidgetItem *item)
{
    qDebug() << "AlarmChanged:" << item->text();

    std::list<Msg::Alarm*> alarms = Msg::AlarmDaemon::getInstance().getAlarms();
    for ( std::list<Msg::Alarm*>::iterator it = alarms.begin(); it != alarms.end(); it++ )
    {
        if ( (*it)->getName().compare(item->text()) == 0 )
        {
            (*it)->setActive(item->checkState());
            (*it)->save();
        }
    }
}
