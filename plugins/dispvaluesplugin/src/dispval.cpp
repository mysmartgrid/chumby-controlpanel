#include "dispval.h"
#include <QObject>


DisplayPage::DisplayPage ( QWidget* parent ) : QWidget ( parent )
{
	digitalClk = new QLCDNumber ();
	digitalClk->setSegmentStyle ( QLCDNumber::Filled );
	digitalClk->setFrameStyle ( QFrame::StyledPanel | QFrame::Plain );
	digitalClk->setMaximumSize ( 220, 32 );
	digitalClk->setMinimumSize ( 190, 30 );
	digitalClk->setDigitCount ( 8 );
	digitalClk->display ( "00:00:00" );

	QLabel* clkLabel = new QLabel ( "<span style='font-size:16pt;'>Time</span>" );
	clkLabel->setTextFormat ( Qt::RichText );
	clkLabel->setFrameStyle ( QFrame::StyledPanel | QFrame::Plain );

	QHBoxLayout* clkLayout = new QHBoxLayout();
	clkLayout->addWidget ( clkLabel );
	clkLayout->addWidget ( digitalClk );

	sensorval = new QLCDNumber ();
	sensorval->setSegmentStyle ( QLCDNumber::Filled );
	sensorval->setFrameStyle ( QFrame::StyledPanel | QFrame::Plain );
	sensorval->setMinimumSize ( 190, 75 );
	sensorval->setMaximumSize ( 200, 80 );

	QLabel* sensorLabel = new QLabel ( "<span style='font-size:16pt;'>Consumption</span><br><span style='font-size:12pt;'>Watt</span>" );
	sensorLabel->setTextFormat ( Qt::RichText );
	sensorLabel->setFrameStyle ( QFrame::StyledPanel | QFrame::Plain );

	QHBoxLayout* sensorLayout = new QHBoxLayout();
	sensorLayout->addWidget ( sensorLabel );
	sensorLayout->addWidget ( sensorval );

	QGridLayout* avgLayout = new QGridLayout();
	QLabel* avgLabel1 = new QLabel ( tr ( "avg.(1m)" ) );
	QLabel* avgLabel2 = new QLabel ( tr ( "avg.(15m)" ) );
	avgLabel1->setFrameStyle ( QFrame::StyledPanel | QFrame::Plain );
	avgLabel2->setFrameStyle ( QFrame::StyledPanel | QFrame::Plain );
	avgLabel1->setAlignment ( Qt::AlignRight );
	avgLabel2->setAlignment ( Qt::AlignRight );
	avgLabel1->setMaximumSize ( 150, 20 );
	avgLabel2->setMaximumSize ( 150, 20 );
	avgval1 = new QLCDNumber ();
	avgval2 = new QLCDNumber ();
	avgval1->display ( "---" );
	avgval2->display ( "---" );
	avgval1->setSegmentStyle ( QLCDNumber::Filled );
	avgval1->setFrameStyle ( QFrame::StyledPanel | QFrame::Plain );
	avgval2->setSegmentStyle ( QLCDNumber::Filled );
	avgval2->setFrameStyle ( QFrame::StyledPanel | QFrame::Plain );

	avgLayout->addWidget ( avgLabel1, 0, 0, 1, 1 );
	avgLayout->addWidget ( avgLabel2, 0, 1, 1, 1 );
	avgLayout->addWidget ( avgval1, 1, 0, 1, 1 );
	avgLayout->addWidget ( avgval2, 1, 1, 1, 1 );
	avgLayout->setSpacing ( 1 );

	QVBoxLayout* layout = new QVBoxLayout ( this );
	layout->addLayout ( clkLayout );
	layout->addLayout ( sensorLayout );
	layout->addLayout ( avgLayout );
	layout->setContentsMargins ( 0, 0, 0, 0 );
	layout->setSpacing ( 4 );

	valueiter = 0;
	lastval = 0;
}


URLPage::URLPage ( QWidget* parent ) : QWidget ( parent )
{
	//QSettings settings ( "flukso.conf", QSettings::IniFormat );	
	//QSettings settings ( "/mnt/usb/displayvalues/flukso.conf", QSettings::IniFormat );
	QSettings settings ( "/mnt/usb/flukso.conf", QSettings::IniFormat );

	QLabel* urlLabel = new QLabel ( tr ( "Flukso IP:Port" ) );
	//urlLineEdit = new QLineEdit ( "131.246.191.27:8080" );
	//urlLineEdit = new QLineEdit ( "192.168.130.21:8080" );
	urlLineEdit = new QLineEdit ( settings.value ( "ip" ).toString() + ":" + settings.value ( "port" ).toString() );

	QLabel* sensorLabel = new QLabel ( tr ( "Sensors:" ) );

	sensorLabel1 = new QLabel ( settings.value ( "sensor1" ).toString() );
	sensorLabel2 = new QLabel ( settings.value ( "sensor2" ).toString() );
	sensorLabel3 = new QLabel ( settings.value ( "sensor3" ).toString() );

	sen1 = new QPushButton ( "Enable" );
	sen2 = new QPushButton ( "Enable" );
	sen3 = new QPushButton ( "Enable" );

	sen1->setCheckable ( true );
	sen2->setCheckable ( true );
	sen3->setCheckable ( true );

	sen1->setMaximumWidth ( 54 );
	sen2->setMaximumWidth ( 54 );
	sen3->setMaximumWidth ( 54 );

	if ( settings.value ( "sen1ena" ).toInt() == 1 ) {
		sen1->setChecked ( true );
		sen1->setText ( "Disable" );
	}

	if ( settings.value ( "sen2ena" ).toInt() == 1 ) {
		sen2->setChecked ( true );
		sen2->setText ( "Disable" );
	}

	if ( settings.value ( "sen3ena" ).toInt() == 1 ) {
		sen3->setChecked ( true );
		sen3->setText ( "Disable" );
	}

	QHBoxLayout* topLayout = new QHBoxLayout;
	topLayout->addWidget ( urlLabel );
	topLayout->addWidget ( urlLineEdit );

	QHBoxLayout* topLayout_s1 = new QHBoxLayout;
	//topLayout_s1->addWidget ( sensorLineEdit1 );
	topLayout_s1->addWidget ( sensorLabel1 );
	topLayout_s1->addWidget ( sen1 );

	QHBoxLayout* topLayout_s2 = new QHBoxLayout;
	//topLayout_s2->addWidget ( sensorLineEdit2 );
	topLayout_s2->addWidget ( sensorLabel2 );
	topLayout_s2->addWidget ( sen2 );

	QHBoxLayout* topLayout_s3 = new QHBoxLayout;
	//topLayout_s3->addWidget ( sensorLineEdit3 );
	topLayout_s3->addWidget ( sensorLabel3 );
	topLayout_s3->addWidget ( sen3 );

	QVBoxLayout* topLayout2 = new QVBoxLayout;
	topLayout2->addWidget ( sensorLabel );
	topLayout2->addLayout ( topLayout_s1 );
	topLayout2->addLayout ( topLayout_s2 );
	topLayout2->addLayout ( topLayout_s3 );

	statusLabel = new QLabel ( tr ( "Status: ----" ) );
	debugLabel = new QLabel ( tr ( "Mapsize: ----" ) );

	QLabel* info = new QLabel ( tr ( "Settings / Debug " ) );
	QVBoxLayout* layout = new QVBoxLayout ( this );
	layout->addWidget ( info );
	layout->addStretch();
	layout->addLayout ( topLayout );
	layout->addLayout ( topLayout2 );
	//layout->addStretch();
	layout->addWidget ( statusLabel );
	layout->addWidget ( debugLabel );
	layout->addStretch();
	layout->setContentsMargins ( 0, 0, 0, 0 );
	layout->setSpacing ( 5 );

	connect ( sen1, SIGNAL ( toggled ( bool ) ), this, SLOT ( buttonToggled ( bool ) ) );
	connect ( sen2, SIGNAL ( toggled ( bool ) ), this, SLOT ( buttonToggled ( bool ) ) );
	connect ( sen3, SIGNAL ( toggled ( bool ) ), this, SLOT ( buttonToggled ( bool ) ) );
}

void URLPage::buttonToggled ( bool checked )
{
	QPushButton* button = ( QPushButton* ) sender();
	button->setText ( ( checked ) ? QString ( "Disable" ) : QString ( "Enable" ) );
}



PlotPage::PlotPage ( QWidget* parent ) : QWidget ( parent )
{
	QVBoxLayout* layout = new QVBoxLayout ( this );
	plotter = new Plotter ( this );

	TimeConverter_s* tc = new TimeConverter_s(); //create a new converter object...
	plotter->setConverter ( tc ); //override the default "do-nothing"-converter

	layout->addWidget ( plotter );
	layout->setContentsMargins ( 0, 0, 0, 0 );
	layout->setSpacing ( 5 );
}


VisPage::VisPage ( QWidget* parent ) : QWidget ( parent )
{
	QVBoxLayout* layout = new QVBoxLayout ( this );
	tacho = new Tacho ( this );

	layout->addWidget ( tacho );
	layout->setContentsMargins ( 0, 0, 0, 0 );
	layout->setSpacing ( 5 );
}


Display::Display() : QDialog()
{
	startButton = new QPushButton ( tr ( "Start" ) );
	startButton->setDefault ( true );
	quitButton = new QPushButton ( tr ( "Quit" ) );
	quitButton->setAutoDefault ( false );
	previous = new QPushButton ( tr ( "< Numbers" ) );
	next = new QPushButton ( tr ( "Plot >" ) );

	previous->setEnabled ( false );

	QHBoxLayout* mainbuttonBox = new QHBoxLayout();
	mainbuttonBox->addWidget ( startButton );
	mainbuttonBox->addWidget ( previous );
	mainbuttonBox->addWidget ( next );
	mainbuttonBox->addWidget ( quitButton );
	mainbuttonBox->setContentsMargins ( 0, 0, 0, 0 );
	mainbuttonBox->setSpacing ( 5 );

	pages = new QStackedWidget ( this );
	pages->addWidget ( displayPg = new DisplayPage ( pages ) ); // index 0
	pages->addWidget ( plotPg = new PlotPage ( pages ) ); // index 1
	pages->addWidget ( visPg = new VisPage ( pages ) ); // index 2
	pages->addWidget ( urlPg = new URLPage ( pages ) ); // index 3


	QVBoxLayout* mainlayout = new QVBoxLayout ( this );
	mainlayout->addWidget ( pages );
	mainlayout->addLayout ( mainbuttonBox );
	mainlayout->setContentsMargins ( 5, 10, 5, 8 );
	mainlayout->setSpacing ( 6 );

	connect ( next, SIGNAL ( clicked() ), this, SLOT ( doNext() ) );
	connect ( previous, SIGNAL ( clicked() ), this, SLOT ( doPrev() ) );

	connect ( startButton, SIGNAL ( clicked() ), this, SLOT ( startDisp() ) );
	connect ( quitButton, SIGNAL ( clicked() ), this, SLOT ( close() ) );
	connect ( urlPg->urlLineEdit, SIGNAL ( textChanged ( QString ) ), this, SLOT ( enablestartButton() ) );

	curtimestamp  = 0; //variable for the latest timestamp (for further use)
	valinterval   = 2; //timeinterval (sec) for updating the numerical consumption display
	fetchinterval = 4; //timeinterval (sec) for fetching sensordate from a flukso device
	dlcounter = 0; //counter for debug purposes
	currentsensors = 1; //on app start only one sensor is enabled by default....
	int plotdelay = 1; // delay on plotter updating (see httpReadyRead() method for details)


	tfetch = new QTimer();
	tshow = new QTimer();
	tplot = new QTimer();

	tfetch->setInterval ( 1000 * fetchinterval );
	tshow->setInterval ( 1000 * valinterval );
	tplot->setInterval ( 1000 * plotdelay );

	map1 = new QMap<uint, uint>();
	map2 = new QMap<uint, uint>();
	map3 = new QMap<uint, uint>();


	connect ( tshow, SIGNAL ( timeout() ), this, SLOT ( showCurrentVal_alt() ) );
	connect ( tfetch, SIGNAL ( timeout() ), this, SLOT ( getAllSensors_new() ) );
	connect ( tplot, SIGNAL ( timeout() ), this, SLOT ( updatePlotter() ) );

	finishedMapper = new QSignalMapper ( this );
	readyreadMapper = new QSignalMapper ( this );
	errMapper = new QSignalMapper ( this );

	connect ( finishedMapper, SIGNAL ( mapped ( QObject* ) ), this, SLOT ( httpFinished ( QObject* ) ) );
	connect ( readyreadMapper, SIGNAL ( mapped ( QObject* ) ), this, SLOT ( httpReadyRead ( QObject* ) ) );
	connect ( errMapper, SIGNAL ( mapped ( QObject* ) ), this, SLOT ( sensorErr ( QObject* ) ) );

	connect ( urlPg->sen1, SIGNAL ( toggled ( bool ) ), this, SLOT ( buttonToggled_gatekeeper ( bool ) ) );
	connect ( urlPg->sen2, SIGNAL ( toggled ( bool ) ), this, SLOT ( buttonToggled_gatekeeper ( bool ) ) );
	connect ( urlPg->sen3, SIGNAL ( toggled ( bool ) ), this, SLOT ( buttonToggled_gatekeeper ( bool ) ) );

	qDebug() << "created..";

	checkSettings();
}


//performs a quick check at app-start; if the flukso device specified in "flukso.conf" with
//it's first sensor is reachable or not
void Display::checkSettings()
{
	if ( QFile::exists ( "/mnt/usb/flukso.conf" ) ) {

		QSettings settings ( "/mnt/usb/flukso.conf", QSettings::IniFormat );
		QString ipport ( settings.value ( "ip" ).toString() + ":" + settings.value ( "port" ).toString() );
		QString sen ( settings.value ( "sensor1" ).toString() );

		QNetworkRequest req ( QUrl ( "http://" + ipport + QString ( "/sensor/%1?unit=watt&interval=minute&version=1.0" ).arg ( sen ) ) );

		qDebug() << "---checker-->" << req.url().toString();

		QNetworkAccessManager* manager = new QNetworkAccessManager ( this );
		manager->get ( req );

		connect ( manager, SIGNAL ( finished ( QNetworkReply* ) ), this, SLOT ( checkSettingsStatus ( QNetworkReply* ) ) );

	} else {
		qDebug() << "could not find the flukso configuration file...";

		QMessageBox* msgBox = new QMessageBox ( QMessageBox::Critical, "Configuration Error", "Couldn't find the configuration file!", QMessageBox::Ok , this, Qt::Dialog | Qt::CustomizeWindowHint );
		msgBox->move ( 0, 1 );
		msgBox->exec();
	}

}


//evaluates the result of the networking check at the start..
void Display::checkSettingsStatus ( QNetworkReply* rep )
{

	if ( rep->error() == QNetworkReply::NoError ) {
		qDebug() << "-out-->" << rep->readAll();
		rep->deleteLater();
		startDisp();
	} else {
		qDebug() << QString ( "there was a problem downloading %1 data!" ).arg ( rep->url().toString() );
		qDebug() << "error was: " << rep->errorString();

		QMessageBox* msgBox = new QMessageBox ( QMessageBox::Critical, "Connection Error", "Couldn't reach the Flukso device. Check your Settings!", QMessageBox::Abort | QMessageBox::Retry, this, Qt::Dialog | Qt::CustomizeWindowHint  );
		msgBox->layout()->setContentsMargins ( 5, 6, 5, 6 );
		msgBox->layout()->setSpacing ( 8 );
		msgBox->move ( 0, 1 );
		msgBox->setDetailedText ( ( QString ( "Sensor URL: %1\nError: %2" ).arg ( rep->url().toString() ).arg ( rep->errorString() ) ).remove ( "?unit=watt&interval=minute&version=1.0" , Qt::CaseInsensitive ) );
		int	ret = msgBox->exec();

		if ( ret == QMessageBox::Abort ) {
			rep->deleteLater();
			close();
		} else if ( ret == QMessageBox::Retry ) {
			rep->deleteLater();
			checkSettings();
		}
		//QMessageBox* msgBox = new QMessageBox ( QMessageBox::Critical, "Connection Error" , "Couldn't reach the Flukso device. Check your Settings!", QMessageBox::Ok | QMessageBox::Retry , this, Qt::Dialog | Qt::CustomizeWindowHint );
		//msgBox->exec();
	}
	//rep->deleteLater();
}


void Display::buttonToggled_gatekeeper ( bool checked )
{
	( checked ) ? currentsensors++ : currentsensors--;
	qDebug() << "sensors enabled: " << currentsensors;

	if ( !urlPg->sen1->isChecked() ) {
		map1->clear();
		qDebug() << "sensor1 data (map1) cleared!";
	}

	if ( !urlPg->sen2->isChecked() ) {
		map2->clear();
		qDebug() << "sensor2 data (map1) cleared!";
	}

	if ( !urlPg->sen3->isChecked() ) {
		map3->clear();
		qDebug() << "sensor3 data (map1) cleared!";
	}
}


void Display::updatePlotter()
{
	//qDebug() << "plotupdate triggered..";
	tplot->stop();
	plotData_new ( plotPg->plotter );
	showAvg();
}


void Display::doNext() // TODO: refactor button functions!
{
	switch ( pages->currentIndex() ) {

		case 0: //displayPg
			pages->setCurrentWidget ( plotPg );
			previous->setText ( tr ( "< Numbers" ) );
			previous->setEnabled ( true );
			next->setText ( tr ( "Visual >" ) );
			break;

		case 1: //plotPg
			pages->setCurrentWidget ( visPg );
			previous->setText ( tr ( "< Plot" ) );
			next->setText ( tr ( "Settings >" ) );
			break;

		case 2: //visPg
			pages->setCurrentWidget ( urlPg );
			previous->setText ( tr ( "< Visual" ) );
			next->setText ( tr ( "Settings >" ) );
			next->setEnabled ( false );
			break;
	}
}


void Display::doPrev()
{

	switch ( pages->currentIndex() ) {

		case 1: //plotPg
			pages->setCurrentWidget ( displayPg );
			previous->setEnabled ( false );
			previous->setText ( tr ( "< Numbers" ) );
			next->setText ( tr ( "Plot >" ) );
			break;

		case 2: //visPg
			pages->setCurrentWidget ( plotPg );
			previous->setText ( tr ( "< Numbers" ) );
			next->setText ( tr ( "Visual >" ) );
			break;

		case 3: //urlPg
			pages->setCurrentWidget ( visPg );
			previous->setText ( tr ( "< Plot" ) );
			next->setEnabled ( true );
			next->setText ( tr ( "Settings >" ) );
			break;
	}
}


void Display::enablestartButton()
{
	startButton->setEnabled ( !urlPg->urlLineEdit->text().isEmpty() );
}


void Display::startDisp()
{
	if ( tshow->isActive() || tfetch->isActive() ) {
		qDebug() << "stoping...";
		tfetch->stop();
		tshow->stop();
		qDebug() << "timer stoped.";

		dlcounter = 0;
		map1->clear();
		map2->clear();
		map3->clear();

		startButton->setText ( tr ( "Start" ) );
		urlPg->statusLabel->setText ( tr ( "Stopped" ) );
	} else {
		qDebug() << "starting...";
		getAllSensors_new();
		tfetch->start();
		tshow->start();
		qDebug() << "timer started...";

		startButton->setText ( tr ( "Stop" ) );
	}

}


void Display::getSensor ( QNetworkReply*& replyn, QString sensortoken, QString sensormarker )
{

	QString sensortxt = "/sensor/%1?unit=watt&interval=minute&version=1.0";
	QNetworkRequest req ( QUrl ( "http://" + urlPg->urlLineEdit->text() + sensortxt.arg ( sensortoken ) ) );

	req.setAttribute ( QNetworkRequest::User, QString ( sensormarker ) );

	replyn = qnam.get ( req ); //store reply in a QNetworkReply object

	finishedMapper->setMapping ( replyn, qobject_cast<QObject*> ( replyn ) );
	readyreadMapper->setMapping ( replyn, qobject_cast<QObject*> ( replyn ) );
	errMapper->setMapping ( replyn, qobject_cast<QObject*> ( replyn ) );

	connect ( replyn, SIGNAL ( finished() ), finishedMapper, SLOT ( map() ) );
	connect ( replyn, SIGNAL ( readyRead() ), readyreadMapper, SLOT ( map() ) );
	connect ( replyn, SIGNAL ( error ( QNetworkReply::NetworkError ) ), errMapper, SLOT ( map() ) );
}


void Display::getAllSensors_new()
{
	if ( !urlPg->sensorLabel1->text().isEmpty() && urlPg->sen1->isChecked() ) {
		getSensor ( reply1 , urlPg->sensorLabel1->text(), "sensor1" );
	}

	if ( !urlPg->sensorLabel2->text().isEmpty() && urlPg->sen2->isChecked() ) {
		getSensor ( reply2 , urlPg->sensorLabel2->text(), "sensor2" );
	}

	if ( !urlPg->sensorLabel3->text().isEmpty() && urlPg->sen3->isChecked() ) {
		getSensor ( reply3 , urlPg->sensorLabel3->text(), "sensor3" );
	}
}


void Display::httpFinished ( QObject* repn )
{
	QNetworkReply* replyn = qobject_cast<QNetworkReply*> ( repn );

	if ( replyn->error() ) {
		qDebug() << QString ( tr ( "Sensordata (%1) Download failed: %2" ).arg ( replyn->request().attribute ( QNetworkRequest::User ).toString() ).arg ( replyn->errorString() ) );
	} else {
		dlcounter += 1;
		urlPg->statusLabel->setText ( tr ( "Sensordata downloaded! (count: %1)" ).arg ( dlcounter ) );
	}

	replyn->deleteLater();
	replyn = 0;

}


// this slot is only triggered when QNetworkReply has new data available,
// not when the finished() signal is emmited - this way we use less RAM
void Display::httpReadyRead ( QObject* repn )
{
	QNetworkReply* replyn = qobject_cast<QNetworkReply*> ( repn );

	//trigger a plot delay timer; if there is another sensor readout while the
	//timer is running, the plot update will be delayed again (timer restart).
	//when a timeout occurs, meaning there was no sensor readout during the delay,
	//the plot will be updated and averages recalculated. this way the peak cpu
	//usage on the chumby is reduced..
	tplot->start();

	QByteArray result;
	result = replyn->readAll();

	QString sensormark ( replyn->request().attribute ( QNetworkRequest::User ).toString() );
	//qDebug() << "req user data: " << sensormark;

	QMap<uint, uint>* map = 0;

	//depending on which sensordata was requested, the "output" map is switched...
	if ( sensormark == QString ( "sensor1" ) ) {
		map = map1;
	} else if ( sensormark == QString ( "sensor2" ) ) {
		map = map2;
	} else if ( sensormark == QString ( "sensor3" ) ) {
		map = map3;
	}


	bool ok;
	QVariantList json = QtJson::Json::parse ( QString ( result ), ok ).toList();

	if ( !ok ) {
		qFatal ( "An error occurred during parsing" );
	} else {
		if ( json.isEmpty() ) {
			qDebug() << "json data is empty!";
		}

		foreach ( QVariant data, json ) {
			if ( data.toList().value ( 1 ).toString() != "nan" ) {
				map->insert ( data.toList().value ( 0 ).toUInt(), data.toList().value ( 1 ).toUInt() );
			} else {
				map->insert ( data.toList().value ( 0 ).toUInt(), ( map->end() - 1 ).value() );
				//map->insert ( data.toList().value ( 0 ).toUInt(), map->value( data.toList().value ( 0 ).toUInt() - 1 ) ); //alternative
				//map->insert ( data.toList().value ( 0 ).toUInt(), 0 ); //alternative2

				//qDebug() << "zero/nan value found!-> dupe inserted - map size was: " << map->size() << "for " << sensormark;
			}
			//qDebug() << data.toList().value(1).toString();
		}

		if ( map->size() > 2100 ) {
			QMap<uint, uint>::iterator i = map->begin();
			while ( i != map->end() - 1801  ) {
				i = map->erase ( i );
			}
			qDebug() << sensormark << "data map size was cut down! new size: " << map->size();
		}

		urlPg->debugLabel->setText ( tr ( "Map sizes: %1 | %2 | %3" ).arg ( map1->size() ).arg ( map2->size() ).arg ( map3->size() ) );

	}

}

void Display::plotData_new ( Plotter* plotter )
{

	int plotlen = 900; //TODO: make plot length method parameter, thus variable

	bool s1 = urlPg->sen1->isChecked();
	bool s2 = urlPg->sen1->isChecked();
	bool s3 = urlPg->sen1->isChecked();

	if ( plotlen > 240 ) {
		TimeConverter_m* tc = new TimeConverter_m();
		plotPg->plotter->setConverter ( tc );
	}

	uint minx = 0;
	uint maxx = 0;
	uint miny = 0;
	uint maxy = 0;

	uint maxx1 = ( s1 && !map1->isEmpty() ) ? ( map1->end() - 1 ).key() : 0;
	uint maxx2 = ( s2 && !map2->isEmpty() ) ? ( map2->end() - 1 ).key() : 0;
	uint maxx3 = ( s3 && !map3->isEmpty() ) ? ( map3->end() - 1 ).key() : 0;

	if ( maxx1 >= maxx2 && maxx1 >= maxx3 ) {
		maxx = maxx1;
		minx = ( ( map1->begin() ).key() ) - ( plotlen - map1->size() );
		miny = ( map1->end() - 1 ).value();
	} else if ( maxx2 >= maxx1 && maxx2 >= maxx3 ) {
		maxx = maxx2;
		minx = ( ( map2->begin() ).key() ) - ( plotlen - map2->size() );
		miny = ( map2->end() - 1 ).value();
	} else if ( maxx3 >= maxx1 && maxx3 >= maxx2 ) {
		maxx = maxx3;
		minx = ( ( map3->begin() ).key() ) - ( plotlen - map3->size() );
		miny = ( map3->end() - 1 ).value();
	}

	curtimestamp = maxx;
	//qDebug() << "minx pre: " << minx << " | maxx pre: " << maxx << " | miny pre : " << miny << " | maxy pre: " << maxy ;

	if ( s1 && !map1->isEmpty() ) {
		QVector<QPointD> plval = plotData_helper ( map1, plotlen, miny, maxy );
		plotter->setCurveData ( 0, plval );
		//qDebug() << "minx m1: " << miny << " | maxy m1: " << maxy;
	}

	if ( s2 && !map2->isEmpty() ) {
		QVector<QPointD> plval = plotData_helper ( map2, plotlen, miny, maxy );
		plotter->setCurveData ( 1, plval );
		//qDebug() << "minx m2: " << miny << " | maxy m2: " << maxy;
	}

	if ( s3 && !map3->isEmpty() ) {
		QVector<QPointD> plval = plotData_helper ( map3, plotlen, miny, maxy );
		plotter->setCurveData ( 2, plval );
		//qDebug() << "minx m3: " << miny << " | maxy m3: " << maxy;
	}

	miny = miny - std::floor ( miny * 0.1 );
	maxy = maxy + std::ceil ( maxy * 0.1 );

	//qDebug() << "minx: " << minx << " | maxx: " << maxx << " | miny: " << miny << " | maxy: " << maxy << "\n";
	//PlotSettings* pset = new PlotSettings (  minx, miny, maxx, maxy, 4, 5 );
	//plotter->setPlotSettings ( *pset );
	plotter->setPlotSettings ( * ( new PlotSettings (  minx, miny, maxx, maxy, 4, 5 ) ) );

}

// plot-helper method; just to keep the code a bit cleaner..
QVector<QPointD> Display::plotData_helper ( QMap<uint, uint>* mp, int plen, uint& miny, uint& maxy )
{
	int msize = mp->size();
	int goback = ( msize < plen ) ? msize : plen ;

	QVector<QPointD> plval;
	QMap<uint, uint>::const_iterator i;

	for ( i = mp->constEnd() - goback ; i != mp->constEnd(); ++i ) {
		uint val = i.value();
		plval.append ( QPointD ( i.key(), val ) );

		if ( val > maxy ) {
			maxy = val;
		} else if ( val < miny ) {
			miny = val;
		}
	}

	return plval;
}


void Display::showCurrentVal_alt()
{
	//qDebug() << "lastval: " << displayPg->lastval;
	//qDebug() << "mapend:" << ( map->constEnd() - 1 ).key();

	if ( ! ( displayPg->lastval == ( map1->constEnd() - 1 ).key() ) ) {
		displayPg->lastval = ( map1->constEnd() - 1 ).key();
		displayPg->valueiter = 0;
	} else {
		displayPg->valueiter = displayPg->valueiter + valinterval;
		//qDebug() << "valiter: " << displayPg->valueiter;
	}

	uint showts = ( map1->constEnd() - 1 - ( int ) fetchinterval + ( int ) displayPg->valueiter ).key();

	//output data for calculated timestamp...
	QDateTime dtime = QDateTime::fromMSecsSinceEpoch ( quint64 ( showts ) * 1000 );
	displayPg->digitalClk->display (  dtime.toString ( "hh:mm:ss" ) );
	displayPg->sensorval->display (  QString::number ( map1->value ( showts ) ) );

	visPg->tacho->setValue ( map1->value ( showts ) );
	//qDebug() << "show:" << show;
	//qDebug() << "mapvals: "  <<  map1->value ( show ) << "\n";
}


void Display::showAvg() //TODO: make dependant on which sensor data is displayed in the first place..
{
	uint avg1 = 0;
	uint avg2 = 0;
	int size = map1->size();
	int a1end = 0;
	int a2end = 0;


	if ( map1->isEmpty() ) {
		return;
	}

	QMap<uint, uint>::const_iterator i;

	( size < 60 ) ? a1end = size : a1end = 60; //1min
	( size < 900 ) ? a2end = size : a2end = 900; // 10min = 600

	for ( i = ( map1->constEnd() - 1 ); i != ( map1->constEnd() - 1 - a1end ); --i ) {
		avg1 += i.value(); //TODO: hier absichern falls values nicht geliefert wurden...
	}
	avg1 = avg1 / a1end;
	displayPg->avgval1->display ( ( int ) avg1 );
	//qDebug() << "avg1: " << avg1;

	for ( i = ( map1->constEnd() - 1 ); i != ( map1->constEnd() - 1 - a2end ); --i ) {
		avg2 += i.value();

	}
	avg2 = avg2 / a2end;
	displayPg->avgval2->display ( ( int ) avg2 );
	//qDebug() << "avg2: " << avg2;
}


void Display::sensorErr ( QObject* repn )
{
	QNetworkReply* replyn = qobject_cast<QNetworkReply*> ( repn );
	qDebug() << QString ( "seems there was a problem downloading %1 data!" ).arg ( replyn->request().attribute ( QNetworkRequest::User ).toString() );
	qDebug() << "error was: " << replyn->errorString();

	urlPg->statusLabel->setText ( tr ( "http-reply error! - resuming in 2s.." ) );
	QTimer::singleShot ( 2000, this, SLOT ( getAllSensors_new() ) );

}


QString TimeConverter_s::convert ( double value )
{
	QDateTime dtime = QDateTime::fromMSecsSinceEpoch ( qint64 ( value ) * 1000 );
	return dtime.toString ( "hh:mm:ss" );
}


QString TimeConverter_m::convert ( double value )
{
	QDateTime dtime = QDateTime::fromMSecsSinceEpoch ( qint64 ( value ) * 1000 );
	return dtime.toString ( "hh:mm" );
}
