#ifndef ALARMWIDGET_H
#define ALARMWIDGET_H

#include "ui_alarmwidget.h"

#include <QWidget>
#include <QtGui/QLabel>
#include <QtCore/QTimer>

class AlarmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = 0);

signals:
    void dismissed();
    void snoozed();
    void resumed();

private slots:
    void updateClock();

private:
    Ui::alarmWidget *_ui;
    QTimer* timer;
};

#endif // ALARMWIDGET_H
