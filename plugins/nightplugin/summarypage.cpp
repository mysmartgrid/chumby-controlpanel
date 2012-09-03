#include "summarypage.h"
#include "ui_summarypage.h"

#include "alarmwizard.h"

#include <QtCore/QDebug>

SummaryPage::SummaryPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SummaryPage)
{
    ui->setupUi(this);
}

void SummaryPage::initializePage()
{
        ui->textBrowser->clear();
        ui->textBrowser->append(field("name").toString());
        ui->textBrowser->append("Time: " + field("hour").toString() + ":" + field("minute").toString());
        QString rep = "";
        if ( field("weekday").toBool() && field("weekend").toBool() )
            rep += "Everyday, ";
        else if ( field("weekday").toBool() )
            rep += "Weekdays, ";
        else if ( field("weekend").toBool() )
            rep += "Weekend, ";
        else
        {
            if ( field("monday").toBool() )
                rep += "Monday, ";
            if ( field("tuesday").toBool() )
                rep += "Tuesday, ";
            if ( field("wednesday").toBool() )
                rep += "Wednesday, ";
            if ( field("thursday").toBool() )
                rep += "Thursday, ";
            if ( field("friday").toBool() )
                rep += "Friday, ";
            if ( field("saturday").toBool() )
                rep += "Saturday, ";
            if ( field("sunday").toBool() )
                rep += "Sunday, ";
        }
        rep.chop(2);
        ui->textBrowser->append("Repetitions: " + rep);
        ui->textBrowser->append("Ringtone: " + ((AlarmWizard*) wizard())->getSource());
        ui->textBrowser->append("Snoozetime: " + field("snooze").toString());
}

SummaryPage::~SummaryPage()
{
    delete ui;
}

int SummaryPage::nextId() const
{
    return -1;
}
