#include "snoozepage.h"
#include "ui_snoozepage.h"

SnoozePage::SnoozePage(QWidget *parent) :
    QWizardPage(parent),
    _ui(new Ui::SnoozePage)
{
    _ui->setupUi(this);

    registerField("snooze", _ui->snoozeEdit);

        connect(_ui->snoozeUp, SIGNAL(clicked()), this, SLOT(up()));
        connect(_ui->snoozeDown, SIGNAL(clicked()), this, SLOT(down()));
}

void SnoozePage::initializePage()
{
        if ( ! field("snooze").toString().isEmpty() )
                _ui->snoozeEdit->setText(field("snooze").toString());
        else
                _ui->snoozeEdit->setText("05");
}

SnoozePage::~SnoozePage()
{
    delete _ui;
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
