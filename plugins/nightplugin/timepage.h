#ifndef TIMEPAGE_H
#define TIMEPAGE_H

#include <QWidget>

#include "alarm.h"

namespace Ui {
class TimePage;
}

class TimePage : public QWidget
{
    Q_OBJECT

public:
    explicit TimePage(Msg::Alarm *alarm, QWidget *parent = 0);
    ~TimePage();

private slots:
    void setTime();
    void hourUp();
    void hourDown();
    void minuteUp();
    void minuteDown();

private:
    Ui::TimePage *_ui;
    Msg::Alarm *_alarm;
};

#endif // TIMEPAGE_H
