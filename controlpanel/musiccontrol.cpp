#include "musiccontrol.h"

#include <QtGui/QDesktopWidget>

namespace Msg
{
    MusicControl::MusicControl()
    {
        qDebug() << "Initializing music control!";
        const char *card = "default";
        const char *selem_name = "DAC";

        ::snd_mixer_open(&handle, 0);
        ::snd_mixer_attach(handle, card);
        ::snd_mixer_selem_register(handle, NULL, NULL);
        ::snd_mixer_load(handle);

        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, selem_name);
        elem = snd_mixer_find_selem(handle, sid);

        snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

        vol = new VolumeWidget(min, max);
        QPalette p(vol->palette());
        p.setColor(QPalette::Background, Qt::darkGray);
        vol->setPalette(p);
        vol->setVolume(getMasterVolume());
        vol->move(60, 180);
    }

    MusicControl::~MusicControl()
    {
        snd_mixer_close(handle);
    }

    void MusicControl::setMasterVolume(long int volume)
    {
        setMasterMute(1);

        snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

        vol->setVolume(getMasterVolume());
        vol->showWidget();
    }

    long int MusicControl::getMasterVolume()
    {
        long int volume;
        snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);

        return volume;
    }

    void MusicControl::increaseMasterVolume()
    {
        setMasterMute(1);

        long int volume = getMasterVolume();
        qDebug() << min << " <= " << volume << " <= " << max;
        if ( volume <= max - 3 )
        {
            volume += 3;
            snd_mixer_selem_set_playback_volume_all(elem, volume);
        }

        vol->setVolume(volume);
        vol->showWidget();
    }

    void MusicControl::lowerMasterVolume()
    {
        setMasterMute(1);
        long int volume = getMasterVolume();
        qDebug() << min << " <= " << volume << " <= " << max;
        if ( volume >= min + 3 )
        {
            volume -= 3;
            snd_mixer_selem_set_playback_volume_all(elem, volume);
        }

        vol->setVolume(volume);
        vol->showWidget();
    }

    //set mute flag ( 0: muted, 1: not muted )
    void MusicControl::setMasterMute(int value)
    {
        snd_mixer_selem_set_playback_switch_all(elem, value);
    }

    VolumeWidget::VolumeWidget(int min = 0, int max = 100)
        :QWidget(NULL, Qt::ToolTip)
    {
        layout = new QHBoxLayout(this);
        QLabel* low = new QLabel();
        low->setPixmap(QIcon(":/icon/resources/vol-down.png").pixmap(15));
        layout->addWidget(low);
        qDebug() << "Building Progressbar";
        bar = new QProgressBar();
        bar->setMaximum(max);
        bar->setMinimum(min);
        layout->addWidget(bar);
        QLabel* high = new QLabel();
        high->setPixmap(QIcon(":/icon/resources/vol-up.png").pixmap(15));
        layout->addWidget(high);
        timer = new QTimer();
        timer->setInterval(1000);
        connect(timer, SIGNAL(timeout()), this, SLOT(hide()));
    }

    void VolumeWidget::setVolume(long int vol)
    {
        this->volume = vol;
        bar->setValue(this->volume);
    }

    void VolumeWidget::showWidget()
    {
        this->show();
        timer->start();
    }
}
