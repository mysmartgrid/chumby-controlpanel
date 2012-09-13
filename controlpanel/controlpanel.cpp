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
  : QWidget(parent)
	, currentPlugin(0)
	, pluginWidget(0)
	, _mc(0)
	, _alarm(0)
{
	ui.setupUi(this);
	this->grabKeyboard();
	plugins = QMap< QString, QPair<QIcon*, DLLFactory<PluginFactory>*> >();
	this->getPlugins();
	QTimer* timer = new QTimer( this );
	this->updateClock();
	connect( timer, SIGNAL( timeout() ), this, SLOT( updateClock() ) );
	timer->start( 1000 );
	QListWidget* list = ui.pluginsWidget;
	list->setGridSize(QSize(92, 65));
	list->setViewMode(QListWidget::IconMode);
	list->setMovement(QListWidget::Static);
	for ( QMap<QString, QPair<QIcon*, DLLFactory<PluginFactory>*> >::iterator i = plugins.begin(); i != plugins.end(); i++ )
	{
		list->addItem(new QListWidgetItem(*(i.value().first), i.key()));
	}
	connect(list, SIGNAL( clicked(QModelIndex) ), this, SLOT( startPlugin() ));

	sock_iwconfig = ::iw_sockets_open();
	//_mc = MusicControl();

	currentPlugin = NULL;

#ifdef Q_WS_QWS
	_mc = &MusicControl::getInstance();
#endif /* Q_WS_QWS */

	_alarm = &AlarmDaemon::getInstance();
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
	QString key = ui.pluginsWidget->selectedItems().first()->text();
	lui.loadingLabel->setText("Loading " + key);
	loadingWidget->showFullScreen();
	qApp->processEvents();
	if ( currentPlugin != NULL && currentPlugin->getName().compare(key.toStdString()) != 0 )
	{
		qDebug() << QString::fromStdString(currentPlugin->getName()) << " != " << key;
		delete pluginWidget;
		delete currentPlugin;
		currentPlugin = NULL;
		pluginWidget = NULL;
	}
	if ( currentPlugin == NULL )
	{
		currentPlugin = plugins.value(key).second->factory->CreatePlugin();
		currentPlugin->init();
		pluginWidget = NULL;
		qDebug() << "Plugin version:" << currentPlugin->getVersion();
	}
	showWidget();
	qDebug() << QString::fromStdString(currentPlugin->getName()) << " == " << key;
}

void Controlpanel::showWidget()
{
	if ( pluginWidget == NULL )
	{
		pluginWidget = currentPlugin->getWidget();
		connect( currentPlugin, SIGNAL( stopWidget() ), this, SLOT( stopPlugin() ) );
	}
	pluginWidget->raise();
	pluginWidget->showFullScreen();
	this->hide();
}

void Controlpanel::stopPlugin()
{
	if( pluginWidget != NULL) {
		printf(">>pluginWidget not NULL\n");
		pluginWidget->close();
	}
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
	ui.timeLabel->setText( text );


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
	ui.wifiLabel->setPixmap( wicon.pixmap(15) );
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
#else /* Q_WS_QWS */
	case 65:
#endif /* Q_WS_QWS */
		if ( AlarmDaemon::getInstance().isAlarmActive() )
		{
			qDebug() << "snooze";
			emit keyPressed();
		}
		else
			this->stopPlugin();
		break;
	case 8:
		// volume
		std::cout << "adjusting volume" << std::endl;
		_mc->increaseMasterVolume();
		break;
	case 9:
		_mc->lowerMasterVolume();
		break;
	}
}

InitThread::InitThread(Plugin *plugin)
{
	this->plugin = plugin;
}

void InitThread::run()
{
		plugin->init();
		emit ready();
}
}
