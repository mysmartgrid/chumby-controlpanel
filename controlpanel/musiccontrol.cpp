#include "musiccontrol.h"

#include <QtGui/QDesktopWidget>

namespace Msg
{
    MusicControl* MusicControl::instance = 0;
//    snd_pcm_t* MusicControl::playback = 0;
//    snd_pcm_t* MusicControl::capture = 0;
//    pthread_t MusicControl::playback_thread = 0;
//    volatile int MusicControl::playback_thread_running = 0;

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

        if ( playback == NULL )
            MusicControl::alsa_open(&playback, SND_PCM_STREAM_PLAYBACK);

        if ( playback == NULL )
            qDebug() << "MC says: playback is null";

        thread = 0;
    }

    MusicControl::~MusicControl()
    {
        snd_mixer_close(handle);
    }

    MusicControl& MusicControl::getInstance()
    {
        if ( !instance )
            instance = new MusicControl();

        return *instance;
    }

    void MusicControl::destroy()
    {
        if ( instance )
            delete instance;
        instance = 0;
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

    void MusicControl::play(snd_pcm_t *source) {
        qDebug() << "Starting playback thread!";
        capture = source;
        /*dev_info_t dev;
        dev.source = source;
        dev.dest = playback;*/
        if ( playback == NULL )
        {
            qDebug() << "ERROR: playback null";
        }
        if ( capture == NULL )
        {
            qDebug() << "ERROR: capture null";
        }
        if ( thread == NULL )
            thread = new PlaybackThread(capture, playback);

        setMasterMute(1);
        qDebug() << "Calling pthread";
        thread->start();
//        MusicControl::playback_thread_running = 1;
//        pthread_create(&MusicControl::playback_thread, NULL, &playback_run, NULL);
    }

    void MusicControl::stop() {
        thread->stop();
//        MusicControl::playback_thread_running = 0;
        setMasterMute(0);
        thread->wait();
        delete thread;
        thread = 0;
    }

    int MusicControl::alsa_open(snd_pcm_t **pcm_handle, int stream_type) {

        /* This structure contains information about    */
        /* the hardware and can be used to specify the  */
        /* configuration to be used for the PCM stream. */
        snd_pcm_hw_params_t *hwparams;

        /* Name of the PCM device, like plughw:0,0          */
        /* The first number is the number of the soundcard, */
        /* the second number is the number of the device.   */
        char *pcm_name;

        /* Init pcm_name. Of course, later you */
        /* will make this configurable ;-)     */
        pcm_name = strdup("chumix");


        /* Allocate the snd_pcm_hw_params_t structure on the stack. */
        snd_pcm_hw_params_alloca(&hwparams);

        /* Open PCM. The last parameter of this function is the mode. */
        /* If this is set to 0, the standard mode is used. Possible   */
        /* other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.       */
        /* If SND_PCM_NONBLOCK is used, read / write access to the    */
        /* PCM device will return immediately. If SND_PCM_ASYNC is    */
        /* specified, SIGIO will be emitted whenever a period has     */
        /* been completely processed by the soundcard.                */
        if (snd_pcm_open(pcm_handle, pcm_name, (snd_pcm_stream_t) stream_type, 0) < 0) {
            fprintf(stderr, "Error opening PCM device %s\n", pcm_name);
            return(-1);
        }

        if ( pcm_handle == NULL )
            fprintf(stderr, "pcm_handle is null (device opened)\n");

        /* Init hwparams with full configuration space */
        if (snd_pcm_hw_params_any(*pcm_handle, hwparams) < 0) {
            fprintf(stderr, "Can not configure this PCM device.\n");
            return(-1);
        }

        if ( pcm_handle == NULL )
            fprintf(stderr, "pcm_handle is null (hwparams allocated)\n");

        unsigned int rate = 44100; /* Sample rate */
        unsigned int exact_rate;   /* Sample rate returned by */
        int periods = 8;       /* Number of periods */

        /* Set access type. This can be either    */
        /* SND_PCM_ACCESS_RW_INTERLEAVED or       */
        /* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
        /* There are also access types for MMAPed */
        /* access, but this is beyond the scope   */
        /* of this introduction.                  */
        if (snd_pcm_hw_params_set_access(*pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
            fprintf(stderr, "Error setting access.\n");
            return(-1);
        }


        if ( pcm_handle == NULL )
            fprintf(stderr, "pcm_handle is null (access set)\n");

        /* Set sample format */
        if (snd_pcm_hw_params_set_format(*pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0) {
            fprintf(stderr, "Error setting format.\n");
            return(-1);
        }


        if ( pcm_handle == NULL )
            fprintf(stderr, "pcm_handle is null (format set)\n");

        /* Set sample rate. If the exact rate is not supported */
        /* by the hardware, use nearest possible rate.         */
        exact_rate = rate;
        if (snd_pcm_hw_params_set_rate_near(*pcm_handle, hwparams, &exact_rate, 0) < 0) {
            fprintf(stderr, "Error setting rate.\n");
            return(-1);
        }
        if (rate != exact_rate) {
            fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n \
                ==> Using %d Hz instead.\n", rate, exact_rate);
        }

        /* Set number of channels */
        if (snd_pcm_hw_params_set_channels(*pcm_handle, hwparams, 2) < 0) {
            fprintf(stderr, "Error setting channels.\n");
            return(-1);
        }

        /* Set number of periods. Periods used to be called fragments. */
        if (snd_pcm_hw_params_set_periods(*pcm_handle, hwparams, periods, 0) < 0) {
            fprintf(stderr, "Error setting periods.\n");
            return(-1);
        }

        /* Set buffer size (in frames). The resulting latency is given by */
        /* latency = periodsize * periods / (rate * bytes_per_frame)     */
        if (snd_pcm_hw_params_set_buffer_size(*pcm_handle, hwparams, 16384) < 0) {
            fprintf(stderr, "Error setting buffersize.\n");
            return(-1);
        }


        if ( pcm_handle == NULL )
            fprintf(stderr, "pcm_handle is null (hwparams prepared)\n");

        /* Apply HW parameter settings to */
        /* PCM device and prepare device  */
        if (snd_pcm_hw_params(*pcm_handle, hwparams) < 0) {
            fprintf(stderr, "Error setting HW params.\n");
            return(-1);
        }


        if ( pcm_handle == NULL )
            fprintf(stderr, "pcm_handle is null (hwparams applied)\n");

        int status = snd_pcm_prepare(*pcm_handle);
            fprintf(stdout, "Status: %i\n", status);
        if(status<0) {
            fprintf(stderr, "Unable to prepare audio device: %s\n",
                    snd_strerror(status));
            return(-1);
        }


        if ( pcm_handle == NULL )
            fprintf(stderr, "pcm_handle is null (end of alsa_open)\n");

        //return pcm_handle;
        return 1;
    }

    void MusicControl::alsa_close() {
        if ( capture )
            snd_pcm_close(capture);
    }

    void MusicControl::alsa_select_input(int input) {
        qDebug() << "Setting input" << input;
        const char* card = "default";
        const char* selem_left_name = "Left ADC Mux";
        const char* selem_right_name = "Right ADC Mux";
        snd_mixer_t* mHandle;
        snd_mixer_elem_t *mElem;
        snd_mixer_selem_id_t *adc;

        ::snd_mixer_open(&mHandle, 0);
        ::snd_mixer_attach(mHandle, card);
        ::snd_mixer_selem_register(mHandle, NULL, NULL);
        ::snd_mixer_load(mHandle);

        snd_mixer_selem_id_alloca(&adc);
        snd_mixer_selem_id_set_index(adc, 0);
        snd_mixer_selem_id_set_name(adc, selem_left_name);
        mElem = snd_mixer_find_selem(mHandle, adc);

        int res;
        res = snd_mixer_selem_set_enum_item(mElem, SND_MIXER_SCHN_MONO, input);
        if ( res < 0 )
        {
            qDebug() << "Setting left input failed:" << snd_strerror(res) << "(" << res << ")";
        }

        snd_mixer_selem_id_set_name(adc, selem_right_name);
        mElem = snd_mixer_find_selem(mHandle, adc);

        res = snd_mixer_selem_set_enum_item(mElem, SND_MIXER_SCHN_MONO, input);
        if ( res < 0 )
        {
            qDebug() << "Setting right input failed:" << snd_strerror(res) << "(" << res << ")";
        }

    }

    PlaybackThread::PlaybackThread(snd_pcm_t *in, snd_pcm_t *out)
    {
        this->capture = in;
        this->playback = out;
    }

    void PlaybackThread::stop()
    {
        this->thread_running = false;
    }

    void PlaybackThread::run() {
        this->thread_running = true;
        qDebug() << "I'm a playback thread!";
        if ( playback == NULL )
        {
            qDebug() << "ERROR/pthread: playback null";
        }
        if ( capture == NULL )
        {
            qDebug() << "ERROR/pthread: capture null";
        }
        qDebug() << "Starting playback!";
        snd_pcm_uframes_t data[4096];
        while (thread_running) {
            int bytes_read = snd_pcm_readi(capture, data, sizeof(data)/sizeof(data[0]));
            // If we didn't get enough data, wait, then try again.
            if(bytes_read == -EAGAIN) {
                fprintf(stderr, "Read error, trying again: %s\n",
                        snd_strerror(bytes_read));
                continue;
            }

            else if(bytes_read == -EPIPE) {
                fprintf(stderr, "Read error: (Over/Under)run\n");
                if(snd_pcm_prepare(capture) < 0) {
                    this->thread_running = false;
                    break;
                }
                continue;
            }

            // Or if we had a different error, bail.
            else if(bytes_read < 0) {
                fprintf(stderr, "Read error: %s\n", snd_strerror(bytes_read));
                this->thread_running = false;
                break;
            }

            int bytes_written = snd_pcm_writei(playback, data, bytes_read);

            if(bytes_written == -EPIPE) {
                fprintf(stderr, "Write error: (Over/Under)run\n");
                if(snd_pcm_prepare(playback) < 0) {
                    this->thread_running = false;
                    break;
                }
                continue;
            }

            else if(bytes_written <= 0) {
                fprintf(stderr, "Write error (%d): %s\n",
                        bytes_written, snd_strerror(bytes_written));
                this->thread_running = false;
                break;
            }
        }
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
