#include "volumepage.h"
#include "ui_volumepage.h"

#include "musiccontrol.h"

VolumePage::VolumePage(Msg::Alarm *alarm, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::VolumePage),
    _alarm(alarm),
    _sysVolume(Msg::MusicControl::getInstance().getMasterVolume())
{
    _ui->setupUi(this);

    _ui->volumeSlider->setValue(_alarm->getVolume());

    connect(_ui->backButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect(_ui->setButton, SIGNAL(clicked()), this, SLOT(setVolume()));
    connect(_ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(playTestTone(int)));
}

VolumePage::~VolumePage()
{
    Msg::MusicControl::getInstance().setMasterVolume(_sysVolume);
    delete _ui;
}

void VolumePage::setVolume()
{
    _alarm->setVolume(_ui->volumeSlider->value());
    deleteLater();
}

void VolumePage::playTestTone(int)
{
    //TODO: should change the volume and play a simple test tone to show the current volume
}
