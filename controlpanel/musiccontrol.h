#ifndef MUSICCONTROL_H
#define MUSICCONTROL_H

#include <QtCore/QDebug>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QProgressBar>
#include <QtCore/QTimer>
#include <QtCore/QThread>

#include <alsa/asoundlib.h>

#include "audioplugin.h"
#include "dllmanager.h"
#include "btwrapper.h"

#define INPUT_MIC   0
#define INPUT_LINE1 1
#define INPUT_HP    2
#define INPUT_LINE2 3

#define VOLUME_STEP 3

#define ALSA_ASIO

namespace Msg
{
    class VolumeWidget : public QWidget
    {
    public:
        VolumeWidget();
        void setVolume(long int vol);
        void showWidget();

    private:
        long int _volume;
        QHBoxLayout* _layout;
        QProgressBar* _bar;
        QTimer* _timer;
    };

    class PlaybackThread : public QThread
    {
        Q_OBJECT

        struct callback_data_t {
            snd_pcm_uframes_t* buffer;
            snd_pcm_t* out;
            int period_size;
        };

    public:
        PlaybackThread(snd_pcm_t* in, snd_pcm_t* out);
        PlaybackThread(QString source);

    public slots:
        void stop();

    protected:
        void run();
        void playAlsa(snd_pcm_uframes_t *data, int period_size = 1024);
#ifdef ALSA_ASIO
        static void alsaCallback(snd_async_handler_t *pcm_callback);
#endif
        void playBlueTune();

    private:
        bool _thread_running;
        snd_pcm_t* _playback;
        snd_pcm_t* _capture;
        QString _source;
        BtWrapper* _wrapper;
#ifdef ALSA_ASIO
        snd_async_handler_t *_handler;
        callback_data_t *_c_data;
#endif
    };

    class MusicControl : public QObject
    {
        Q_OBJECT

    public:
        static MusicControl& getInstance();
        static void destroy();

        void setMasterVolume(long int volume);
        long int getMasterVolume();
        void setMasterMute(int value);
        void increaseMasterVolume();
        void lowerMasterVolume();
        long getMinMasterVolume();
        long getMaxMasterVolume();

        int alsa_open(snd_pcm_t **pcm_handle, int stream_type);
        void alsa_close();
        void alsa_select_input(int input);
        void play(snd_pcm_t* source);
        void play(QString source);
        void stop();
        void stopPlaybackThread();
        static void *playback_run(void *params);

        void addAudioPlugin(QString name, DLLFactory<PluginFactory>* dll);
        QList< QString > getAudioPlugins();
        AudioPlugin* getAudioPlugin(QString plugin);
        //QList< QMap< QString, QList< QString > > > getAudioSources();

    signals:
        void stopPlugins();

    private:
        MusicControl();
        MusicControl(const MusicControl&) {}
        ~MusicControl();
        static MusicControl* _instance;

        snd_mixer_t *_handle;
        snd_mixer_selem_id_t *_sid;
        snd_mixer_elem_t* _elem;
        long _min, _max;

        VolumeWidget* _vol;

        snd_pcm_t* _capture;
        snd_pcm_t *_playback;

        QMap< QString, DLLFactory<PluginFactory>* > _audioPlugins;

        PlaybackThread* _thread;
    };
}

#endif // MUSICCONTROL_H
