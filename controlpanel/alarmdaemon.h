#ifndef ALARMDAEMON_H
#define ALARMDAEMON_H

#include "alarm.h"
#include "alarmwidget.h"

namespace Msg
{
    class AlarmDaemon: public QObject
    {
        Q_OBJECT

    public:
        static AlarmDaemon& getInstance();
        void check();
        void addAlarm(Alarm *new_alarm);
        void removeAlarm(Alarm *alarm);
        std::list<Alarm*> getAlarms();
        void setAlarmActive(bool active);
        bool isAlarmActive();
        QSettings* getSettings();

    signals:
        void alarmAdded(Alarm* newAlarm);
        void snooze();

    private:
        AlarmDaemon();
        AlarmDaemon(const AlarmDaemon&);
        ~AlarmDaemon();
        static AlarmDaemon* _instance;
        std::list<Alarm*> *_alarms;
        bool _alarmActive;

        QSettings* _settings;
    };
}

#endif // ALARMDAEMON_H
