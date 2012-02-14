#ifndef MUSICCONTROL_H
#define MUSICCONTROL_H

#include <QtCore/QDebug>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QProgressBar>
#include <QtCore/QTimer>

#include <alsa/asoundlib.h>

namespace Msg
{
    class VolumeWidget : public QWidget
    {
    public:
        VolumeWidget(int min, int max);
        void setVolume(long int vol);
        void showWidget();

    private:
        long int volume;
        QHBoxLayout* layout;
        QProgressBar* bar;
        QTimer* timer;
    };

    class MusicControl
    {
    public:
        MusicControl();
        ~MusicControl();
        void setMasterVolume(long int volume);
        long int getMasterVolume();
        void setMasterMute(int value);
        void increaseMasterVolume();
        void lowerMasterVolume();

    private:
        snd_mixer_t *handle;
        snd_mixer_selem_id_t *sid;
        snd_mixer_elem_t* elem;
        long min, max;

        VolumeWidget* vol;
    };
}

#endif // MUSICCONTROL_H
