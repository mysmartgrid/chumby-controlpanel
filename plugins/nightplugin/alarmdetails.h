#ifndef ALARMDETAILS_H
#define ALARMDETAILS_H

#include <QWidget>

#include "../../controlpanel/alarmdaemon.h"

namespace Ui {
class AlarmDetails;
}

class AlarmDetails : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmDetails(QWidget *parent = 0, Msg::Alarm* alarm = 0);
    ~AlarmDetails();

private slots:
        void editAlarm();
        void deleteAlarm();

private:
    Ui::AlarmDetails *_ui;
    Msg::Alarm *_alarm;
};

#endif // ALARMDETAILS_H
