#include "controlpanel.h"

#include <QtCore/QDir>
#include <QtCore/QDirIterator>

#include <QtCore/QTimer>
#include <QtCore/QTime>

#include <QDebug>

#include <iwlib.h>
#include <alsa/asoundlib.h>


namespace Msg
{
    Controlpanel::Controlpanel(QWidget *parent)
        : QDialog(parent)
    {
        this->grabKeyboard();
        plugins = QMap< QString, QPair<DLLFactory<PluginFactory>*, Plugin*> >();
        this->getPlugins();
        layout = new QVBoxLayout( this );
        header = new QHBoxLayout();
        layout->addLayout( header );
        header->addWidget( new QLabel(), 10 );
        clock = new QLabel();
        header->addWidget( clock );
        wifi = new QLabel();
        QTimer* timer = new QTimer( this );
        this->updateClock();
        connect( timer, SIGNAL( timeout() ), this, SLOT( updateClock() ) );
        timer->start( 1000 );
        header->addWidget(wifi);
        list = new QListWidget();
        layout->addWidget( list );
        list->setGridSize(QSize(110, 65));
        list->setViewMode(QListWidget::IconMode);
        list->setMovement(QListWidget::Static);
        for ( QMap<QString, QPair<DLLFactory<PluginFactory>*, Plugin*> >::iterator i = plugins.begin(); i != plugins.end(); i++ )
        {
            list->addItem(new QListWidgetItem(*(i.value().second->getIcon()), i.key()));
        }
        connect(list, SIGNAL( clicked(QModelIndex) ), this, SLOT( startPlugin() ));

        sock_iwconfig = ::iw_sockets_open();
    }

    Controlpanel::~Controlpanel()
    {
        this->releaseKeyboard();
    }

    void Controlpanel::startPlugin()
    {
        QString key = list->selectedItems().first()->text();
        std::cout << plugins.value(key).second->init() << std::endl;
        //stack->addWidget(plugins.value(key).second->getWidget());
        QWidget* test = plugins.value(key).second->getWidget();
        connect( plugins.value(key).second, SIGNAL( stopWidget() ), this, SLOT( stopPlugin() ) );
        this->hide();
        test->showFullScreen();
        test->show();
    }

    void Controlpanel::stopPlugin()
    {
        std::cout << "Widget stopped" << std::endl;
        this->show();
    }

    void Controlpanel::getPlugins()
    {
        QDirIterator directory_walker("/mnt/usb/chumby-controlpanel/plugins/", QDir::Files);

        while ( directory_walker.hasNext() )
        {
            directory_walker.next();

            std::cout << directory_walker.fileName().toStdString() << std::endl;

            if ( directory_walker.fileInfo().completeSuffix() == "so" )
            {
                // Now create a DLLFactory for our PlugInFactory attached
                // to the requested file

                DLLFactory<PluginFactory>* dll = new DLLFactory<PluginFactory>( directory_walker.filePath().toAscii() );

                //
                // If it worked we should have dll.factory pointing
                // to a subclass of PlugInFactory
                //

                if( dll->factory )
                {
                    //
                    // yes, we have a factory. Ask the factory to create a
                    // PlugIn object for us.
                    //

                    Plugin *c=dll->factory->CreatePlugin();

                    // tell it to show itself

                    //c->Show();
                    std::cout << c->init() << std::endl;

                    plugins.insert(QString::fromStdString(c->init()), QPair<DLLFactory<PluginFactory>*, Plugin*>(dll, c));
                }
                else
                {
                    std::cout << "Error opening dll!" << std::endl;

                    // show the error message if any

                    if( dll->LastError() )
                    {
                        std::cout << "DLL Error: " << dll->LastError() << std::endl;
                    }

                }
            }
        }
    }

    void Controlpanel::updateClock()
    {
        QTime time = QTime::currentTime();
        QString text = time.toString("hh:mm");
        if ((time.second() % 2) == 0)
                 text[2] = ' ';
        clock->setText( text );


        iwstats stats;
        QIcon wicon;
        if (iw_get_stats(sock_iwconfig, "wlan0", &stats, NULL, 0) >= 0 )
        {
            unsigned int quality = stats.qual.qual;
            qDebug() << "Stats: " << quality;
            if ( quality >= 60 )
                wicon = QIcon(":/icon/resources/wifi-high.png");
            else if ( quality >= 40 )
                wicon = QIcon(":/icon/resources/wifi-medium.png");
            else
                wicon = QIcon(":/icon/resources/wifi-low.png");
        } else {
            qDebug() << "retrieving iw stats failed!";
            wicon = QIcon(":/icon/resources/wifi-disconnected.png");
        }
        wifi->setPixmap( wicon.pixmap(15) );
    }

    void Controlpanel::keyPressEvent(QKeyEvent *event)
    {
        qDebug() << "key pressed: "
                 << event->key()
                 << "("
                 << event->text()
                 << ")";
        switch ( event->key() )
        {
            case 28:
                this->stopPlugin();
                break;
            case 8:
                // volume
                std::cout << "adjusting volume" << std::endl;
                this->setMasterVolume(100);
                break;
        }
    }

    void Controlpanel::setMasterVolume(long int volume)
    {
        long min, max;
        snd_mixer_t *handle;
        snd_mixer_selem_id_t *sid;
        const char *card = "default";
        const char *selem_name = "DAC";

        ::snd_mixer_open(&handle, 0);
        ::snd_mixer_attach(handle, card);
        ::snd_mixer_selem_register(handle, NULL, NULL);
        ::snd_mixer_load(handle);

        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, selem_name);
        snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

        snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
        snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

        snd_mixer_close(handle);
    }
}
