#include "daypage.h"
#include "ui_daypage.h"

DayPage::DayPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::DayPage)
{
    ui->setupUi(this);
		
		registerField("monday", ui->monBox);
		registerField("tuesday", ui->tueBox);
		registerField("wednesday", ui->wedBox);
		registerField("thursday", ui->thuBox);
		registerField("friday", ui->friBox);
		registerField("saturday", ui->satBox);
		registerField("sunday", ui->sunBox);
		registerField("weekday", ui->weekBox);
		registerField("weekend", ui->weekEndBox);
}

DayPage::~DayPage()
{
    delete ui;
}

void DayPage::initializePage()
{
	if ( field("monday").toBool() )
		ui->monBox->setChecked(true);
	if ( field("tuesday").toBool() )
		ui->tueBox->setChecked(true);
	if ( field("wednesday").toBool() )
		ui->wedBox->setChecked(true);
	if ( field("thursday").toBool() )
		ui->thuBox->setChecked(true);
	if ( field("friday").toBool() )
		ui->friBox->setChecked(true);
	if ( field("saturday").toBool() )
		ui->satBox->setChecked(true);
	if ( field("sunday").toBool() )
		ui->sunBox->setChecked(true);
	if ( field("weekday").toBool() )
		ui->weekBox->setChecked(true);
	if ( field("weekend").toBool() )
		ui->weekEndBox->setChecked(true);
}
