#include "timepage.h"
#include "ui_timepage.h"

#include <QtCore/QDebug>
#include <QtCore/QTime>

TimePage::TimePage(Msg::Alarm *alarm, QWidget *parent) :
  QWidget(parent),
  _ui(new Ui::TimePage),
  _alarm(alarm)
{
    _ui->setupUi(this);

    _ui->hourEdit->setText(QString::number(_alarm->getHour()).rightJustified(2, '0'));
    _ui->minEdit->setText(QString::number(_alarm->getMinute()).rightJustified(2, '0'));

    connect(_ui->hourUp, SIGNAL(clicked()), this, SLOT(hourUp()));
    connect(_ui->hourDown, SIGNAL(clicked()), this, SLOT(hourDown()));
    connect(_ui->minUp, SIGNAL(clicked()), this, SLOT(minuteUp()));
    connect(_ui->minDown, SIGNAL(clicked()), this, SLOT(minuteDown()));

    connect(_ui->backButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect(_ui->setButton, SIGNAL(clicked()), this, SLOT(setTime()));
}

TimePage::~TimePage()
{
    delete _ui;
}

void TimePage::hourUp()
{
    int hour = _ui->hourEdit->text().toInt();
    if ( hour < 23 )
        _ui->hourEdit->setText(QString::number(hour+1).rightJustified(2, '0'));
    else
        _ui->hourEdit->setText("00");
}

void TimePage::hourDown()
{
    int hour = _ui->hourEdit->text().toInt();
    if ( hour > 0 )
        _ui->hourEdit->setText(QString::number(hour-1).rightJustified(2, '0'));
    else
        _ui->hourEdit->setText("23");
}

void TimePage::minuteUp()
{
    int minute = _ui->minEdit->text().toInt();
    if ( minute < 59 )
        _ui->minEdit->setText(QString::number(minute+1).rightJustified(2, '0'));
    else
        _ui->minEdit->setText("00");
}

void TimePage::minuteDown()
{
    int minute = _ui->minEdit->text().toInt();
    if ( minute > 0 )
        _ui->minEdit->setText(QString::number(minute-1).rightJustified(2, '0'));
    else
        _ui->minEdit->setText("59");
}

void TimePage::setTime()
{
    _alarm->setTime(_ui->hourEdit->text().toInt(), _ui->minEdit->text().toInt());
    deleteLater();
}
