#ifndef VOLUMEPAGE_H
#define VOLUMEPAGE_H

#include <QWidget>

#include "alarm.h"

namespace Ui {
class VolumePage;
}

class VolumePage : public QWidget
{
    Q_OBJECT
    
public:
    explicit VolumePage(Msg::Alarm *alarm, QWidget *parent = 0);
    ~VolumePage();

private slots:
    void setVolume();
    void playTestTone(int);
    
private:
    Ui::VolumePage *_ui;
    Msg::Alarm *_alarm;
    int _sysVolume;
};

#endif // VOLUMEPAGE_H
