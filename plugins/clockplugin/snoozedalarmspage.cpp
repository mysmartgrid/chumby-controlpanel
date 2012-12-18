#include "snoozedalarmspage.h"
#include "ui_snoozedalarmspage.h"

#include <QtCore/QDebug>

#include <QtGui/QListWidgetItem>

#include "controlpanel/alarmdaemon.h"

SnoozedAlarmsPage::SnoozedAlarmsPage(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::SnoozedAlarmsPage)
{
    _ui->setupUi(this);

    connect(_ui->alarmList, SIGNAL(itemSelectionChanged()), this, SLOT(cancelSnooze()));
    connect(_ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
}

SnoozedAlarmsPage::~SnoozedAlarmsPage()
{
    delete _ui;
}

void SnoozedAlarmsPage::setSnoozed(std::list<Msg::Alarm*> *snoozed)
{
    qDebug() << "setSnoozed" << snoozed->size();
    _ui->alarmList->clear();

    for ( std::list<Msg::Alarm*>::iterator it = snoozed->begin(); it != snoozed->end(); it++ )
    {
        QListWidgetItem *item = new QListWidgetItem((*it)->getName() + " (" + (*it)->getNextFireTime().toString() + ")");
        _ui->alarmList->addItem(item);
    }
}

void SnoozedAlarmsPage::cancelSnooze()
{
    if ( _ui->alarmList->selectedItems().count() == 0 )
        return;

    Msg::Alarm *alarm = NULL;
    std::list<Msg::Alarm*> alarms = Msg::AlarmDaemon::getInstance().getAlarms();
    for ( std::list<Msg::Alarm*>::iterator it = alarms.begin(); it != alarms.end(); it++ )
    {
        if ( _ui->alarmList->selectedItems().first()->text().startsWith((*it)->getName()) )
        {
            qDebug() << (*it)->getName() << "==" << _ui->alarmList->selectedItems().first()->text();
            alarm = *it;
        }
    }

    alarm->cancelSnooze();
    delete _ui->alarmList->selectedItems().first();
    _ui->alarmList->clearSelection();
}
