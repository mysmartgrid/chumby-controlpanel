#include "alarmwizard.h"

#include "daypage.h"
#include "namepage.h"
#include "snoozepage.h"
#include "sourcepage.h"
#include "summarypage.h"
#include "timepage.h"

#include <QtCore/QDebug>

AlarmWizard::AlarmWizard(QWidget *parent, Alarm* alarm) :
        QWizard(parent)
{
        setOption(QWizard::IndependentPages);
        setPage(TIMEPAGE, new TimePage());
        setPage(DAYPAGE, new DayPage());
        setPage(SOURCEPAGE, new SourcePage());
        setPage(SNOOZEPAGE, new SnoozePage());
        setPage(NAMEPAGE, new NamePage());
        setPage(SUMMARYPAGE, new SummaryPage());
        _nextPage = SUMMARYPAGE;

        _alarm = NULL;
        if ( alarm )
        {
                _alarm = alarm;
                //load settings
                qDebug() << "loading alarm:";
                qDebug() << "time:" << alarm->getHour() << ":" << alarm->getMinute();
                setField("hour", alarm->getHour());
                setField("minute", alarm->getMinute());
                Weekdays days = alarm->getDays();
                if ( days.saturday && days.sunday )
                        setField("weekend", true);
                else
                {
                        setField("saturday", days.saturday);
                        setField("sunday", days.sunday);
                }
                if ( days.monday &&
                                days.tuesday &&
                                days.wednesday &&
                                days.thursday &&
                                days.friday )
                        setField("weekday", true);
                else
                {
                        setField("monday", days.monday);
                        setField("tuesday", days.tuesday);
                        setField("wednesday", days.wednesday);
                        setField("thursday", days.thursday);
                        setField("friday", days.friday);
                }
                setField("snooze", alarm->getSnoozeTime());
                setField("name", alarm->getName());
                _source = alarm->getSource();
        } else {
                setField("hour", "");
                setField("minute", "");
                setField("monday", false);
                setField("tuesday", false);
                setField("wednesday", false);
                setField("thursday", false);
                setField("friday", false);
                setField("saturday", false);
                setField("sunday", false);
                setField("weekday", false);
                setField("weekend", false);
                setField("name", "");
                setField("snooze", "");
                _source = "";
        }

        //setStartId(0); //not needed as default startId is 0
}

void AlarmWizard::accept()
{
        qDebug() << "accept";
        if ( _alarm ) {
                AlarmDaemon::getInstance().removeAlarm(_alarm);
                delete _alarm;
                _alarm = NULL;
        }
        _alarm = new Alarm(field("name").toString());
        qDebug() << "saving alarm:" << field("name").toString();
        qDebug() << "time:" << field("hour").toString() << ":" << field("minute").toString();
        _alarm->setTime(field("hour").toInt(), field("minute").toInt());
        if ( field("weekday").toBool() )
        {
                setField("monday", true);
                setField("tuesday", true);
                setField("wednesday", true);
                setField("thursday", true);
                setField("friday", true);
        }
        if ( field("weekend").toBool() )
        {
                setField("saturday", true);
                setField("sunday", true);
        }
        Weekdays days = { field("monday").toBool(),
                          field("tuesday").toBool(),
                          field("wednesday").toBool(),
                          field("thursday").toBool(),
                          field("friday").toBool(),
                          field("saturday").toBool(),
                          field("sunday").toBool()
                        };
        _alarm->setDays(days);
        _alarm->setSource(_source);
        _alarm->setSnooze(field("snooze").toInt());
        AlarmDaemon::getInstance().addAlarm(_alarm);
        QWizard::accept();
}

int AlarmWizard::getNextPage()
{
        return _nextPage;
}

void AlarmWizard::addPage(QWizardPage* page)
{
        setPage(++_nextPage, page);
}

QString AlarmWizard::getSource()
{
        return _source;
}

void AlarmWizard::setSource(QString source)
{
        _source = source;
}
