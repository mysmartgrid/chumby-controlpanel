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
    Alarm(QString name);
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
    QString toString();


private:
    QString name;
    QTime time;
    Weekdays weekdays; // bit field encoding repetitions
    bool active;
};

class AlarmDaemon
{
public:
    static AlarmDaemon& getInstance();
    void check();
    void addAlarm(Alarm *new_alarm);
    std::list<Alarm*> getAlarms();

private:
    AlarmDaemon();
    AlarmDaemon(const AlarmDaemon&) {}
    ~AlarmDaemon();
    static AlarmDaemon* instance;
    std::list<Alarm*> alarms;
};

#endif // ALARMDAEMON_H
