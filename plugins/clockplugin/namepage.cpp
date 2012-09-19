#include "namepage.h"
#include "ui_namepage.h"

NamePage::NamePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NamePage)
{
    ui->setupUi(this);

        registerField("name", ui->nameEdit);
}

NamePage::~NamePage()
{
    delete ui;
}

void NamePage::initializePage()
{
    //if ( field("name").toString().isEmpty() )
    //{
        QString name = "";
        if ( field("weekday").toBool() && field("weekend").toBool() )
            name += "Everyday, ";
        else if ( field("weekday").toBool() )
            name += "Weekdays, ";
        else if ( field("weekend").toBool() )
            name += "Weekend, ";
        else
        {
            if ( field("monday").toBool() )
                name += "Monday, ";
            if ( field("tuesday").toBool() )
                name += "Tuesday, ";
            if ( field("wednesday").toBool() )
                name += "Wednesday, ";
            if ( field("thursday").toBool() )
                name += "Thursday, ";
            if ( field("friday").toBool() )
                name += "Friday, ";
            if ( field("saturday").toBool() )
                name += "Saturday, ";
            if ( field("sunday").toBool() )
                name += "Sunday, ";
        }
        name += "at " + field("hour").toString() + ":" + field("minute").toString();
        ui->nameEdit->setText(name);
    //} else {
    //    ui->nameEdit->setText(field("name").toString());
    //}
}
