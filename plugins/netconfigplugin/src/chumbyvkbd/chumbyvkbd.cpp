#include "chumbyvkbd.h"

#define defaultWidth    320
#define defaultHeight   180


ChumbyVirtualKeyboard::ChumbyVirtualKeyboard ( QWidget* parent ) : QWidget ( parent )
{
	parentWidget = parent;      // save pointer to the parent widget
	createButtons();            // call method that actually creates the pushbuttons
	this->setMinimumSize ( 300, 120 );


	// initialize the signal mapper for the clicked()-signal on the char buttons
	signalMapper = new QSignalMapper ( this );

	for ( int i = 0; i < 26; i++ ) {
		signalMapper->setMapping ( button[i], i );
		connect ( button[i], SIGNAL ( clicked() ), signalMapper, SLOT ( map() ) );
	}
	// connect signalmapper "out" to method handling clicked() signal
	connect ( signalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( sendChar ( int ) ) );

	// initialize the signal mapper for the pressed()-signal on the char buttons
	signalMapper2 = new QSignalMapper ( this );

	for ( int i = 0; i < 26; i++ ) {
		signalMapper2->setMapping ( button[i], i );
		connect ( button[i], SIGNAL ( pressed() ), signalMapper2, SLOT ( map() ) );
	}
	// connect signalmapper2 "out" to method handling pressed() signal
	connect ( signalMapper2, SIGNAL ( mapped ( int ) ), this, SLOT ( buttonPressed ( int ) ) );

	// initialize the signal mapper for the clicked()-signal on the char buttons (umlaute)
	OtherCharSignalMapper = new QSignalMapper ( this );

	for ( int i = 0; i < 10; i++ ) {
		OtherCharSignalMapper->setMapping ( OtherCharButton[i], i );
		connect ( OtherCharButton[i], SIGNAL ( clicked() ), OtherCharSignalMapper, SLOT ( map() ) );
	}
	// connect "other" signal mapper "out" to method handling clicked() signal
	connect ( OtherCharSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( sendOtherChar ( int ) ) );

	// connect button signals (buttons with special functions)
	connect ( buttonC1, SIGNAL ( clicked() ), this, SLOT ( buttonC1Function() ) );
	connect ( buttonC2, SIGNAL ( clicked() ), this, SLOT ( buttonC2Function() ) );
	connect ( buttonC3, SIGNAL ( clicked() ), this, SLOT ( buttonC3Function() ) );
	connect ( buttonC4, SIGNAL ( clicked() ), this, SLOT ( buttonC4Function() ) );
	connect ( buttonC5, SIGNAL ( clicked() ), this, SLOT ( buttonC5Function() ) );
	connect ( buttonC6, SIGNAL ( clicked() ), this, SLOT ( buttonC6Function() ) );

	// connect timer that triggers the display of pressed key
	connect ( pressTimer, SIGNAL ( timeout() ), this, SLOT ( displayOtherChar() ) );
}


void ChumbyVirtualKeyboard::createButtons ( void )
{
	// default layout: QWERTY (or AZERTY with "1")
	AlphabetLayout = 0;

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

// create 26 pushbuttons, store them in a QVector
	for ( int i = 0; i < 26; i++ ) {
		button.append ( new QPushButton ( ".", this ) );
		button[i]->setFocusPolicy ( Qt::NoFocus );  // set for no focus..
	}

// create main layout widget
	verticalLayoutWidget = new QWidget ( this );
	verticalLayoutWidget->setGeometry ( QRect ( 0, 0, kw, kh / 2 ) );

// create the main layout
	mainVerticalLayout = new QVBoxLayout ( verticalLayoutWidget );
	mainVerticalLayout->setSpacing ( 0 );
	mainVerticalLayout->setContentsMargins ( 0, 0, 0, 0 );

// layout "line" 1 - insert first 9 buttons
	horizontalLayout_1 = new QHBoxLayout();

	for ( int i = 0; i < 10; i++ ) {
		horizontalLayout_1->addWidget ( button[i] );
	}

	mainVerticalLayout->addLayout ( horizontalLayout_1 );

// layout "line" 2 - insert another 10 buttons
	horizontalLayout_2 = new QHBoxLayout();

	for ( int i = 10; i < 20; i++ ) {
		horizontalLayout_2->addWidget ( button[i] );
	}

	mainVerticalLayout->addLayout ( horizontalLayout_2 );

// layout "line" 3 - insert remaining 6 buttons plus the special--function buttons
	horizontalLayout_3 = new QHBoxLayout();

	buttonC1 = new QPushButton ( this ); // caps-lock key (shift)
	buttonC1->setFocusPolicy ( Qt::NoFocus );
	buttonC1->setText ( QString ( " " ) );
	buttonC1->setIcon ( QIcon ( ":/icon/ressource/arrow.png" ) );
	buttonC1->setCheckable ( true );

	horizontalLayout_3->addWidget ( buttonC1 );

	horizontalSpacer = new QSpacerItem ( 4, 10, QSizePolicy::Fixed, QSizePolicy::Minimum );
	horizontalLayout_3->addItem ( horizontalSpacer );

	// insert the 6 buttons...
	for ( int i = 20; i < 26; i++ ) {
		horizontalLayout_3->addWidget ( button[i] );
	}

	horizontalSpacer_2 = new QSpacerItem ( 4, 10, QSizePolicy::Fixed, QSizePolicy::Minimum );
	horizontalLayout_3->addItem ( horizontalSpacer_2 );

	buttonC2 = new QPushButton ( QIcon ( ":/icon/ressource/delArrow.png" ), " ", this ); //del-button
	buttonC2->setFocusPolicy ( Qt::NoFocus );
	horizontalLayout_3->addWidget ( buttonC2 );

	mainVerticalLayout->addLayout ( horizontalLayout_3 );

// layout line 4 - insert another set of special-function buttons
	horizontalLayout_4 = new QHBoxLayout();

	buttonC3 = new QPushButton ( this );
	buttonC3->setFocusPolicy ( Qt::NoFocus );
	buttonC3->setMaximumSize ( QSize ( kw*19 / 100, 16777215 ) );
	buttonC3->setText ( QString ( ".?123" ) );

	horizontalLayout_4->addWidget ( buttonC3 );

	buttonC4 = new QPushButton ( "Space", this );
	buttonC4->setFocusPolicy ( Qt::NoFocus );
	horizontalLayout_4->addWidget ( buttonC4 );

	buttonC5 = new QPushButton ( "Hide", this );
	buttonC5->setFocusPolicy ( Qt::NoFocus );
	buttonC5->setMaximumSize ( QSize ( kw*19 / 100, 16777215 ) );
	horizontalLayout_4->addWidget ( buttonC5 );

	buttonC6 = new QPushButton ( "Enter", this );
	buttonC6->setFocusPolicy ( Qt::NoFocus );
	buttonC6->setMaximumSize ( QSize ( kw*19 / 100, 16777215 ) );
	horizontalLayout_4->addWidget ( buttonC6 );

	mainVerticalLayout->addLayout ( horizontalLayout_4 );

// create a hidden label for displaying the pressed buttons
	labelDisplayChar = new QPushButton ( "", parentWidget );
	labelDisplayChar->hide();

// insert alphabet "into" created buttons
	displayAlphabet();

// initialize buttons with "other" (special) characters - "umlaute"
	for ( int i = 0; i < 10; i++ ) {
		OtherCharButton.append ( new QPushButton ( "", this ) );
		OtherCharButton[i]->setFocusPolicy ( Qt::NoFocus );
		OtherCharButton[i]->hide();
	}

// create timer that triggers the "pressed-key-label"
	pressTimer = new QTimer ( this );

// backround for "other" character buttons
	backgroundOtherCharLabel = new QLabel ( "", parentWidget );
	verticalLayoutWidget2 = new QWidget ( parentWidget );
	horizontalLayout = new QHBoxLayout ( verticalLayoutWidget2 );
	horizontalLayout->setContentsMargins ( 5, 0, 5, 0 );
	horizontalLayout->setSpacing ( 1 );

	return;
}

//TODO: add display for small chars when caps-lock/shift activated...
void ChumbyVirtualKeyboard::displayAlphabet ( void )
{
	if ( AlphabetLayout )  // if AlphabetLayout == 1 -> AZERTY layout
	{
		unsigned char azerty[] = {'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'W', 'X', 'C', 'V', 'B', 'N'};

		if ( !buttonC1->isChecked() ) {
			for ( int i = 0; i < 26; i++ ) {
				azerty[i] = tolower ( azerty[i] );
			}
		}

		for ( int i = 0; i < 26; i++ )
		{
			button[i]->setText ( QChar::fromAscii ( azerty[i] ) );
		}
	}
	else  // else QWERTY layout
	{
		unsigned char qwerty[] = {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M'};

		if ( !buttonC1->isChecked() ) {
			for ( int i = 0; i < 26; i++ ) {
				qwerty[i] = tolower ( qwerty[i] );
			}
		}

		// place button 19 to third layout line
		horizontalLayout_2->removeWidget ( button[19] );
		horizontalLayout_3->insertWidget ( 2, button[19] );

		for ( int i = 0; i < 26; i++ ) {
			button[i]->setText ( QChar::fromAscii ( qwerty[i] ) );
		}
	}

	return;
}


void ChumbyVirtualKeyboard::displayNumber ( void )
{
	if ( !AlphabetLayout ) // if using QWERTY layout, place button 19 to second layout line
	{
		horizontalLayout_3->removeWidget ( button[19] );
		horizontalLayout_2->addWidget ( button[19] );
	}

	unsigned short int number[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '/', ':', ';', '(', ')', 0x20ac, '&', '@', '"', '.', ',', '?', '!', 39, 0x00B2};

	for ( int i = 0; i < 26; i++ ) {
		if ( number[i] != '&' ) {
			button[i]->setText ( QChar ( number[i] ) );
		} else {
			button[i]->setText ( QString ( QChar ( number[i] ) ) + QString ( QChar ( number[i] ) ) ); // for "&" double-insertation needed to work properly
		}
	}

	return;
}


void ChumbyVirtualKeyboard::displaySpecialChar ( void )
{
	//unsigned char SpecialChar[] = {'[', ']', '{', '}', '#', '%', '^', '*', '+', '=', '_', '\\', '|', '~', '<', '>', '$', '£', '¥', '¤', '.', ',', '?', '!', '\'', '°'};
	//char SpecialChar[] = "[]{}#%^*+=_\\|~<>$£¥¤.,?!'°";
	QString SpecialChar = QString::fromUtf8 ( "[]{}#%^*+=_\\|~<>$£¥¤.,?!'°" );
	//QString SpecialChar[] = {'[', ']', '{', '}', '#', '%', '^', '*', '+', '=', '_', '\\', '|', '~', '<', '>', '$', '£', '¥', '¤', '.', ',', '?', '!', '\'', '°'};
	//QByteArray chrs = SpecialChar.toLocal8Bit();

	for ( int i = 0; i < 26; i++ ) {
		//button[i]->setText ( QChar::fromAscii ( SpecialChar2[i] ) );
		//button[i]->setText ( QChar::fromL( SpecialChar[i] ) );
		button[i]->setText ( QChar ( SpecialChar[i] ) );
	}

	return;
}


void ChumbyVirtualKeyboard::buttonC1Function ( void )
{
	if ( buttonC3->text() == QString ( "ABC" ) ) {
		if ( buttonC1->text() == QString ( "#+=" ) ) {
			buttonC1->setText ( QString ( "123" ) );
			buttonC1->setCheckable ( false );
			displaySpecialChar();
		} else {
			buttonC1->setText ( QString ( "#+=" ) );
			buttonC1->setIcon ( QIcon ( "" ) );
			buttonC1->setCheckable ( false );
			displayNumber();
		}

	} else {
		displayAlphabet();
	}

	return;
}


void ChumbyVirtualKeyboard::buttonC2Function ( void )
{
	QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, QString ( QChar ( 0x0008 ) ) ) );
	return;
}


void ChumbyVirtualKeyboard::buttonC3Function ( void )
{
	if ( buttonC3->text() == QString ( ".?123" ) ) {
		buttonC3->setText ( QString ( "ABC" ) );
		buttonC1->setText ( QString ( "#+=" ) );
		buttonC1->setIcon ( QIcon ( "" ) );
		buttonC1->setCheckable ( false );
		displayNumber();
	} else {
		buttonC3->setText ( QString ( ".?123" ) );
		buttonC1->setText ( QString ( " " ) );
		buttonC1->setIcon ( QIcon ( ":/icon/ressource/arrow.png" ) );
		buttonC1->setCheckable ( true );
		displayAlphabet();
	}

	return;
}


void ChumbyVirtualKeyboard::buttonC4Function ( void )
{
	QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, ' ', Qt::NoModifier, QString ( " " ) ) );
	return;
}


void ChumbyVirtualKeyboard::buttonC5Function ( void )
{
	this->hide();
	return;
}


void ChumbyVirtualKeyboard::buttonC6Function ( void )
{
	QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier, QString ( QChar ( 13 ) ) + QString ( QChar ( 13 ) ) ) );
	return;
}


// method that seds pressed char to widget with focus
void ChumbyVirtualKeyboard::sendChar ( int indexOfCharToSend )
{
	pressTimer->stop();

	if ( verticalLayoutWidget2->isVisible() )  // when using "other" characters, leave method here
	{
		return;
	}

	QChar charToSend ( button[indexOfCharToSend]->text().at ( 0 ) );

	if ( !buttonC1->isChecked() && buttonC3->text() == QString ( ".?123" ) ) {
		QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, charToSend.toLower().unicode(), Qt::NoModifier, QString ( charToSend.toLower() ) ) );
	} else {
		QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, charToSend.unicode(), Qt::NoModifier, QString ( charToSend ) ) );
	}


	labelDisplayChar->hide();
	return;
}



void ChumbyVirtualKeyboard::buttonPressed ( int indexOfCharToSend )
{
	if ( buttonC1->isChecked() )  // start timer for special character display...
	{
		pressTimer->start ( 1000 );
	}

	backgroundOtherCharLabel->hide();
	verticalLayoutWidget2->hide();

	lastPressedKey = indexOfCharToSend;

	if ( !buttonC1->isChecked() && buttonC3->text() == QString ( ".?123" ) ) {
		labelDisplayChar->setText ( QString ( button[indexOfCharToSend]->text() ).toLower() );
	} else {
		labelDisplayChar->setText ( QString ( button[indexOfCharToSend]->text() ) );
	}

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


void ChumbyVirtualKeyboard::resizeEvent ( QResizeEvent * event )
{
	event->accept();

	kw = event->size().width();
	kh = event->size().height();

	backgroundLabel->resize ( kw, kh );
	verticalLayoutWidget->resize ( kw, kh );
	buttonC3->setMaximumSize ( QSize ( kw*19 / 100, 16777215 ) );
	buttonC5->setMaximumSize ( QSize ( kw*19 / 100, 16777215 ) );
	buttonC6->setMaximumSize ( QSize ( kw*19 / 100, 16777215 ) );

	return;
}


void ChumbyVirtualKeyboard::displayOtherChar ( void )
{
	QChar pressedKey ( button[lastPressedKey]->text().at ( 0 ) );
	qDebug ( "pressed key: %s", qPrintable ( QString ( pressedKey ) ) );

	QString OtherCharString;

	if ( pressedKey == QChar ( 'A' ) ) {
		OtherCharString = QString::fromLocal8Bit ( "àâäã" );
	} else if ( pressedKey == QChar ( 'C' ) ) {
		OtherCharString = QString::fromLocal8Bit ( "ç" );
	} else if ( pressedKey == QChar ( 'E' ) ) {
		OtherCharString = QString::fromLocal8Bit ( "éèêë" );
	} else if ( pressedKey == QChar ( 'I' ) ) {
		OtherCharString = QString::fromLocal8Bit ( "îï" );
	} else if ( pressedKey == QChar ( 'N' ) ) {
		OtherCharString = QString::fromLocal8Bit ( "ñ" );
	} else if ( pressedKey == QChar ( 'O' ) ) {
		OtherCharString =  QString::fromLocal8Bit ( "ôö" );
	} else if ( pressedKey == QChar ( 'U' ) ) {
		OtherCharString = QString::fromLocal8Bit ( "ùûüµ" );
	} else if ( pressedKey == QChar ( 'Y' ) ) {
		OtherCharString = QString::fromLocal8Bit ( "ÿ" );
	} else {
		return;
	}

	labelDisplayChar->hide();
	pressTimer->stop();

	qDebug ( "charstring size: %d", OtherCharString.size() );
	qDebug ( "chars; %s", qPrintable ( OtherCharString ) );

	int width = this->width() * 6 / 100 * OtherCharString.size();
	if ( width < 4 + 22 * OtherCharString.size() ) {
		width = 4 + 22 * OtherCharString.size();
	}

	backgroundOtherCharLabel->setStyleSheet ( QString::fromUtf8 ( "background-color:rgb(230, 230, 230);border: 2px solid #8f8f91;border-radius: 6px;" ) );
	backgroundOtherCharLabel->setGeometry ( QRect ( button[lastPressedKey]->x() + this->x(), button[lastPressedKey]->y() - 15*button[lastPressedKey]->height() / 10 - 3 + this->y(), width, 15*button[lastPressedKey]->height() / 10 ) );
	backgroundOtherCharLabel->show();

	verticalLayoutWidget2->setGeometry ( QRect ( button[lastPressedKey]->x() + this->x(), button[lastPressedKey]->y() - 15*button[lastPressedKey]->height() / 10 - 3 + this->y(), width, 15*button[lastPressedKey]->height() / 10 ) );
	verticalLayoutWidget2->show();

	for ( int i = 0; i < OtherCharButton.size(); i++ ) {
		horizontalLayout->removeWidget ( OtherCharButton[i] );
		OtherCharButton[i]->hide();
	}

	for ( int i = 0; i < OtherCharString.size(); i++ ) {
		OtherCharButton[i]->setText ( OtherCharString.at ( i ) );
		horizontalLayout->addWidget ( OtherCharButton[i] );
		OtherCharButton[i]->show();
	}

	return;
}


void ChumbyVirtualKeyboard::sendOtherChar ( int indexOfCharToSend )
{
	QChar charToSend ( OtherCharButton[indexOfCharToSend]->text().at ( 0 ) );

	QApplication::sendEvent ( parentWidget->focusWidget(), new QKeyEvent ( QEvent::KeyPress, charToSend.unicode(), Qt::NoModifier, QString ( charToSend ) ) );

	backgroundOtherCharLabel->hide();
	verticalLayoutWidget2->hide();

	return;
}


void ChumbyVirtualKeyboard::mouseReleaseEvent ( QMouseEvent* event )
{
	event->accept();
	labelDisplayChar->hide();

	return;
}

void ChumbyVirtualKeyboard::hideHideBtn ( bool hidesw )
{
	buttonC5->setHidden ( hidesw );

	return;
}

void ChumbyVirtualKeyboard::setKeyboardLayout ( int layout )
{
	AlphabetLayout = layout;

	if ( buttonC3->text() == QString ( ".?123" ) ) {
		displayAlphabet();
	}

	return;
}
