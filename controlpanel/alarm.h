#ifndef ALARM_H
#define ALARM_H

#include <QtCore/QObject>

#include <QtCore/QDateTime>
#include <QtCore/QTime>
#include <QtCore/QSettings>
#include <QtCore/QTimer>

#include "audioplugin.h"

namespace Msg
{
    typedef struct {
        bool monday;
        bool tuesday;
        bool wednesday;
        bool thursday;
        bool friday;
        bool saturday;
        bool sunday;
    } Weekdays;

    class Alarm : public QObject
    {
        Q_OBJECT
    public:
        Alarm(QString name = "", QObject* parent = 0);
        void save();
        void remove();
        bool setTime(unsigned int hour, unsigned int minute);
        bool setTime(QTime time);
        void setDays(bool monday = false,
                     bool tuesday = false,
                     bool wednesday = false,
                     bool thursday = false,
                     bool friday = false,
                     bool saturday = false,
                     bool sunday = false);
        void setDays(Weekdays days);
        void setSource(QString source);
        void setSnooze(int snooze);
        void setName(QString name);
        void setVolume(int vol);
        bool check(QDateTime current);
        bool isActive();
        void setActive(bool active);
        QString getName();
        QString getTime();
        int getHour();
        int getMinute();
        Weekdays getDays();
        QString getSource();
        int getSnoozeTime();
        int getVolume();

    public slots:
        bool run();
        void dismiss();
        void snooze();

    private:
        QString _name;
        QTime _time;
        Weekdays _weekdays; // bit field encoding repetitions
        QString _source;
        int _volume;
        int _tmpVol;
        int _snoozeTime;
        bool _active;
        Msg::AudioPlugin* _plugin;
        QTimer* _timer;
        QWidget* _widget; // AlarmWidget
    };
}

#endif // ALARM_H
