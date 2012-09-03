#ifndef __CHUMBYVKBD_H__
#define __CHUMBYVKBD_H__

#include <QtGui>

class ChumbyVirtualKeyboard : public QWidget
{
		Q_OBJECT

	public:
		ChumbyVirtualKeyboard ( QWidget* parent = 0 );

		void mouseReleaseEvent ( QMouseEvent* event );
		void resizeEvent ( QResizeEvent* event );
		void setKeyboardLayout ( int layout );
		void hideHideBtn ( bool hidesw );


	private slots:
		void createButtons ( void );

		//void displayAlphabet(void);
		void displayAlphabet ( void );
		void displayNumber ( void );
		void displaySpecialChar ( void );

		void buttonC1Function ( void );
		void buttonC2Function ( void );
		void buttonC3Function ( void );
		void buttonC4Function ( void );
		void buttonC5Function ( void );
		void buttonC6Function ( void );

		void sendChar ( int indexOfCharToSend );
		void buttonPressed ( int indexOfCharToSend );

		void displayOtherChar ( void );
		void sendOtherChar ( int indexOfCharToSend );

	private:
		QWidget* parentWidget;

//buttons and signal mappers
		QSignalMapper* signalMapper;
		QSignalMapper* signalMapper2;

		QVector<QPushButton*> button;

		QPushButton* buttonC1;
		QPushButton* buttonC2;
		QPushButton* buttonC3;
		QPushButton* buttonC4;
		QPushButton* buttonC5;
		QPushButton* buttonC6;

		QPushButton* labelDisplayChar;

		QLabel* backgroundLabel;

		QWidget* verticalLayoutWidget;
		QVBoxLayout* mainVerticalLayout;
		QHBoxLayout* horizontalLayout_1;
		QHBoxLayout* horizontalLayout_2;
		QHBoxLayout* horizontalLayout_3;
		QHBoxLayout* horizontalLayout_4;

		QSpacerItem *horizontalSpacer;
		QSpacerItem *horizontalSpacer_2;

		int kw; // Keyboard custom width
		int kh; // Keyboard custom height


		QTimer* pressTimer;
		int lastPressedKey;
		QVector<QPushButton*> OtherCharButton;
		QSignalMapper* OtherCharSignalMapper;

		QLabel* backgroundOtherCharLabel;

		QWidget* verticalLayoutWidget2;
		QHBoxLayout* horizontalLayout;

		int AlphabetLayout; // AZERTY = 1 ou QWERTY = 0

};

#endif // __CHUMBYVKBD_H__
