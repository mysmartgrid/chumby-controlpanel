#include "timepage.h"
#include "ui_timepage.h"

#include <QtCore/QDebug>
#include <QtCore/QTime>

TimePage::TimePage(QWidget *parent) :
  QWizardPage(parent),
  _ui(new Ui::TimePage)
{
    _ui->setupUi(this);

    registerField("hour", _ui->hourEdit);
    registerField("minute", _ui->minEdit);
}

void TimePage::initializePage()
{
    if ( ! field("hour").toString().isEmpty() )
    {
        qDebug() << "loaded time:" << field("hour").toString() << field("minute").toString();
        _ui->hourEdit->setText(QString::number(field("hour").toInt()).rightJustified(2, '0'));
        _ui->minEdit->setText(QString::number(field("minute").toInt()).rightJustified(2, '0'));
    } else {
        QTime now = QTime::currentTime();
        _ui->hourEdit->setText(QString::number(now.hour()).rightJustified(2, '0'));
        _ui->minEdit->setText(QString::number(now.minute()).rightJustified(2, '0'));
    }

    connect(_ui->hourUp, SIGNAL(clicked()), this, SLOT(hourUp()));
    connect(_ui->hourDown, SIGNAL(clicked()), this, SLOT(hourDown()));
    connect(_ui->minUp, SIGNAL(clicked()), this, SLOT(minuteUp()));
    connect(_ui->minDown, SIGNAL(clicked()), this, SLOT(minuteDown()));
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
