#include "alarm.h"

#include <QtCore/QDebug>

#include "alarmdaemon.h"
#include "musiccontrol.h"

Msg::Alarm::Alarm(QString name, QObject *parent)
    : QObject(parent),
      _name(name),
      _time(QTime::currentTime()),
      _source(""), //TODO: default ringtone
      _volume(Msg::MusicControl::getInstance().getMasterVolume()),
      _snoozeTime(5),
      _active(true),
      _plugin(NULL),
      _timer(new QTimer),
      _widget(NULL)
{
    _weekdays.monday = true;
    _weekdays.tuesday = true;
    _weekdays.wednesday = true;
    _weekdays.thursday = true;
    _weekdays.friday = true;
    _weekdays.saturday = true;
    _weekdays.sunday = true;
    if ( _name.isEmpty() )
        _name = _time.toString("hh:mm");
}

void Msg::Alarm::save()
{
    QSettings* settings = AlarmDaemon::getInstance().getSettings();
    settings->beginGroup(getName());
    settings->setValue("time", getTime());
    settings->setValue("monday", _weekdays.monday);
    settings->setValue("tuesday", _weekdays.tuesday);
    settings->setValue("wednesday", _weekdays.wednesday);
    settings->setValue("thursday", _weekdays.thursday);
    settings->setValue("friday", _weekdays.friday);
    settings->setValue("saturday", _weekdays.saturday);
    settings->setValue("sunday", _weekdays.sunday);
    settings->setValue("snooze", _snoozeTime);
    settings->setValue("source", getSource());
    settings->setValue("volume", _volume);
    settings->setValue("active", _active);
    settings->endGroup();
}

void Msg::Alarm::remove()
{
    QSettings* settings = AlarmDaemon::getInstance().getSettings();
    settings->beginGroup(getName());
    settings->remove("");
    settings->endGroup();
}

bool Msg::Alarm::setTime(unsigned int hour, unsigned int minute)
{
    return _time.setHMS(hour, minute, 0);
}

bool Msg::Alarm::setTime(QTime time)
{
    _time = time;

    return true;
}

void Msg::Alarm::setDays(bool monday, bool tuesday, bool wednesday, bool thursday, bool friday, bool saturday, bool sunday)
{
    _weekdays.monday = monday;
    _weekdays.tuesday = tuesday;
    _weekdays.wednesday = wednesday;
    _weekdays.thursday = thursday;
    _weekdays.friday = friday;
    _weekdays.saturday = saturday;
    _weekdays.sunday = sunday;
}

void Msg::Alarm::setDays(Msg::Weekdays days)
{
    _weekdays = days;
}

void Msg::Alarm::setSource(QString source)
{
    _source = source;
}

void Msg::Alarm::setSnooze(int snooze)
{
    _snoozeTime = snooze;
}

bool Msg::Alarm::check(QDateTime current)
{
    if ( !isActive() )
        return false;

    if ( _weekdays.monday
                    || _weekdays.tuesday
                    || _weekdays.wednesday
                    || _weekdays.thursday
                    || _weekdays.friday
                    || _weekdays.saturday
                    || _weekdays.sunday )
    {
            switch (current.date().dayOfWeek())
            {
            case 0: //Sunday
                if ( ! _weekdays.sunday )
                    return false;
                break;
            case 1: //Monday
                if ( ! _weekdays.monday )
                    return false;
                break;
            case 2: //Tuesday
                if ( ! _weekdays.tuesday )
                    return false;
                break;
            case 3: //Wednesday
                if ( ! _weekdays.wednesday )
                    return false;
                break;
            case 4: //Thursday
                if ( ! _weekdays.thursday )
                    return false;
                break;
            case 5: //Friday
                if ( ! _weekdays.friday )
                    return false;
                break;
            case 6: //Saturday
                if ( ! _weekdays.saturday )
                    return false;
                break;
            }
    }

    if ( current.time().hour() != _time.hour() || current.time().minute() != _time.minute() )
        return false;

    return true;
}

bool Msg::Alarm::run()
{
    int index = _source.indexOf("/");
    QString audioPlugin = _source.left(index);
    QString pluginSource = _source;
    pluginSource.remove(0, index + 1);
    qDebug() << "Running" << audioPlugin << "with source" << pluginSource;
    if ( audioPlugin.length() == 0 || pluginSource.length() == 0 )
        return false;
    _plugin = Msg::MusicControl::getInstance().getAudioPlugin(audioPlugin);
    if ( ! _plugin )
        return false;

    qDebug() << "Plugin for alarm loaded";
    _plugin->init();

    if ( ! _plugin->play(pluginSource) )
        return false;

    qDebug() << "Alarm started";

    if ( !_widget )
    {
        _widget = new AlarmWidget();
        connect((AlarmWidget*) _widget, SIGNAL(dismissed()), this, SLOT(dismiss()));
        connect((AlarmWidget*) _widget, SIGNAL(snoozed()), this, SLOT(snooze()));
        connect((AlarmWidget*) _widget, SIGNAL(resumed()), this, SLOT(run()));
    }
    _widget->showFullScreen();

    AlarmDaemon::getInstance().setAlarmActive(true);

    return true;
}

void Msg::Alarm::setActive(bool active)
{
    _active = active;
}

void Msg::Alarm::snooze()
{
        if ( _snoozeTime > 0 )
        {
                qDebug() << "Starting timer with" << _snoozeTime << "minutes.";
                /* QObject::startTimer: QTimer can only be used with threads started with QThread
                if ( timer )
                {
                        delete timer;
                        timer = NULL;
                }
                timer = new QTimer();*/
                _timer->start(_snoozeTime * 60 * 1000);
                connect(_timer, SIGNAL(timeout()), (AlarmWidget*) _widget, SIGNAL(resumed()));
                connect(_timer, SIGNAL(timeout()), _timer, SLOT(stop()));
                dismiss();
        }
}

void Msg::Alarm::dismiss()
{
    qDebug() << "dismiss";
    AlarmDaemon::getInstance().setAlarmActive(false);
    _plugin->stop();
    _widget->hide();
}

QString Msg::Alarm::getName()
{
    return _name;
}

void Msg::Alarm::setName(QString name)
{
    _name = name;
}

bool Msg::Alarm::isActive()
{
    return _active;
}

QString Msg::Alarm::getTime()
{
    return _time.toString("hh:mm");
}

int Msg::Alarm::getHour()
{
    return _time.hour();
}

int Msg::Alarm::getMinute()
{
    return _time.minute();
}

Msg::Weekdays Msg::Alarm::getDays()
{
    return _weekdays;
}

QString Msg::Alarm::getSource()
{
    return _source;
}

int Msg::Alarm::getSnoozeTime()
{
    return _snoozeTime;
}

void Msg::Alarm::setVolume(int vol)
{
    _volume = vol;
}

int Msg::Alarm::getVolume()
{
    return _volume;
}
