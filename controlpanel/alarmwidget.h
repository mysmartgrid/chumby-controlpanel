#ifndef ALARMWIDGET_H
#define ALARMWIDGET_H

#include <QWidget>

class AlarmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = 0);
    
signals:
    void dismissed();
    void snoozed();
    void resumed();
};

#endif // ALARMWIDGET_H
