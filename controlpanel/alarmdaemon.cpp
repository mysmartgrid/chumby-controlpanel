#include "alarmdaemon.h"


#include <QtCore/QDateTime>
#include <QtCore/QStringList>

#include <QtCore/QDebug>

Msg::AlarmDaemon* Msg::AlarmDaemon::_instance = 0;

Msg::AlarmDaemon& Msg::AlarmDaemon::getInstance()
{
    if ( !_instance )
        _instance = new Msg::AlarmDaemon;

    return *_instance;
}

Msg::AlarmDaemon::AlarmDaemon()
    :QObject(),
      _alarms(new std::list<Msg::Alarm*>()),
      _alarmActive(false),
      _settings(new QSettings("/mnt/usb/alarms.conf", QSettings::NativeFormat))
{
    _settings->setIniCodec("UTF-8");
    QStringList storedAlarms = _settings->childGroups();
    for ( QStringList::iterator it = storedAlarms.begin(); it != storedAlarms.end(); it++ )
    {
        _settings->beginGroup(*it);
        Msg::Alarm* newAlarm = new Msg::Alarm(*it);
        newAlarm->setTime(QTime().fromString(_settings->value("time").toString(), "hh:mm"));
        bool monday = _settings->value("monday").toBool();
        bool tuesday = _settings->value("tuesday").toBool();
        bool wednesday = _settings->value("wednesday").toBool();
        bool thursday = _settings->value("thursday").toBool();
        bool friday = _settings->value("friday").toBool();
        bool saturday = _settings->value("saturday").toBool();
        bool sunday = _settings->value("sunday").toBool();
        newAlarm->setDays(monday, tuesday, wednesday, thursday, friday, saturday, sunday);
        newAlarm->setSnooze(_settings->value("snooze").toInt());
        newAlarm->setSource(_settings->value("source").toString());
        _alarms->push_back(newAlarm);
        _settings->endGroup();
    }
}

Msg::AlarmDaemon::~AlarmDaemon()
{
}

void Msg::AlarmDaemon::check()
{
    if ( !isAlarmActive() )
    {
        QDateTime time = QDateTime::currentDateTime();
        qDebug() << time.toString();
        if ( _alarms )
        {
            for ( std::list<Msg::Alarm*>::iterator it = _alarms->begin(); it != _alarms->end(); it++ )
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
}

void Msg::AlarmDaemon::addAlarm(Msg::Alarm* new_alarm)
{
    /*settings->beginGroup(new_alarm->toString());
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
    settings->endGroup();*/
    new_alarm->save();
    _alarms->push_back(new_alarm);
    emit alarmAdded(new_alarm);
    qDebug() << "AlarmDaemon: Alarm" << new_alarm->getName() << "added";
}

void Msg::AlarmDaemon::removeAlarm(Msg::Alarm* alarm)
{
    alarm->remove();
    _alarms->remove(alarm);
}

std::list<Msg::Alarm*> Msg::AlarmDaemon::getAlarms()
{
    return *(_alarms);
}

void Msg::AlarmDaemon::setAlarmActive(bool active)
{
    _alarmActive = active;
}

bool Msg::AlarmDaemon::isAlarmActive()
{
    return _alarmActive;
}

QSettings* Msg::AlarmDaemon::getSettings()
{
    return _settings;
}
