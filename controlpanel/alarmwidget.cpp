#include "alarmwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QKeyEvent>

#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QDebug>

Msg::AlarmWidget::AlarmWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::alarmWidget()),
    _timer(new QTimer())
{
    _ui->setupUi(this);

    updateClock();
    _timer->start(1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateClock()));

    connect( _ui->dismissButton, SIGNAL(clicked()), this, SIGNAL(dismissed()));
    connect( _ui->snoozeButton, SIGNAL(clicked()), this, SIGNAL(snoozed()));
}

void Msg::AlarmWidget::updateClock()
{
    _ui->timeLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
}
