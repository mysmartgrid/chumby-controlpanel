#include "alarmdaemon.h"

#include <QtCore/QDateTime>

#include <QtCore/QDebug>

AlarmDaemon::AlarmDaemon()
{
    alarms = std::list<Alarm*>();
    Alarm* testalarm = new Alarm();
    testalarm->setTime(13, 12);
    testalarm->setDays(false,true,false,false,false,false,false);
    alarms.push_back(testalarm);
}

void AlarmDaemon::check()
{
    QDateTime time = QDateTime::currentDateTime();
    qDebug() << time.toString();
    for ( std::list<Alarm*>::iterator it = alarms.begin(); it != alarms.end(); it++ )
    {
        if ((*it)->check(time))
            qDebug() << "Activating alarm";
    }
}

Alarm::Alarm()
{
    this->active = true;
}

bool Alarm::setTime(unsigned int hour, unsigned int minute)
{
    return this->time.setHMS(hour, minute, 0);
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

bool Alarm::check(QDateTime current)
{
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
