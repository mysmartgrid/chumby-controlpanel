#include "alarmdaemon.h"

#include "musiccontrol.h"

#include <QtCore/QDateTime>
#include <QtCore/QStringList>

#include <QtCore/QDebug>

AlarmDaemon* AlarmDaemon::instance = 0;

AlarmDaemon& AlarmDaemon::getInstance()
{
    if ( !instance )
        instance = new AlarmDaemon;

    return *instance;
}

AlarmDaemon::AlarmDaemon()
    :QObject()
{
    alarms = new std::list<Alarm*>();
    settings = new QSettings("/mnt/usb/alarms.conf", QSettings::NativeFormat);
    QStringList storedAlarms = settings->childGroups();
    for ( QStringList::iterator it = storedAlarms.begin(); it != storedAlarms.end(); it++ )
    {
        settings->beginGroup(*it);
        Alarm* newAlarm = new Alarm(*it);
        newAlarm->setTime(QTime().fromString(settings->value("time").toString(), "hh:mm"));
        alarms->push_back(newAlarm);
        settings->endGroup();
    }
}

AlarmDaemon::~AlarmDaemon()
{
}

void AlarmDaemon::check()
{
    QDateTime time = QDateTime::currentDateTime();
    qDebug() << time.toString();
    if ( alarms )
    {
        for ( std::list<Alarm*>::iterator it = alarms->begin(); it != alarms->end(); it++ )
        {
            if ((*it)->check(time))
            {
                qDebug() << "Activating alarm";
                connect(this, SIGNAL(snooze()), *it, SLOT(snooze()));
                if ((*it)->run())
                {
                    qDebug() << "Alarm successfully started";
                } else {
                    qDebug() << "Error starting alarm";
                    //TODO: fallback alarm!
                }
            }
        }
    }
}

void AlarmDaemon::addAlarm(Alarm* new_alarm)
{
    settings->beginGroup(new_alarm->toString());
    settings->setValue("time", new_alarm->getTime());
    Weekdays days = new_alarm->getDays();
    settings->setValue("monday", days.monday);
    settings->setValue("tuesday", days.tuesday);
    settings->setValue("wednesday", days.wednesday);
    settings->setValue("thursday", days.thursday);
    settings->setValue("friday", days.friday);
    settings->setValue("saturday", days.saturday);
    settings->setValue("sunday", days.sunday);
    settings->setValue("source", new_alarm->getSource());
    settings->endGroup();
    this->alarms->push_back(new_alarm);
    emit alarmAdded(new_alarm);
    qDebug() << "AlarmDaemon: Alarm" << new_alarm->toString() << "added";
}

std::list<Alarm*> AlarmDaemon::getAlarms()
{
    return *(this->alarms);
}

void AlarmDaemon::setAlarmActive(bool active)
{
    alarmActive = active;
}

bool AlarmDaemon::isAlarmActive()
{
    return alarmActive;
}

Alarm::Alarm(QString name, QObject *parent)
    : QObject(parent)
{
    this->name = name;
    this->active = true;
}

bool Alarm::setTime(unsigned int hour, unsigned int minute)
{
    return this->time.setHMS(hour, minute, 0);
}

bool Alarm::setTime(QTime time)
{
    this->time = time;

    return true;
}

void Alarm::setDays(bool monday, bool tuesday, bool wednesday, bool thursday, bool friday, bool saturday, bool sunday)
{
    this->weekdays.monday = monday;
    this->weekdays.tuesday = tuesday;
    this->weekdays.wednesday = wednesday;
    this->weekdays.thursday = thursday;
    this->weekdays.friday = friday;
    this->weekdays.saturday = saturday;
    this->weekdays.sunday = sunday;
}

void Alarm::setSource(QString source)
{
    this->source = source;
}

bool Alarm::check(QDateTime current)
{
    if ( !this->isActive() )
        return false;

    qDebug() << current.date().dayOfWeek();

    switch (current.date().dayOfWeek())
    {
    case 0: //Sunday
        if ( ! this->weekdays.sunday )
            return false;
        break;
    case 1: //Monday
        if ( ! this->weekdays.monday )
            return false;
        break;
    case 2: //Tuesday
        if ( ! this->weekdays.tuesday )
            return false;
        break;
    case 3: //Wednesday
        if ( ! this->weekdays.wednesday )
            return false;
        break;
    case 4: //Thursday
        if ( ! this->weekdays.thursday )
            return false;
        break;
    case 5: //Friday
        if ( ! this->weekdays.friday )
            return false;
        break;
    case 6: //Saturday
        if ( ! this->weekdays.saturday )
            return false;
        break;
    }

    if ( current.time().hour() != this->time.hour() || current.time().minute() != this->time.minute() )
        return false;

    return true;
}

bool Alarm::run()
{
    int index = this->source.indexOf("/");
    QString audioPlugin = this->source.left(index);
    QString pluginSource = this->source;
    pluginSource.remove(0, index + 1);
    qDebug() << "Running" << audioPlugin << "with source" << pluginSource;
    if ( audioPlugin.length() == 0 || pluginSource.length() == 0 )
        return false;
    plugin = Msg::MusicControl::getInstance().getAudioPlugin(audioPlugin);
    if ( ! plugin )
        return false;

    qDebug() << "Plugin for alarm loaded";
    plugin->init();

    if ( ! plugin->play(pluginSource) )
        return false;

    qDebug() << "Alarm started";

    widget = new AlarmWidget();
    timer = new QTimer();
    connect((AlarmWidget*) widget, SIGNAL(dismissed()), this, SLOT(dismiss()));
    connect((AlarmWidget*) widget, SIGNAL(snoozed()), this, SLOT(snooze()));
    connect((AlarmWidget*) widget, SIGNAL(resumed()), this, SLOT(run()));
    widget->showFullScreen();

    AlarmDaemon::getInstance().setAlarmActive(true);

    return true;
}

void Alarm::snooze()
{
    qDebug() << "Starting timer...";
    timer->start(5000);
    connect(timer, SIGNAL(timeout()), (AlarmWidget*) widget, SIGNAL(resumed()));
    connect(timer, SIGNAL(timeout()), timer, SLOT(stop()));
    dismiss();
}

void Alarm::dismiss()
{
    AlarmDaemon::getInstance().setAlarmActive(false);
    plugin->stop();
    widget->hide();
    //delete widget;
}

QString Alarm::toString()
{
    return this->name;
}

bool Alarm::isActive()
{
    return this->active;
}

QString Alarm::getTime()
{
    return this->time.toString("hh:mm");
}

Weekdays Alarm::getDays()
{
    return this->weekdays;
}

QString Alarm::getSource()
{
    return this->source;
}
