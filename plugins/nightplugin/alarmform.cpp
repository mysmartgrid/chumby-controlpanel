#include "alarmform.h"
#include "ui_alarmform.h"

#include "alarmdetails.h"
#include "alarmwizard.h"

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
    _ui->alarmList->addItem("New Alarm");
    std::list<Msg::Alarm*> alarms = Msg::AlarmDaemon::getInstance().getAlarms();
    for ( std::list<Msg::Alarm*>::iterator it = alarms.begin(); it != alarms.end(); it++ )
        _ui->alarmList->addItem((*it)->getName());
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

    if ( alarm )
    {
            AlarmDetails *widget = new AlarmDetails(NULL, alarm);
            connect( widget, SIGNAL(destroyed()), this, SLOT(refresh()) );
            widget->showFullScreen();
    }
    else
    {
            AlarmWizard *wizard = new AlarmWizard(NULL, alarm);
            connect( wizard, SIGNAL(accepted()), this, SLOT(refresh()) );
            connect( wizard, SIGNAL(rejected()), _ui->alarmList, SLOT(clearSelection()));
            wizard->showFullScreen();
    }
}
