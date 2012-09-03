#include <QWidget>
#include <QCheckBox>
#include <QTextEdit>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QListWidget>
#include <QProgressBar>
#include <QLineEdit>

#include <QDomDocument>
#include <QProcess>
#include <QFile>
#include <QTextStream>

//#include <QTest> //temp for debug!

#include "wizard.h"
#include "chumbyvnumpad/chumbyvnumpad.h"
#include "chumbyvkbd/chumbyvkbd.h"



IntroPage::IntroPage ( QWidget *parent ) : QWidget ( parent )
{
	QVBoxLayout* layout = new QVBoxLayout ( this );

	QTextEdit* textEdit = new QTextEdit();
	textEdit->setReadOnly ( true );
	textEdit->setHtml ( tr ( "<h2>Welcome to the Chumby WiFi-Network Setup!</h2>"
	                         "<p>Just follow the Instructions onscreen.<br> \
	                         Please consider that this assistant is a work-in-progress project and there may be some rough edges! <br><br> \
	                         When you run across a bug, please send me a mail at montibel@itwm.fhg.de</p>" ) );

	layout->addWidget ( textEdit );

	layout->setContentsMargins ( 0, 0, 0, 0 );
	layout->setSpacing ( 6 );
}


ScanPage::ScanPage ( QWidget* parent ) : QWidget ( parent )
{
	// create public variables
	selectedap = new QMap<QString, QVariant>();
	securedap = false;

	QVBoxLayout* layout = new QVBoxLayout ( this );

	aplist = new QListWidget;
	aplist->setFlow ( QListView::TopToBottom );
	aplist->setViewMode ( QListView::ListMode );
	aplist->setSelectionMode ( QAbstractItemView::SingleSelection );
	aplist->setSelectionBehavior ( QAbstractItemView::SelectItems );
	//aplist->setEditFocus(true);

	QFont aplfont = aplist->font();
	qDebug ( "Fontsize: %d", aplfont.pointSize() );
	aplfont.setPointSize ( 18 );
	aplist->setFont ( aplfont );
	aplist->setSpacing ( 4 );
	//aplist->setVisible(false);

	QProgressBar* bar = new QProgressBar();
	bar->setRange ( 0, 0 );
	bar->setValue ( 1 );
	bar->setVisible ( false );
	bar->setStyleSheet ( "margin-bottom: 40px" );

	scanbtn = new QPushButton ( "Scan for WiFi Networks..." );
	qDebug ( "Scanbtn height: %d", scanbtn->sizeHint().height() );
	scanbtn->setFixedHeight ( ( int ) ( scanbtn->sizeHint().height() * 1.5 ) );
	QFont scbtfont = scanbtn->font();
	scbtfont.setPointSize ( 14 );
	scanbtn->setFont ( scbtfont );

	layout->addStretch();
	layout->addWidget ( aplist );
	layout->addWidget ( bar );
	layout->addWidget ( scanbtn );
	layout->setContentsMargins ( 0, 0, 0, 0 );
	layout->setSpacing ( 6 );

	scanrun = false;
	connect ( scanbtn, SIGNAL ( clicked() ), this, SLOT ( scanAP() ) );
	//connect ( scanbtn, SIGNAL ( clicked() ), this, SLOT ( parseAP() ) ); //change for actuall use here!!
	connect ( this, SIGNAL ( scanning ( bool ) ), bar, SLOT ( setHidden ( bool ) ) );
	connect ( this, SIGNAL ( scanning ( bool ) ), scanbtn, SLOT ( setEnabled ( bool ) ) );
	connect ( this, SIGNAL ( scanning ( bool ) ), aplist, SLOT ( setVisible ( bool ) ) );
	connect ( aplist, SIGNAL ( itemClicked ( QListWidgetItem* ) ), this, SLOT ( enableNext ( QListWidgetItem* ) ) );

	scanscript = new QProcess();
	connect ( scanscript, SIGNAL ( finished ( int, QProcess::ExitStatus ) ), this, SLOT ( parseAP() ) );
}


void ScanPage::enableNext ( QListWidgetItem* itm )
{
	if ( itm->isSelected() ) {
		qDebug ( "signal triggered..." );
		emit selected ( true );

	} else {
		qDebug ( "signal not triggered propperly" );
	}
}


void ScanPage::scanAP()
{
	if ( scanrun == false ) {
		scanrun = true;
		emit scanning ( !scanrun );
	}

	QString command = "/usr/chumby/scripts/ap_scan";

	scanscript->start ( command );

	//if (scanrun == true)
	//{
	//scanrun = false;
	//emit scanning(scanrun);
	//}
}


void ScanPage::parseAP()
{
	QDomDocument doc ( "" );
	QString errorStr;
	int errorLine;
	int errorColumn;

	aplist->clear();
	//----

	//QFile file ( "apxml.xml" );
	//QString instr = "";

	//if ( file.open ( QIODevice::ReadOnly ) ) {
	//QTextStream stream ( &file );

	//while ( !stream.atEnd() ) {
	//instr = stream.readAll(); // line of text excluding '\n'
	//}

	//file.close();
	//}

	//---
	QString instr ( scanscript->readAll() );

	if ( !instr.startsWith ( "<aps>" ) ) {
		instr = instr.remove ( 0, instr.indexOf ( "<aps>" ) );
	}

	qDebug ( "%s", qPrintable ( instr ) );

	if ( !doc.setContent ( instr, &errorStr, &errorLine, &errorColumn ) ) {
		qCritical ( "Problem with setContent!" );
		qCritical ( "at Line: %d", errorLine );
		qCritical ( "at Col: %d", errorColumn );
		qCritical ( "Err.: %s", qPrintable ( errorStr ) );
		exit ( 1 );
	}


	QDomElement root = doc.documentElement();

	if ( root.tagName() != "aps" )
		exit ( 2 );

	QDomNode n = root.firstChild();

	while ( !n.isNull() ) {
		QDomElement e = n.toElement();

		if ( !e.isNull() ) {
			if ( e.tagName() == "ap" ) {
				QMap<QString, QVariant> wifiap;
				wifiap.insert ( "ssid", e.attribute ( "ssid", "" ) );
				wifiap.insert ( "hwaddr", e.attribute ( "hwaddr", "" ) );
				wifiap.insert ( "channel", e.attribute ( "channel", "" ) );
				wifiap.insert ( "auth", e.attribute ( "auth", "" ) );
				wifiap.insert ( "encryption", e.attribute ( "encryption", "" ) );
				wifiap.insert ( "linkquality", e.attribute ( "linkquality", "" ) );
				wifiap.insert ( "mode", e.attribute ( "mode", "" ) );
				wifiap.insert ( "wps", e.attribute ( "wps", "" ) );
				wifiap.insert ( "type", "wlan" );

				QListWidgetItem* item = new QListWidgetItem ( e.attribute ( "ssid", "" ) + " | " + wifiap.value ( "linkquality" ).toString() + "%", aplist );
				item->setData ( Qt::UserRole, QVariant ( wifiap ) );
				item->setIcon ( QIcon ( ( wifiap.value ( "encryption" ) == "NONE" ) ? ":/icon/ressource/open.png" : ":/icon/ressource/locked.png" ) );
			}
		}

		n = n.nextSibling();
	}

	if ( scanrun == true ) {
		scanrun = false;
		emit scanning ( !scanrun );
	}
}


const QListWidgetItem* ScanPage::getListSelection()
{
	return aplist->currentItem();
}


void ScanPage::getAPData ( QMap<QString, QVariant>* /*apoint*/ )
{
	qDebug ( "geting ap data.." );
	//const QListWidgetItem* selected = aplist->currentItem();
	//QVariant apinfo = selected->data ( Qt::UserRole );
	QVariant apinfo = aplist->currentItem()->data ( Qt::UserRole );

	if ( !apinfo.canConvert ( QVariant::Map ) ) {
		qDebug ( "Error while reading stored AP Information!" );
	} else {
		qDebug ( "mapping AP data..." );
		*selectedap = apinfo.toMap();
		//*apoint = apinfo.toMap();
		qDebug ( "ap data mapped!" );
	}

	//check if selected WiFi is protected by any encryption and set a flag
	//if ( !apoint->isEmpty() ) {
	if ( !selectedap->isEmpty() ) {
		if ( selectedap->value ( "encryption" ).toString() != "NONE" ) {
			//if ( apoint->value ( "encryption" ).toString() != "NONE" ) {
			qDebug ( "selected WiFi is using encryption protection!" );
			securedap = true;
		} else {
			qDebug ( "selected WiFi is using no encryption!" );
			securedap = false;
		}
	}
}


WifiKeyPage::WifiKeyPage ( QWidget* parent ) : QWidget ( parent )
{
	//Wizard* rparent = qobject_cast<Wizard*>(parent);

	QVBoxLayout* layout = new QVBoxLayout ( this );
	layout->setContentsMargins ( 0, 0, 0, 0 );
	lab = new QLabel ( "Enter the Encryption Key (ASCII):" );
	key = new QLineEdit ( this );

	ChumbyVirtualKeyboard* keyboard = new ChumbyVirtualKeyboard ( this );
	keyboard->hideHideBtn ( true );
	keyboard->show();

	layout->addWidget ( lab );

	layout->addStretch();
	layout->addWidget ( key );
	layout->addStretch();
	layout->addWidget ( keyboard );
	layout->addStretch();
}


void WifiKeyPage::setLabel ( ScanPage* page )
{
	if ( page->securedap ) {
		lab->setText ( QString ( "Enter the Encryption Key (ASCII) for %1" ).arg ( page->selectedap->value ( "ssid" ).toString() ) );
		//key->setFocus();
	}
}


void WifiKeyPage::saveKey ( ScanPage* page )
{
	page->selectedap->insert ( "key", key->text() );
}


CfgSelectPage::CfgSelectPage ( QWidget* parent ) : QWidget ( parent )
{
	QVBoxLayout* layout = new QVBoxLayout ( this );
	layout->setContentsMargins ( 0, 0, 0, 0 );

	QGroupBox* groupBox = new QGroupBox ( tr ( "Select connection configuration type" ) );

	radio1 = new QRadioButton ( tr ( "Automatic (DHCP) - recommended" ) );
	radio2 = new QRadioButton ( tr ( "Manual" ) );

	radio1->setChecked ( true );

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addStretch ( 2 );
	vbox->addWidget ( radio1 );
	vbox->addStretch ( 1 );
	vbox->addWidget ( radio2 );
	vbox->addStretch ( 2 );
	groupBox->setLayout ( vbox );

	//layout->addStretch();
	layout->addWidget ( groupBox );
	//layout->addStretch();
}


void CfgSelectPage::setAllocation ( ScanPage* page )
{
	if ( this->radio1->isChecked() ) {
		page->selectedap->insert ( "allocation", "dhcp" );
		if ( page->selectedap->contains ( "ip" ) ) {
			page->selectedap->remove ( "ip" );
			page->selectedap->remove ( "gateway" );
			page->selectedap->remove ( "netmask" );
			page->selectedap->remove ( "nameserver1" );
			page->selectedap->remove ( "nameserver2" );
		}

	} else {
		page->selectedap->insert ( "allocation", "static" );
		page->selectedap->insert ( "ip", "" );
		page->selectedap->insert ( "gateway", "" );
		page->selectedap->insert ( "nameserver1", "" );
		page->selectedap->insert ( "nameserver2", "" );
		page->selectedap->insert ( "netmask", "255.255.255.0" );
	}
}


InputPage::InputPage ( QString name, QWidget* parent ) : QWidget ( parent )
{
	QVBoxLayout* layout = new QVBoxLayout ( this );
	layout->setContentsMargins ( 0, 0, 0, 0 );

	QLabel* label = new QLabel ( QString ( "Enter the %1 of your Chumby:" ).arg ( name ) );
	input = new QLineEdit ( this );

	ChumbyVirtualNumpad* numpad = new ChumbyVirtualNumpad ( this );

	layout->addWidget ( label );
	layout->addWidget ( input );
	layout->addStretch();
	layout->addWidget ( numpad );
	layout->addStretch();
}


void InputPage::saveInput ( ScanPage* page, QString option )
{
	qDebug ( "%s: %s", qPrintable ( option ), qPrintable ( input->text() ) );
	page->selectedap->insert ( option , input->text() );
}


ShowCfgPage::ShowCfgPage ( QWidget* parent ) : QWidget ( parent )
{
	QVBoxLayout* layout = new QVBoxLayout ( this );
	layout->setContentsMargins ( 0, 0, 0, 0 );
	textEdit = new QTextEdit();
	textEdit->setReadOnly ( true );

	//textEdit2 = new QTextEdit();
	//textEdit2->setReadOnly ( true );

	layout->addWidget ( new QLabel ( tr ( "Selected WiFi Network:" ) ) );
	layout->addWidget ( textEdit );
	//layout->addWidget ( textEdit2 );
}


void ShowCfgPage::getAPInfo ( ScanPage* page )
{
	QString info = "";
	info.append ( "SSID: " + page->selectedap->value ( "ssid" ).toString() + "\n" );
	info.append ( "MAC: " + page->selectedap->value ( "hwaddr" ).toString() + "\n" );
	info.append ( "AUTH: " + page->selectedap->value ( "auth" ).toString() + "\n" );
	info.append ( "ENCRYPTION: " + page->selectedap->value ( "encryption" ).toString() + "\n" );
	if ( page->selectedap->value ( "encryption" ).toString() != "NONE" ) {
		info.append ( "WIFI_KEY: " + page->selectedap->value ( "key" ).toString() + "\n" );
	}
	info.append ( "LINK QUALITY: " + page->selectedap->value ( "linkquality" ).toString() + "\n" );
	info.append ( "CHANNEL: " + page->selectedap->value ( "channel" ).toString() + "\n" );
	info.append ( "ALLOCATION: " + page->selectedap->value ( "allocation" ).toString() + "\n" );

	if ( page->selectedap->value ( "allocation" ).toString() == "static" ) {
		info.append ( "IP: " + page->selectedap->value ( "ip" ).toString() + "\n" );
		info.append ( "NETMASK: " + page->selectedap->value ( "netmask" ).toString() + "\n" );
		info.append ( "DNS 1:" + page->selectedap->value ( "nameserver1" ).toString() + "\n" );
		info.append ( "DNS 2:" + page->selectedap->value ( "nameserver2" ).toString() + "\n" );
	}

	info.append ( "TYPE: " + page->selectedap->value ( "type" ).toString() + "\n" );

	textEdit->setPlainText ( info );

}


void ShowCfgPage::writeCfg ( ScanPage* page )
{
	QString cfg = "<configuration ";

	cfg.append ( "allocation=\"" + page->selectedap->value ( "allocation" ).toString() + "\" " );
	if ( page->selectedap->value ( "allocation" ).toString() == "static" ) {
		cfg.append ( "gateway=\"" + page->selectedap->value ( "gateway" ).toString() + "\" " );
		cfg.append ( "ip=\"" + page->selectedap->value ( "ip" ).toString() + "\" " );
		cfg.append ( "nameserver1=\"" + page->selectedap->value ( "nameserver1" ).toString() + "\" " );
		cfg.append ( "nameserver2=\"" + page->selectedap->value ( "nameserver2" ).toString() + "\" " );
		cfg.append ( "netmask=\"" + page->selectedap->value ( "netmask" ).toString() + "\" " );
	}
	cfg.append ( "key=\"" + page->selectedap->value ( "key" ).toString() + "\" " );
	if ( page->selectedap->value ( "encryption" ).toString() != "NONE" ) {
		cfg.append ( "encoding=\"ascii\" " );
	}
	cfg.append ( "encryption=\"" + page->selectedap->value ( "encryption" ).toString() + "\" " );
	cfg.append ( "auth=\"" + page->selectedap->value ( "auth" ).toString() + "\" " );
	cfg.append ( "hwaddr=\"" + page->selectedap->value ( "hwaddr" ).toString() + "\" " );
	cfg.append ( "ssid=\"" + page->selectedap->value ( "ssid" ).toString() + "\" " );
	cfg.append ( "type=\"" + page->selectedap->value ( "type" ).toString() + "\" " );
	cfg.append ( "/>" );

	qDebug ( "%s", qPrintable ( cfg ) );
	//textEdit2->setPlainText ( cfg );

	qDebug ( "currdir: %s", qPrintable ( QDir::currentPath() ) );
	//QFile cfgfile ( "/mnt/usb/network_config" );
	QFile cfgfile ( "/psp/network_config" );


	if ( cfgfile.open ( QIODevice::WriteOnly | QIODevice::Text ) ) {
		QTextStream out ( &cfgfile );
		out << cfg;
		cfgfile.close();
	} else {
		qDebug ( "%s", qPrintable ( QString ( "could not open file: %1" ).arg ( cfgfile.fileName() ) ) );
	}

	//the following code is for debugging only - it copies the generated cfg-file
	//to the external usb storage device...please remove/comment it if not needed!
	/*
	if ( QFile::exists ( "/mnt/usb/netconf/network_config" ) ) {
		QFile::remove ( "/mnt/usb/netconf/network_config" );
		qDebug ( "%s", qPrintable ( QString ( "CFG File removed..." ) ) );
	} else {
		QFile::copy ( "/psp/network_config", "/mnt/usb/netconf/network_config" );
		qDebug ( "%s", qPrintable ( QString ( "CFG File copied..." ) ) );
	}
	*/
}

ConnectionTest::ConnectionTest ( QWidget* parent ) : QWidget ( parent )
{
	QVBoxLayout* layout = new QVBoxLayout ( this );

	result = new QTextEdit ( this );
	result->setReadOnly ( true );
	result->setPlainText ( "Log:" );
	result->setVisible ( false );

	title = new QLabel ( "Checking connection..." );

	QProgressBar* bar = new QProgressBar();
	bar->setRange ( 0, 0 );
	bar->setValue ( 1 );
	bar->setVisible ( false );
	bar->setStyleSheet ( "margin-bottom: 40px" );


	layout->addStretch();
	layout->addWidget ( title );
	layout->addStretch();
	layout->addWidget ( result );
	layout->addWidget ( bar );
	layout->addStretch();
	//layout->addWidget ( scanbtn );
	layout->setContentsMargins ( 0, 0, 0, 0 );
	layout->setSpacing ( 6 );

	testrun = false;

	connect ( this, SIGNAL ( connecting ( bool ) ), bar, SLOT ( setVisible ( bool ) ) );
	connect ( this, SIGNAL ( connecting ( bool ) ), result, SLOT ( setHidden ( bool ) ) );

	connectscript = new QProcess();
	checkscript  = new QProcess();
	connect ( connectscript, SIGNAL ( error ( QProcess::ProcessError ) ), this, SLOT ( err() ) );
	connect ( checkscript, SIGNAL ( error ( QProcess::ProcessError ) ), this, SLOT ( err() ) );

	connect ( connectscript, SIGNAL ( finished ( int, QProcess::ExitStatus ) ), this, SLOT ( checkStatus() ) );
	connect ( checkscript, SIGNAL ( finished ( int, QProcess::ExitStatus ) ), this, SLOT ( parseStatus() ) );
}


void ConnectionTest::parseStatus()
{
	QDomDocument doc ( "" );
	QString errorStr;
	int errorLine;
	int errorColumn;
	result->clear();

	//qDebug ( "%s", qPrintable ( QString ( "Status parse (1) ..." ) ) );

	QString instr ( checkscript->readAll() );

	//qDebug ( "%s", qPrintable ( QString ( "Status parse (2) ..." ) ) );

	if ( !instr.startsWith ( "<network>" ) ) {
		instr = instr.remove ( 0, instr.indexOf ( "<network>" ) );
	}
	qDebug ( "%s", qPrintable ( instr ) );

	if ( !doc.setContent ( instr, &errorStr, &errorLine, &errorColumn ) ) {
		qCritical ( "Problem with setContent!" );
		qCritical ( "at Line: %d", errorLine );
		qCritical ( "at Col: %d", errorColumn );
		qCritical ( "Err.: %s", qPrintable ( errorStr ) );
		exit ( 1 );
	}

	//qDebug ( "%s", qPrintable ( QString ( "Status parse (3) ..." ) ) );

	QString outstr;
	QDomElement root = doc.documentElement();

	if ( root.tagName() != "network" ) {
		exit ( 2 );
	}

	QDomNode n = root.firstChild();

	//qDebug ( "%s", qPrintable ( QString ( "Status parse (4) ..." ) ) );

	while ( !n.isNull() ) {
		QDomElement e = n.toElement();
		if ( !e.isNull() ) {
			if ( e.tagName() == "interface" ) {
				if ( e.attribute ( "up", "" ) == "true" ) {
					outstr.append ( "Interface is UP!\n" );
				} else {
					outstr.append ( "Interface is DOWN!\n" );
				}

				if ( e.attribute ( "link", "" ) == "true" ) {
					outstr.append ( "Interface has LINK!\n" );
				} else {
					outstr.append ( "Interface has NO LINK!\n" );
				}

				outstr.append ( "IP: " + e.attribute ( "ip", "" ) + "\n" );
				outstr.append ( "NETMASK: " + e.attribute ( "netmask", "" ) + "\n" );
				outstr.append ( "GATEWAY: " + e.attribute ( "gateway", "" ) + "\n" );
				outstr.append ( "DNS1: " + e.attribute ( "nameserver1", "" ) + "\n" );
				outstr.append ( "DNS2: " + e.attribute ( "nameserver2", "" ) + "\n" );
			}
			
			//this section needs a fix. at the moment it is simply ignored, since "stats" is another child of the "root" element, not 
			//a child of the selected element (interface)..
			if ( e.tagName() == "stats" ) {
				outstr.append ( "Rx: " + e.attribute ( "rx_bytes", "" ) + " bytes in " + e.attribute ( "rx_packets", "" ) + " packets\n" );
				outstr.append ( "Tx: " + e.attribute ( "tx_bytes", "" ) + " bytes in " + e.attribute ( "tx_packets", "" ) + " packets\n" );
			}
		}

		n = n.nextSibling();
	}

	//qDebug ( "%s", qPrintable ( QString ( "Status parse (5) ..." ) ) );
	
	//insert result text...
	result->setPlainText ( outstr );

	//signal the end of the connection attempt..
	if ( testrun == true ) {
		testrun = false;
		emit connecting ( testrun );
	}
}


void ConnectionTest::checkStatus()
{
	qDebug ( "%s", qPrintable ( QString ( "checking..." ) ) );
	QString command = "/usr/chumby/scripts/network_status.sh --fast";
	//QString command = "sh test.sh";

	checkscript->start ( command );
	qDebug ( "out: %s", qPrintable ( QString ( "command run..." ) ) );
}


void ConnectionTest::doConnect()
{
	qDebug ( "%s", qPrintable ( QString ( "connecting..." ) ) );

	//signal the start of the connection atempt..
	if ( testrun == false ) {
		testrun = true;
		emit connecting ( testrun );
		qDebug ( "%s", qPrintable ( QString ( "connecting emit 1" ) ) );
	}

	qDebug ( "%s", qPrintable ( QString ( "connecting..." ) ) );
	//QString command = "/usr/chumby/scripts/start_network wlan0 CONNECT";
	QString command = "/usr/chumby/scripts/start_network";
	//QString command = "sh test.sh";
	connectscript->start ( command );

	qDebug ( "out: %s", qPrintable ( QString ( "command run..." ) ) );

}


void ConnectionTest::err()
{
	if ( testrun == true ) {
		testrun = false;
		emit connecting ( testrun );
		qDebug ( "%s", qPrintable ( QString ( "connecting error emit!" ) ) );
	}

	//scanbtn->setEnabled( false );
	title->setText ( "Checking connection...Error!" );
	qDebug ( "%s", qPrintable ( QString ( "proc err.." ) ) );

}


Wizard::Wizard() : QDialog()
{
	QVBoxLayout* layout = new QVBoxLayout ( this );
	QHBoxLayout* butlayout = new QHBoxLayout();

	QPushButton* cancel = new QPushButton ( tr ( "Cancel" ), this );
	next = new QPushButton ( tr ( "Next >" ) , this );
	previous = new QPushButton ( tr ( "< Previous" ), this );

	pages = new QStackedWidget ( this );

	butlayout->addStretch();
	butlayout->addWidget ( previous );
	butlayout->addWidget ( next );
	butlayout->addWidget ( cancel );
	butlayout->setSpacing ( 6 );
	butlayout->setContentsMargins ( 0, 0, 0, 0 );

	layout->addWidget ( pages );
	layout->addLayout ( butlayout );
	layout->setSpacing ( 8 );
	layout->setContentsMargins ( 5, 6, 5, 6 );

	previous->setEnabled ( false );
	previous->setHidden ( true );
	next->setEnabled ( true );

	connect ( next, SIGNAL ( clicked() ), this, SLOT ( doNext() ) );
	connect ( previous, SIGNAL ( clicked() ), this, SLOT ( doPrev() ) );
	connect ( cancel, SIGNAL ( clicked() ), this, SLOT ( reject() ) );

	pages->addWidget ( introPg = new IntroPage ( pages ) ); // index 0
	pages->addWidget ( scanPg = new ScanPage ( pages ) ); // index 1
	pages->addWidget ( wifikeyPg = new WifiKeyPage ( pages ) ); // index 2
	pages->addWidget ( cfgselectPg = new CfgSelectPage ( pages ) ); // index 3
	pages->addWidget ( ipinputPg = new InputPage ( "IP", pages ) ); // index 4
	pages->addWidget ( netminputPg = new InputPage ( "Netmask", pages ) ); // index 5
	pages->addWidget ( gateinputPg = new InputPage ( "Gateway IP", pages ) ); //index 6
	pages->addWidget ( nameserv1Pg = new InputPage ( "Nameserver 1", pages ) ); //index 7
	pages->addWidget ( nameserv2Pg = new InputPage ( "Nameserver 2", pages ) ); //index 8
	pages->addWidget ( showcfgPg = new ShowCfgPage ( pages ) ); // index 9
	pages->addWidget ( connectPg = new ConnectionTest ( pages ) ); //index 10


	connect ( scanPg, SIGNAL ( scanning ( bool ) ), next, SLOT ( setEnabled ( bool ) ) );
	connect ( scanPg, SIGNAL ( scanning ( bool ) ), previous, SLOT ( setEnabled ( bool ) ) );
	connect ( scanPg, SIGNAL ( selected ( bool ) ), next, SLOT ( setEnabled ( bool ) ) );
	connect ( connectPg, SIGNAL ( connecting ( bool ) ), next, SLOT ( setDisabled ( bool ) ) );
	connect ( wifikeyPg->key, SIGNAL ( textChanged ( QString ) ), this, SLOT ( nextEnable ( QString ) ) );
	connect ( ipinputPg->input, SIGNAL ( textChanged ( QString ) ), this, SLOT ( nextEnable ( QString ) ) );
	connect ( netminputPg->input, SIGNAL ( textChanged ( QString ) ), this, SLOT ( nextEnable ( QString ) ) );
	connect ( gateinputPg->input, SIGNAL ( textChanged ( QString ) ), this, SLOT ( nextEnable ( QString ) ) );
	connect ( nameserv1Pg->input, SIGNAL ( textChanged ( QString ) ), this, SLOT ( nextEnable ( QString ) ) );
	connect ( nameserv2Pg->input, SIGNAL ( textChanged ( QString ) ), this, SLOT ( nextEnable ( QString ) ) );

	selectedAP = new QMap<QString, QVariant>;
}


void Wizard::nextEnable ( QString in )
{
	if ( !in.isEmpty() ) {
		next->setEnabled ( true );
	}  else {
		next->setEnabled ( false );
	}
}


void Wizard::doNext()
{
	switch ( pages->currentIndex() ) {

		case 0: //introPg
			previous->setEnabled ( true );
			previous->setHidden ( false );
			next->setEnabled ( false );
			//disconnect( introPg->acceptDeal, SIGNAL(toggled(bool)), next, SLOT(setEnabled(bool)) );
			pages->setCurrentWidget ( scanPg );

			break;

		case 1: //scanPg
			scanPg->getAPData ( selectedAP );

			if ( scanPg->securedap ) {
				wifikeyPg->setLabel ( scanPg );
				pages->setCurrentWidget ( wifikeyPg );
				wifikeyPg->key->setFocus();
				next->setEnabled ( false );
				next->setDefault ( true );
			} else {
				//showcfgPg->getAPInfo ( scanPg );
				pages->setCurrentWidget ( cfgselectPg );
			}

			break;

		case 2: //wifikeyPg
			wifikeyPg->saveKey ( scanPg );
			pages->setCurrentWidget ( cfgselectPg );

			break;

		case 3: //cfgselectPg
			cfgselectPg->setAllocation ( scanPg );
			if ( cfgselectPg->radio1->isChecked() ) {
				next->setText ( tr ( "Finish" ) );
				showcfgPg->getAPInfo ( scanPg );
				showcfgPg->writeCfg ( scanPg );
				pages->setCurrentWidget ( showcfgPg );
			} else {
				pages->setCurrentWidget ( ipinputPg );
				ipinputPg->input->setFocus();
				next->setEnabled ( false );
				next->setDefault ( true );
			}

			break;

		case 4: //ipinputPg
			ipinputPg->saveInput ( scanPg, "ip" );
			pages->setCurrentWidget ( netminputPg );
			netminputPg->input->setFocus();
			next->setEnabled ( false );
			next->setDefault ( true );

			break;

		case 5: //netminputPg
			netminputPg->saveInput ( scanPg, "netmask" );
			pages->setCurrentWidget ( gateinputPg );
			gateinputPg->input->setFocus();
			next->setEnabled ( false );
			next->setDefault ( true );

			break;

		case 6: //gateinputPg
			gateinputPg->saveInput ( scanPg, "gateway" );
			pages->setCurrentWidget ( nameserv1Pg );
			nameserv1Pg->input->setFocus();
			next->setEnabled ( false );
			next->setDefault ( true );

			break;

		case 7: //nameserv1Pg
			nameserv1Pg->saveInput ( scanPg, "nameserver1" );
			pages->setCurrentWidget ( nameserv2Pg );
			nameserv2Pg->input->setFocus();
			next->setEnabled ( false );
			next->setDefault ( true );

			break;

		case 8: //nameserv2Pg
			nameserv2Pg->saveInput ( scanPg, "nameserver2" );
			showcfgPg->getAPInfo ( scanPg );
			showcfgPg->writeCfg ( scanPg );

			pages->setCurrentWidget ( showcfgPg );

			break;
		case 9: //showcfgPg

			next->setText ( tr ( "Finish" ) );
			pages->setCurrentWidget ( connectPg );
			connectPg->doConnect();
			break;

		case 10: //connectPg

			//QMessageBox* msgBox = new QMessageBox ( QMessageBox::Information, "" , "This is the text", QMessageBox::Ok, this, Qt::Dialog | Qt::CustomizeWindowHint );
			//msgBox->exec();
			
			accept();

			return;
	}
	// linear page order sets currentindex+1 here!!
	//pages->setCurrentIndex( pages->currentIndex()+1 );
}


void Wizard::doPrev()
{
	switch ( pages->currentIndex() ) {

		case 1: //scanPg
			previous->setEnabled ( false );
			previous->setHidden ( true );
			//next->setEnabled( introPg->acceptDeal->isChecked() );
			//connect( introPg->acceptDeal, SIGNAL(toggled(bool)), next, SLOT(setEnabled(bool)) );
			pages->setCurrentWidget ( introPg );

			break;

		case 2: //wifikeyPg
			pages->setCurrentWidget ( scanPg );

			break;

		case 3: //cfgselectPg
			if ( scanPg->securedap ) {
				pages->setCurrentWidget ( wifikeyPg );
				wifikeyPg->key->setFocus();
				next->setDefault ( true );
			} else {
				pages->setCurrentWidget ( scanPg );
			}

			break;

		case 4: //ipinputPg
			pages->setCurrentWidget ( cfgselectPg );

			break;

		case 5: //netminputPg
			ipinputPg->input->setFocus();
			next->setDefault ( true );
			pages->setCurrentWidget ( ipinputPg );

			break;

		case 6: //gateinputPg
			netminputPg->input->setFocus();
			next->setDefault ( true );
			pages->setCurrentWidget ( netminputPg );

			break;

		case 7: //nameserv1Pg
			gateinputPg->input->setFocus();
			next->setDefault ( true );
			pages->setCurrentWidget ( gateinputPg );

			break;
		case 8: //nameserv2Pg
			nameserv1Pg->input->setFocus();
			next->setDefault ( true );
			pages->setCurrentWidget ( nameserv1Pg );

			break;

		case 9: //showcfgPg
			if ( cfgselectPg->radio1->isChecked() ) {
				pages->setCurrentWidget ( cfgselectPg );
			} else {
				pages->setCurrentWidget ( nameserv2Pg );
				nameserv2Pg->input->setFocus();
				next->setDefault ( true );
			}

			break;

		case 10: //connectPg

			next->setText ( tr ( "Next >" ) );
			pages->setCurrentWidget ( showcfgPg );
			break;
	}
	//linear page order sets currentindex-1 here!!
	//pages->setCurrentIndex( pages->currentIndex()-1 );
}
