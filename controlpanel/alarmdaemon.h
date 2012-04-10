#ifndef ALARMDAEMON_H
#define ALARMDAEMON_H

#include <QtCore/QDateTime>
#include <QtCore/QTime>

typedef struct {
    bool monday;
    bool tuesday;
    bool wednesday;
    bool thursday;
    bool friday;
    bool saturday;
    bool sunday;
} Weekdays;

class Alarm
{
public:
    Alarm();
    bool setTime(unsigned int hour, unsigned int minute);
    void setDays(bool monday = false,
                 bool tuesday = false,
                 bool wednesday = false,
                 bool thursday = false,
                 bool friday = false,
                 bool saturday = false,
                 bool sunday = false);
    bool check(QDateTime current);
    bool isActive();


private:
    QTime time;
    Weekdays weekdays; // bit field encoding repetitions
    bool active;
};

class AlarmDaemon
{
public:
    AlarmDaemon();
    void check();

private:
    std::list<Alarm*> alarms;
};

#endif // ALARMDAEMON_H
