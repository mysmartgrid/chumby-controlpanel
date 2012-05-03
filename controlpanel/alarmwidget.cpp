#include "alarmwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QKeyEvent>

#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QDebug>

AlarmWidget::AlarmWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton* snooze = new QPushButton("Snooze");
    clock = new QLabel("");
    QPushButton* dismiss = new QPushButton("Dismiss");
    connect(snooze, SIGNAL(clicked()), this, SIGNAL(snoozed()));
    layout->addWidget(snooze);
    layout->addWidget(clock);
    connect(dismiss, SIGNAL(clicked()), this, SIGNAL(dismissed()));
    layout->addWidget(dismiss);
    this->setLayout(layout);

    this->updateClock();
    timer = new QTimer();
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));

//    connect(this, SIGNAL(snoozed()), this, SLOT(hide()));
//    connect(this, SIGNAL(dismissed()), this, SLOT(hide()));
}

void AlarmWidget::updateClock()
{
    clock->setText(QTime::currentTime().toString("hh:mm:ss"));
}
