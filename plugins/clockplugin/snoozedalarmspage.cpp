#include "snoozedalarmspage.h"
#include "ui_snoozedalarmspage.h"

SnoozedAlarmsPage::SnoozedAlarmsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnoozedAlarmsPage)
{
    ui->setupUi(this);
}

SnoozedAlarmsPage::~SnoozedAlarmsPage()
{
    delete ui;
}
