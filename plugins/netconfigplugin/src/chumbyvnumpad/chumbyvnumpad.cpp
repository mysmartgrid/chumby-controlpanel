#include "chumbyvnumpad.h"

#define defaultWidth    320
#define defaultHeight   180


ChumbyVirtualNumpad::ChumbyVirtualNumpad ( QWidget* parent ) : QWidget ( parent )
{
	parentWidget = parent;      // save pointer to the parent widget
	createButtons();            // call method that actually creates the pushbuttons
	this->setMinimumSize ( 310, 96 );

	// initialize the signal mapper for the clicked()-signal on the char buttons
	signalMapper = new QSignalMapper ( this );

	for ( int i = 0; i < 16; i++ ) {
		signalMapper->setMapping ( button[i], i );
		connect ( button[i], SIGNAL ( clicked() ), signalMapper, SLOT ( map() ) );
	}
	// connect signalmapper "out" to method handling clicked() signal
	connect ( signalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( sendChar ( int ) ) );
	qDebug ( "first sig mapper done..." );
	// initialize the signal mapper for the pressed()-signal on the char buttons
	signalMapper2 = new QSignalMapper ( this );

	for ( int i = 0; i < 16; i++ ) {
		signalMapper2->setMapping ( button[i], i );
		connect ( button[i], SIGNAL ( pressed() ), signalMapper2, SLOT ( map() ) );
	}
	// connect signalmapper2 "out" to method handling pressed() signal
	connect ( signalMapper2, SIGNAL ( mapped ( int ) ), this, SLOT ( buttonPressed ( int ) ) );
	qDebug ( "second sig mapper done..." );

	// connect button signals (buttons with special functions)
	connect ( buttonC1, SIGNAL ( clicked() ), this, SLOT ( buttonC1Function() ) );
	connect ( buttonC2, SIGNAL ( clicked() ), this, SLOT ( buttonC2Function() ) );
	connect ( buttonC3, SIGNAL ( clicked() ), this, SLOT ( buttonC3Function() ) );
	qDebug ( "special button sig mapper done..." );

}


void ChumbyVirtualNumpad::createButtons ( void )
{
// default geometry
	kw = defaultWidth;
	kh = defaultHeight;
	this->setGeometry ( QRect ( 0, 0, kw, kh / 2 ) );
	this->setMinimumSize ( 50, 50 );

// background label
	backgroundLabel = new QLabel ( "", this );
	backgroundLabel->setStyleSheet ( QString::fromUtf8 ( "background-color:rgb(105, 105, 105);" ) );
	backgroundLabel->setGeometry ( QRect ( 0, 0, kw, kh / 2 ) );
	backgroundLabel->show();

// create 15 pushbuttons, store them in a QVector
	for ( int i = 0; i < 16; i++ ) {
		button.append ( new QPushButton ( ".", this ) );
		button[i]->setFocusPolicy ( Qt::NoFocus );  // set for no focus..
	}
	qDebug ( "button vector created..." );
// create main layout widget
	verticalLayoutWidget = new QWidget ( this );
	verticalLayoutWidget->setGeometry ( QRect ( 0, 0, kw, kh / 2 ) );

// create the main layout
	mainVerticalLayout = new QVBoxLayout ( verticalLayoutWidget );
	mainVerticalLayout->setSpacing ( 0 );
	mainVerticalLayout->setContentsMargins ( 0, 0, 0, 0 );

	horizontalLayout_1 = new QHBoxLayout();
	for ( int i = 0; i < 7; i++ ) {
		horizontalLayout_1->addWidget ( button[i] );
	}
	mainVerticalLayout->addLayout ( horizontalLayout_1 );
	qDebug ( "layout 1 done..." );

	horizontalLayout_2 = new QHBoxLayout();
	for ( int i = 7; i < 14; i++ ) {
		horizontalLayout_2->addWidget ( button[i] );
	}
	mainVerticalLayout->addLayout ( horizontalLayout_2 );
	qDebug ( "layout 2 done..." );

	horizontalLayout_3 = new QHBoxLayout();
	for ( int i = 14; i < 16; i++ ) {
		horizontalLayout_3->addWidget ( button[i] );
		button[i]->setMaximumWidth ( ( int ) this->size().width() / 7 );
		qDebug ( "button size soll: %d", ( int ) this->size().width() / 7 );
	}

	horizontalSpacer_2 = new QSpacerItem ( 4, 10, QSizePolicy::Fixed, QSizePolicy::Minimum );
	horizontalLayout_3->addItem ( horizontalSpacer_2 );

	buttonC1 = new QPushButton ( QIcon ( ":/icon/ressource/delArrow.png" ), " ", this ); //del-button
	buttonC1->setFocusPolicy ( Qt::NoFocus );
	buttonC1->setMaximumWidth ( ( int ) this->size().width() / 6 );
	horizontalLayout_3->addWidget ( buttonC1 );

	buttonC2 = new QPushButton ( "Space", this );
	buttonC2->setFocusPolicy ( Qt::NoFocus );
	horizontalLayout_3->addWidget ( buttonC2 );

	horizontalSpacer = new QSpacerItem ( 4, 10, QSizePolicy::Fixed, QSizePolicy::Minimum );
	horizontalLayout_3->addItem ( horizontalSpacer );

	buttonC3 = new QPushButton ( "Enter", this );
	buttonC3->setFocusPolicy ( Qt::NoFocus );
	horizontalLayout_3->addWidget ( buttonC3 );

	mainVerticalLayout->addLayout ( horizontalLayout_3 );
	qDebug ( "layout 3 done..." );

// create a hidden label for displaying the pressed buttons
	labelDisplayChar = new QPushButton ( "", parentWidget );
	labelDisplayChar->hide();

// insert numbers "into" created buttons
	displayNumber();

	return;
}


void ChumbyVirtualNumpad::displayNumber ( void )
{
	qDebug ( "Display numbers..." );
	//unsigned short int number[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '/', ':', ';', '(', ')', 0x20ac, '&', '@', '"', '.', ',', '?', '!', 39, 0x00B2};
	//const char *values[18] = { "1", "2", "3", "4", "5", ".", ",", "6", "7", "8", "9", "0", "DEL", "ENT", "/", "+", "-", " " };
	//unsigned short int numkeys = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', };
	QString numkeys = "12345,.67890+-/*";

	for ( int i = 0; i < 16; i++ ) {
		button[i]->setText ( QChar ( numkeys[i] ) );
	}

	return;
}


void ChumbyVirtualNumpad::buttonC1Function ( void )
{
	QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, QString ( QChar ( 0x0008 ) ) ) );
	return;
}


void ChumbyVirtualNumpad::buttonC2Function ( void )
{
	QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, ' ', Qt::NoModifier, QString ( " " ) ) );
	return;
}


void ChumbyVirtualNumpad::buttonC3Function ( void )
{
	QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier, QString ( QChar ( 13 ) ) + QString ( QChar ( 13 ) ) ) );
	return;
}


// method that seds pressed char to widget with focus
void ChumbyVirtualNumpad::sendChar ( int indexOfCharToSend )
{
	QChar charToSend ( button[indexOfCharToSend]->text().at ( 0 ) );

	QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, charToSend.unicode(), Qt::NoModifier, QString ( charToSend ) ) );

	labelDisplayChar->hide();
	return;
}


void ChumbyVirtualNumpad::buttonPressed ( int indexOfCharToSend )
{
	labelDisplayChar->setText ( QString ( button[indexOfCharToSend]->text() ) );

	labelDisplayChar->setEnabled ( false );

	QFont font;
	font.setPointSize ( 14 );
	font.setBold ( true );
	font.setWeight ( 75 );
	labelDisplayChar->setFont ( font );

	QRect pos ( button[indexOfCharToSend]->x() + button[indexOfCharToSend]->width() / 2 - 32 / 2 + this->x(), button[indexOfCharToSend]->y() - 3 - 32 + this->y(), 30, 30 );
	labelDisplayChar->setGeometry ( pos );
	labelDisplayChar->setStyleSheet ( QString::fromUtf8 ( "background-color:rgb(50, 100, 255); color:#ffffff;" ) );
	labelDisplayChar->setAutoFillBackground ( false );
	labelDisplayChar->show();

	return;
}


void ChumbyVirtualNumpad::resizeEvent ( QResizeEvent * event )
{
	event->accept();

	kw = event->size().width();
	kh = event->size().height();

	backgroundLabel->resize ( kw, kh );
	verticalLayoutWidget->resize ( kw, kh );

	buttonC3->setMaximumSize ( QSize ( kw*25 / 100, 16777215 ) );

	return;
}


void ChumbyVirtualNumpad::mouseReleaseEvent ( QMouseEvent* event )
{
	event->accept();
	labelDisplayChar->hide();

	return;
}
