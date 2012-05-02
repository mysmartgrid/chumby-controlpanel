#include "alarmwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QKeyEvent>

#include <QtCore/QTimer>
#include <QtCore/QDebug>

AlarmWidget::AlarmWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton* snooze = new QPushButton("Snooze");
    connect(snooze, SIGNAL(clicked()), this, SIGNAL(snoozed()));
    layout->addWidget(snooze);
    layout->addWidget(new QLabel("Alarm! Alarm! Alarm!"));
    QPushButton* dismiss = new QPushButton("Dismiss");
    connect(dismiss, SIGNAL(clicked()), this, SIGNAL(dismissed()));
    layout->addWidget(dismiss);
    this->setLayout(layout);
//    connect(this, SIGNAL(snoozed()), this, SLOT(hide()));
//    connect(this, SIGNAL(dismissed()), this, SLOT(hide()));
}
