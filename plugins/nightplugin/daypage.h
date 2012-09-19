#ifndef DAYPAGE_H
#define DAYPAGE_H

#include <QWidget>
#include <QSignalMapper>

#include "alarm.h"

namespace Ui {
class DayPage;
}

class DayPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit DayPage(Msg::Alarm *alarm, QWidget *parent = 0);
    ~DayPage();

private slots:
    void setDays();
    void updateNo();
    void updateWeek();
    void updateWeekEnd();
    void updateMon();
    void updateTue();
    void updateWed();
    void updateThu();
    void updateFri();
    void updateSat();
    void updateSun();
    
private:
    Ui::DayPage *_ui;
    Msg::Alarm *_alarm;
    QSignalMapper *_mapper;
};

#endif // DAYPAGE_H
