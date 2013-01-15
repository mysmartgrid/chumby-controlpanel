#include "volumepage.h"
#include "ui_volumepage.h"

#include "musiccontrol.h"

VolumePage::VolumePage(Msg::Alarm *alarm, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::VolumePage),
    _alarm(alarm),
    _sysVolume(Msg::MusicControl::getInstance().getMasterVolume()),
    _sysMute(Msg::MusicControl::getInstance().getMasterMute())
{
    _ui->setupUi(this);

    _ui->volumeSlider->setValue(_alarm->getVolume());

    connect(_ui->backButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect(_ui->setButton, SIGNAL(clicked()), this, SLOT(setVolume()));
    connect(_ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(playTestTone(int)));

    Msg::MusicControl::getInstance().setMasterVolume(_alarm->getVolume());
    Msg::MusicControl::getInstance().setMasterMute(1);

    QString source = _alarm->getSource();
    int index = source.indexOf("/");
    QString audioPlugin = source.left(index);
    QString pluginSource = source;
    pluginSource.remove(0, index + 1);
    qDebug() << "Running" << audioPlugin << "with source" << pluginSource;
    if ( audioPlugin.length() == 0 || pluginSource.length() == 0 )
        return;

    Msg::AudioPlugin *plugin = Msg::MusicControl::getInstance().getAudioPlugin(audioPlugin);
    if ( ! plugin )
        return;

    qDebug() << "Plugin for alarm loaded";
    plugin->init();

    if ( ! plugin->play(pluginSource) )
        return;
}

VolumePage::~VolumePage()
{
    Msg::MusicControl::getInstance().stop();
    Msg::MusicControl::getInstance().setMasterVolume(_sysVolume);
    Msg::MusicControl::getInstance().setMasterMute(_sysMute);
    delete _ui;
}

void VolumePage::setVolume()
{
    _alarm->setVolume(_ui->volumeSlider->value());
    deleteLater();
}

void VolumePage::playTestTone(int value)
{
    //TODO: should change the volume and play a simple test tone to show the current volume
    Msg::MusicControl::getInstance().setMasterVolume(value);
}
