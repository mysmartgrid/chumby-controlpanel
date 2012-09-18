#include "musiccontrol.h"

#include <QtGui/QDesktopWidget>

namespace Msg
{
    MusicControl* MusicControl::_instance = 0;
//    snd_pcm_t* MusicControl::playback = 0;
//    snd_pcm_t* MusicControl::capture = 0;
//    pthread_t MusicControl::playback_thread = 0;
//    volatile int MusicControl::playback_thread_running = 0;

    MusicControl::MusicControl()
        :_audioPlugins(QMap< QString, DLLFactory<PluginFactory>* >()),
          _handle(NULL),
          _sid(NULL),
          _elem(NULL),
          _min(0),
          _max(0),
          _vol(NULL),
          _capture(NULL),
          _playback(NULL),
          _thread(NULL)
    {
        qDebug() << "Initializing music control!";
        const char *card = "default";
        char* cname = getenv("CHUMBY_SOUND_CARD");
        if ( !cname )
            cname = "DAC";
        const char *selem_name = cname;

        ::snd_mixer_open(&_handle, 0);
        ::snd_mixer_attach(_handle, card);
        ::snd_mixer_selem_register(_handle, NULL, NULL);
        ::snd_mixer_load(_handle);

        snd_mixer_selem_id_alloca(&_sid);
        snd_mixer_selem_id_set_index(_sid, 0);
        snd_mixer_selem_id_set_name(_sid, selem_name);
        _elem = snd_mixer_find_selem(_handle, _sid);

        if ( !_elem )
        {
            qDebug() << "Sound device \"" << selem_name << "\" not found. Disabling MusicControl.";
            return;
        }

        snd_mixer_selem_get_playback_volume_range(_elem, &_min, &_max);
        if ( _min < 128 )
            _min = 128;

        _vol = new VolumeWidget(_min, _max);
        QPalette p(_vol->palette());
        p.setColor(QPalette::Background, Qt::darkGray);
        _vol->setPalette(p);
        _vol->setVolume(getMasterVolume());
        _vol->move(60, 180);

        //if ( playback == NULL )
        MusicControl::alsa_open(&_playback, SND_PCM_STREAM_PLAYBACK);

        //if ( playback == NULL )
        //    qDebug() << "MC says: playback is null";
    }

    MusicControl::~MusicControl()
    {
        snd_mixer_close(_handle);
    }

    MusicControl& MusicControl::getInstance()
    {
        if ( !_instance )
            _instance = new MusicControl();

        return *_instance;
    }

    void MusicControl::destroy()
    {
        if ( _instance )
            delete _instance;
        _instance = 0;
    }

    void MusicControl::setMasterVolume(long int volume)
    {
        if ( !_elem )
            return;

        setMasterMute(1);

        snd_mixer_selem_set_playback_volume_all(_elem, volume * _max / 100);

        _vol->setVolume(getMasterVolume());
        _vol->showWidget();
    }

    long int MusicControl::getMasterVolume()
    {
        if ( !_elem )
            return;

        long int volume;
        snd_mixer_selem_get_playback_volume(_elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);

        return volume;
    }

    void MusicControl::increaseMasterVolume()
    {
        if ( !_elem )
            return;

        setMasterMute(1);

        long int volume = getMasterVolume();
        qDebug() << _min << " <= " << volume << " <= " << _max;
                if ( volume < _min )
				{
                    volume = _min;
                    snd_mixer_selem_set_playback_volume_all(_elem, volume);
                    snd_mixer_selem_set_playback_switch_all(_elem, 1);
				}
        if ( volume <= _max - 3 )
        {
            volume += 3;
            snd_mixer_selem_set_playback_volume_all(_elem, volume);
        }
				else
				{
                    volume = _max;
          snd_mixer_selem_set_playback_volume_all(_elem, volume);
				}

        _vol->setVolume(volume);
        _vol->showWidget();
    }

    void MusicControl::lowerMasterVolume()
    {
        if ( !_elem )
            return;

        setMasterMute(1);
        long int volume = getMasterVolume();
        qDebug() << _min << " <= " << volume << " <= " << _max;
        if ( volume >= _min + 3 )
        {
            volume -= 3;
            snd_mixer_selem_set_playback_volume_all(_elem, volume);
        }
				else
				{
					volume = 0;
                    snd_mixer_selem_set_playback_volume_all(_elem, volume);
                    snd_mixer_selem_set_playback_switch_all(_elem, 0);
				}

        _vol->setVolume(volume);
        _vol->showWidget();
    }

    //set mute flag ( 0: muted, 1: not muted )
    void MusicControl::setMasterMute(int value)
    {
        if ( !_elem )
            return;

        snd_mixer_selem_set_playback_switch_all(_elem, value);
    }

    void MusicControl::play(snd_pcm_t *source) {
        qDebug() << "Starting playback thread!";
        _capture = source;
        /*dev_info_t dev;
        dev.source = source;
        dev.dest = playback;*/
        if ( _playback == NULL )
        {
            qDebug() << "ERROR: playback null";
            return;
        }
        if ( _capture == NULL )
        {
            qDebug() << "ERROR: capture null";
            return;
        }
        if ( _thread == NULL )
            _thread = new PlaybackThread(_capture, _playback);

        setMasterMute(1);
        qDebug() << "Calling pthread";
        _thread->start();
//        MusicControl::playback_thread_running = 1;
//        pthread_create(&MusicControl::playback_thread, NULL, &playback_run, NULL);
    }

    void MusicControl::stop() {
        if ( !_thread )
        {
            qDebug() << "MusicControl: Nothing to stop here!";
            return;
        }

        _thread->stop();
//        MusicControl::playback_thread_running = 0;
        setMasterMute(0);
        _thread->wait();
        delete _thread;
        _thread = 0;
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
        if ( _capture )
            snd_pcm_close(_capture);
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

    void MusicControl::addAudioPlugin(QString name, DLLFactory<PluginFactory> *dll)
    {
        _audioPlugins.insert(name, dll);
    }

    QList<QString> MusicControl::getAudioPlugins()
    {
        return _audioPlugins.keys();
    }

    AudioPlugin* MusicControl::getAudioPlugin(QString plugin)
    {
        return (AudioPlugin*) _audioPlugins.find(plugin).value()->factory->CreatePlugin();
    }

    /*QList<QString> MusicControl::getAudioSources()
    {
        QList<QString> sources = QList<QString>();
        for ( QMap<QString, DLLFactory<PluginFactory>* >::iterator it = audioPlugins.begin(); it != audioPlugins.end(); it++ )
        {
            AudioPlugin* plugin = (AudioPlugin*) it.value()->factory->CreatePlugin();
            if ( plugin )
            {
                sources.append(plugin->getSources());
                delete plugin;
            }
        }
        return sources;
    }*/

    PlaybackThread::PlaybackThread(snd_pcm_t *in, snd_pcm_t *out)
        :_capture(in),
          _playback(out)
    {
    }

    void PlaybackThread::stop()
    {
        _thread_running = false;
    }

    void PlaybackThread::run() {
        _thread_running = true;
        qDebug() << "I'm a playback thread!";
        if ( _playback == NULL )
        {
            qDebug() << "ERROR/pthread: playback null";
            return;
        }
        if ( _capture == NULL )
        {
            qDebug() << "ERROR/pthread: capture null";
            return;
        }
        qDebug() << "Starting playback!";
        snd_pcm_uframes_t data[4096];
        while (_thread_running) {
            int bytes_read = snd_pcm_readi(_capture, data, sizeof(data)/sizeof(data[0]));
            // If we didn't get enough data, wait, then try again.
            if(bytes_read == -EAGAIN) {
                fprintf(stderr, "Read error, trying again: %s\n",
                        snd_strerror(bytes_read));
                continue;
            }

            else if(bytes_read == -EPIPE) {
                fprintf(stderr, "Read error: (Over/Under)run\n");
                if(snd_pcm_prepare(_capture) < 0) {
                    _thread_running = false;
                    break;
                }
                continue;
            }

            // Or if we had a different error, bail.
            else if(bytes_read < 0) {
                fprintf(stderr, "Read error: %s\n", snd_strerror(bytes_read));
                _thread_running = false;
                break;
            }

            int bytes_written = snd_pcm_writei(_playback, data, bytes_read);

            if(bytes_written == -EPIPE) {
                fprintf(stderr, "Write error: (Over/Under)run\n");
                if(snd_pcm_prepare(_playback) < 0) {
                    _thread_running = false;
                    break;
                }
                continue;
            }

            else if(bytes_written <= 0) {
                fprintf(stderr, "Write error (%d): %s\n",
                        bytes_written, snd_strerror(bytes_written));
                _thread_running = false;
                break;
            }
        }
    }

    VolumeWidget::VolumeWidget(int min = 0, int max = 100)
        :QWidget(NULL, Qt::ToolTip),
          _layout(new QHBoxLayout(this)),
          _bar(new QProgressBar()),
          _timer(new QTimer())
    {
        QLabel* low = new QLabel();
        low->setPixmap(QIcon(":/icon/resources/vol-down.png").pixmap(15));
        _layout->addWidget(low);
        qDebug() << "Building Progressbar";
        _bar->setMaximum(max);
        _bar->setMinimum(min);
        _layout->addWidget(_bar);
        QLabel* high = new QLabel();
        high->setPixmap(QIcon(":/icon/resources/vol-up.png").pixmap(15));
        _layout->addWidget(high);
        _timer->setInterval(1000);
        connect(_timer, SIGNAL(timeout()), this, SLOT(hide()));
    }

    void VolumeWidget::setVolume(long int vol)
    {
            this->_volume = vol;
            if ( vol < _bar->minimum() )
                _bar->setValue(_bar->minimum());
			else
                _bar->setValue(this->_volume);
    }

    void VolumeWidget::showWidget()
    {
        this->show();
        _timer->start();
    }
}
