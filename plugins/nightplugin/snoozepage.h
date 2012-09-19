#ifndef SNOOZEPAGE_H
#define SNOOZEPAGE_H

#include <QWidget>

#include "alarm.h"

namespace Ui {
class SnoozePage;
}

class SnoozePage : public QWidget
{
        Q_OBJECT

public:
        explicit SnoozePage(Msg::Alarm *alarm, QWidget *parent =0);
        ~SnoozePage();

private slots:
    void setSnooze();
        void up();
        void down();

private:
        Ui::SnoozePage *_ui;
        Msg::Alarm *_alarm;
};

#endif // SNOOZEPAGE_H
