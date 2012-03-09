#ifndef MUSICCONTROL_H
#define MUSICCONTROL_H

#include <QtCore/QDebug>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QProgressBar>
#include <QtCore/QTimer>

#include <alsa/asoundlib.h>

#define INPUT_MIC   0
#define INPUT_LINE1 1
#define INPUT_HP    2
#define INPUT_LINE2 3

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

        int alsa_open(snd_pcm_t **pcm_handle, int stream_type);
        void alsa_close();
        void alsa_select_input(int input);
        void play(snd_pcm_t* source);
        void stop();
        static void *playback_run(void *params);

    private:
        snd_mixer_t *handle;
        snd_mixer_selem_id_t *sid;
        snd_mixer_elem_t* elem;
        long min, max;

        VolumeWidget* vol;

        snd_pcm_t* capture;
        snd_pcm_t *playback;
    };
}

#endif // MUSICCONTROL_H
