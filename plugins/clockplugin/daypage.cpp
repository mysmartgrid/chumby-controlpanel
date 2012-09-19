#include "daypage.h"
#include "ui_daypage.h"

#include <QDebug>

DayPage::DayPage(Msg::Alarm *alarm, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::DayPage),
    _alarm(alarm)
{
    _ui->setupUi(this);

    Msg::Weekdays days = _alarm->getDays();
    _ui->monBox->setChecked(days.monday);
    _ui->tueBox->setChecked(days.tuesday);
    _ui->wedBox->setChecked(days.wednesday);
    _ui->thuBox->setChecked(days.thursday);
    _ui->friBox->setChecked(days.friday);
    _ui->satBox->setChecked(days.saturday);
    _ui->sunBox->setChecked(days.sunday);
    _ui->weekBox->setChecked(days.monday &&
                             days.tuesday &&
                             days.wednesday &&
                             days.thursday &&
                             days.friday);
    _ui->weekEndBox->setChecked(days.saturday &&
                                days.sunday);
    bool anyDay = days.monday || days.tuesday || days.wednesday || days.thursday || days.friday || days.saturday || days.sunday;
    _ui->noBox->setChecked(!anyDay);

    connect( _ui->noBox, SIGNAL(clicked()), this, SLOT(updateNo()));
    connect( _ui->weekBox, SIGNAL(clicked()), this, SLOT(updateWeek()));
    connect( _ui->weekEndBox, SIGNAL(clicked()), this, SLOT(updateWeekEnd()));
    connect( _ui->monBox, SIGNAL(clicked()), this, SLOT(updateMon()));
    connect( _ui->tueBox, SIGNAL(clicked()), this, SLOT(updateTue()));
    connect( _ui->wedBox, SIGNAL(clicked()), this, SLOT(updateWed()));
    connect( _ui->thuBox, SIGNAL(clicked()), this, SLOT(updateThu()));
    connect( _ui->friBox, SIGNAL(clicked()), this, SLOT(updateFri()));
    connect( _ui->satBox, SIGNAL(clicked()), this, SLOT(updateSat()));
    connect( _ui->sunBox, SIGNAL(clicked()), this, SLOT(updateSun()));

    connect( _ui->backButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect( _ui->setButton, SIGNAL(clicked()), this, SLOT(setDays()));
}

DayPage::~DayPage()
{
    delete _ui;
}

void DayPage::setDays()
{
    Msg::Weekdays days = _alarm->getDays();
    days.monday = _ui->monBox->checkState();
    days.tuesday = _ui->tueBox->checkState();
    days.wednesday = _ui->wedBox->checkState();
    days.thursday = _ui->thuBox->checkState();
    days.friday = _ui->friBox->checkState();
    days.saturday = _ui->satBox->checkState();
    days.sunday = _ui->sunBox->checkState();
    _alarm->setDays(days);
    deleteLater();
}

void DayPage::updateNo()
{
        _ui->weekBox->setChecked(!_ui->noBox->checkState());
        _ui->weekEndBox->setChecked(!_ui->noBox->checkState());
        updateWeek();
        updateWeekEnd();
}

void DayPage::updateWeek()
{
    _ui->monBox->setChecked(_ui->weekBox->checkState());
    _ui->tueBox->setChecked(_ui->weekBox->checkState());
    _ui->wedBox->setChecked(_ui->weekBox->checkState());
    _ui->thuBox->setChecked(_ui->weekBox->checkState());
    _ui->friBox->setChecked(_ui->weekBox->checkState());
}

void DayPage::updateWeekEnd()
{
    _ui->satBox->setChecked(_ui->weekEndBox->checkState());
    _ui->sunBox->setChecked(_ui->weekEndBox->checkState());
}

void DayPage::updateMon()
{
    if ( !_ui->monBox->checkState() )
        _ui->weekBox->setChecked(false);
}

void DayPage::updateTue()
{
    if ( !_ui->tueBox->checkState() )
        _ui->weekBox->setChecked(false);
}

void DayPage::updateWed()
{
    if ( !_ui->wedBox->checkState() )
        _ui->weekBox->setChecked(false);
}

void DayPage::updateThu()
{
    if ( !_ui->thuBox->checkState() )
        _ui->weekBox->setChecked(false);
}

void DayPage::updateFri()
{
    if ( !_ui->friBox->checkState() )
        _ui->weekBox->setChecked(false);
}

void DayPage::updateSat()
{
    if ( !_ui->satBox->checkState() )
        _ui->weekEndBox->setChecked(false);
}

void DayPage::updateSun()
{
    if ( !_ui->sunBox->checkState() )
        _ui->weekEndBox->setChecked(false);
}
