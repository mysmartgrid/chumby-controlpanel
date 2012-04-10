#include "controlpanel.h"

#include <QtCore/QDir>
#include <QtCore/QDirIterator>

#include <QtCore/QTimer>
#include <QtCore/QTime>

#include <QDebug>

#include <iwlib.h>

namespace Msg
{
    Controlpanel::Controlpanel(QWidget *parent)
        : QDialog(parent)
    {
        this->grabKeyboard();
        plugins = QMap< QString, QPair<QIcon*, DLLFactory<PluginFactory>*> >();
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
        list->setGridSize(QSize(92, 65));
        list->setViewMode(QListWidget::IconMode);
        list->setMovement(QListWidget::Static);
        for ( QMap<QString, QPair<QIcon*, DLLFactory<PluginFactory>*> >::iterator i = plugins.begin(); i != plugins.end(); i++ )
        {
            list->addItem(new QListWidgetItem(*(i.value().first), i.key()));
        }
        connect(list, SIGNAL( clicked(QModelIndex) ), this, SLOT( startPlugin() ));

        sock_iwconfig = ::iw_sockets_open();
        //mc = MusicControl();

        currentPlugin = NULL;

        mc = &MusicControl::getInstance();
        alarm = &AlarmDaemon::getInstance();
    }

    Controlpanel::~Controlpanel()
    {
        this->releaseKeyboard();
    }

    void Controlpanel::startPlugin()
    {
        QString key = list->selectedItems().first()->text();
        if ( currentPlugin != NULL && currentPlugin->getName().compare(key.toStdString()) != 0 )
        {
            qDebug() << QString::fromStdString(currentPlugin->getName()) << " != " << key;
            delete currentPlugin;
            currentPlugin = NULL;
        }
        if ( currentPlugin == NULL )
        {
            currentPlugin = plugins.value(key).second->factory->CreatePlugin();
            currentPlugin->init();
        }
        qDebug() << QString::fromStdString(currentPlugin->getName()) << " == " << key;
        QWidget* test = currentPlugin->getWidget();
        connect( currentPlugin, SIGNAL( stopWidget() ), this, SLOT( stopPlugin() ) );
        test->raise();
        test->showFullScreen();
        test->show();
        this->hide();
    }

    void Controlpanel::stopPlugin()
    {
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

                    plugins.insert(QString::fromStdString(c->getName()), QPair< QIcon*, DLLFactory<PluginFactory>* >(c->getIcon(), dll));
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
        if (time.second() == 0)
        {
            qDebug() << "Checking alarms!";
            alarm->check();
        }
        clock->setText( text );


        iwstats stats;
        QIcon wicon;
        if (iw_get_stats(sock_iwconfig, "wlan0", &stats, NULL, 0) >= 0 )
        {
            unsigned int quality = stats.qual.qual;
            qDebug() << "Stats: " << quality;
            if ( quality >= 60 )
                wicon = QIcon(":/icon/green/wifi-high");
            else if ( quality >= 40 )
                wicon = QIcon(":/icon/green/wifi-medium");
            else
                wicon = QIcon(":/icon/green/wifi-low");
        } else {
            //qDebug() << "retrieving iw stats failed!";
            wicon = QIcon(":/icon/green/wifi-disconnected");
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
                mc->increaseMasterVolume();
                break;
            case 9:
                mc->lowerMasterVolume();
                break;
        }
    }
}
