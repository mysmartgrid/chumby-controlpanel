#ifndef ALARMDAEMON_H
#define ALARMDAEMON_H

#include "alarmwidget.h"
#include "audioplugin.h"

#include <QtCore/QDateTime>
#include <QtCore/QTime>
#include <QtCore/QSettings>
#include <QtCore/QTimer>

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
    Alarm(QString name, QObject* parent = 0);
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

public slots:
    bool run();
    void dismiss();
    void snooze();

private:
    QString name;
    QTime time;
    Weekdays weekdays; // bit field encoding repetitions
    QString source;
    int snoozeTime;
    bool active;
    Msg::AudioPlugin* plugin;
    QTimer* timer;
    QWidget* widget; // AlarmWidget
};

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
    static AlarmDaemon* instance;
    std::list<Alarm*> *alarms;
    bool alarmActive;

    QSettings* settings;
};

#endif // ALARMDAEMON_H