#include "snoozepage.h"
#include "ui_snoozepage.h"

SnoozePage::SnoozePage(Msg::Alarm *alarm, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::SnoozePage),
    _alarm(alarm)
{
    _ui->setupUi(this);

    _ui->snoozeEdit->setText(QString::number(_alarm->getSnoozeTime()).rightJustified(2, '0'));

        connect(_ui->snoozeUp, SIGNAL(clicked()), this, SLOT(up()));
        connect(_ui->snoozeDown, SIGNAL(clicked()), this, SLOT(down()));

        connect(_ui->backButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
        connect(_ui->setButton, SIGNAL(clicked()), this, SLOT(setSnooze()));
}

SnoozePage::~SnoozePage()
{
    delete _ui;
}

void SnoozePage::setSnooze()
{
    _alarm->setSnooze(_ui->snoozeEdit->text().toInt());
    deleteLater();
}

void SnoozePage::up()
{
        int snooze = _ui->snoozeEdit->text().toInt();
        _ui->snoozeEdit->setText(QString::number(++snooze).rightJustified(2, '0'));
}

void SnoozePage::down()
{
        int snooze = _ui->snoozeEdit->text().toInt();
        if ( snooze > 0 )
                _ui->snoozeEdit->setText(QString::number(--snooze).rightJustified(2, '0'));
}
