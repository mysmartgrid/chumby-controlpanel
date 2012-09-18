#include "controlpanel.h"

#include "ui_loadingWidget.h"

#include <QtCore/QDir>
#include <QtCore/QDirIterator>

#include <QtCore/QTimer>
#include <QtCore/QTime>

#include <QDebug>

#include <iwlib.h>

namespace Msg
{
Controlpanel::Controlpanel(QWidget *parent)
  : QWidget(parent),
    _ui(new Ui::controlWidget),
    _currentPlugin(NULL),
    _pluginWidget(NULL),
    _sock_iwconfig(::iw_sockets_open()),
    _mc(&MusicControl::getInstance()),
    _alarm(&AlarmDaemon::getInstance())
{
    _ui->setupUi(this);

    _plugins = QMap< QString, QPair<QIcon*, DLLFactory<PluginFactory>*> >();
    grabKeyboard();
    getPlugins();
	QTimer* timer = new QTimer( this );
    updateClock();
	connect( timer, SIGNAL( timeout() ), this, SLOT( updateClock() ) );
	timer->start( 1000 );
    QListWidget* list = _ui->pluginsWidget;
	list->setGridSize(QSize(92, 65));
	list->setViewMode(QListWidget::IconMode);
	list->setMovement(QListWidget::Static);
    for ( QMap<QString, QPair<QIcon*, DLLFactory<PluginFactory>*> >::iterator i = _plugins.begin(); i != _plugins.end(); i++ )
	{
		list->addItem(new QListWidgetItem(*(i.value().first), i.key()));
	}
	connect(list, SIGNAL( clicked(QModelIndex) ), this, SLOT( startPlugin() ));

    connect(this, SIGNAL(keyPressed()), _alarm, SIGNAL(snooze()));
}

Controlpanel::~Controlpanel()
{
	this->releaseKeyboard();
}

void Controlpanel::startPlugin()
{
	QWidget* loadingWidget = new QWidget();
	Ui::loadingWidget lui;
	lui.setupUi(loadingWidget);
    QString key = _ui->pluginsWidget->selectedItems().first()->text();
	lui.loadingLabel->setText("Loading " + key);
	loadingWidget->showFullScreen();
	qApp->processEvents();
    if ( _currentPlugin != NULL && _currentPlugin->getName().compare(key.toStdString()) != 0 )
	{
        qDebug() << QString::fromStdString(_currentPlugin->getName()) << " != " << key;
        delete _pluginWidget;
        delete _currentPlugin;
        _currentPlugin = NULL;
        _pluginWidget = NULL;
	}
    if ( _currentPlugin == NULL )
	{
        _currentPlugin = _plugins.value(key).second->factory->CreatePlugin();
        _currentPlugin->init();
        _pluginWidget = NULL;
        qDebug() << "Plugin version:" << _currentPlugin->getVersion();
	}
	showWidget();
    qDebug() << QString::fromStdString(_currentPlugin->getName()) << " == " << key;
}

void Controlpanel::showWidget()
{
    if ( _pluginWidget == NULL )
	{
        _pluginWidget = _currentPlugin->getWidget();
        connect( _currentPlugin, SIGNAL( stopWidget() ), this, SLOT( stopPlugin() ) );
	}
    _pluginWidget->raise();
    _pluginWidget->showFullScreen();
	this->hide();
}

void Controlpanel::stopPlugin()
{
    if ( _pluginWidget )
        _pluginWidget->close();
	this->show();
    this->raise();
}

void Controlpanel::getPlugins()
{
	QDirIterator directory_walker("/mnt/usb/chumby-controlpanel/plugins/", QDir::Files);

	while ( directory_walker.hasNext() )
	{
		directory_walker.next();

		std::cout << directory_walker.fileName().toStdString() << std::endl;

		if ( !directory_walker.fileInfo().isHidden() )
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

                _plugins.insert(QString::fromStdString(c->getName()), QPair< QIcon*, DLLFactory<PluginFactory>* >(c->getIcon(), dll));

				if ( c->getType() == AUDIO_PLUGIN )
					MusicControl::getInstance().addAudioPlugin(QString::fromStdString(c->getName()), dll);
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
        _alarm->check();
	}
    _ui->timeLabel->setText( text );


	iwstats stats;
	QIcon wicon;
    if (iw_get_stats(_sock_iwconfig, "wlan0", &stats, NULL, 0) >= 0 )
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
    _ui->wifiLabel->setPixmap( wicon.pixmap(15) );
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
#ifdef Q_WS_QWS
	case 28:
#else // Q_WS_QWS
    case 65: // "a"
#endif // Q_WS_QWS
		if ( AlarmDaemon::getInstance().isAlarmActive() )
		{
			qDebug() << "snooze";
			emit keyPressed();
		}
		else
			this->stopPlugin();
		break;
#ifdef Q_WS_QWS
	case 8:
#else // Q_WS_QWS
    case 43: // "+"
#endif // Q_WS_QWS
		// volume
		std::cout << "adjusting volume" << std::endl;
        _mc->increaseMasterVolume();
		break;
#ifdef Q_WS_QWS
	case 9:
#else // Q_WS_QWS
    case 45: // "-"
#endif // Q_WS_QWS
        _mc->lowerMasterVolume();
		break;
	}
}

InitThread::InitThread(Plugin *plugin)
{
    _plugin = plugin;
}

void InitThread::run()
{
        _plugin->init();
		emit ready();
}
}
