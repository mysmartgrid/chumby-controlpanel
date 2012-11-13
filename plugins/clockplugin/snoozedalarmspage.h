#ifndef SNOOZEDALARMSPAGE_H
#define SNOOZEDALARMSPAGE_H

#include <QWidget>

#include "controlpanel/alarm.h"

namespace Ui {
class SnoozedAlarmsPage;
}

class SnoozedAlarmsPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit SnoozedAlarmsPage(QWidget *parent = 0);
    ~SnoozedAlarmsPage();
    void setSnoozed(std::list<Msg::Alarm*>* snoozed);

public slots:
    void cancelSnooze();
    
private:
    Ui::SnoozedAlarmsPage *_ui;
    std::list<Msg::Alarm*> *_snoozed;
};

#endif // SNOOZEDALARMSPAGE_H
