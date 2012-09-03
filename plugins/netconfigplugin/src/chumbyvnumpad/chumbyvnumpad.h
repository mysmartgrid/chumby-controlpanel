#ifndef __CHUMBYVNUMPAD_H__
#define __CHUMBYVNUMPAD_H__

#include <QtGui>

class ChumbyVirtualNumpad : public QWidget
{
		Q_OBJECT

	public:
		ChumbyVirtualNumpad ( QWidget* parent = 0 );

		void mouseReleaseEvent ( QMouseEvent* event );
		void resizeEvent ( QResizeEvent* event );

	private slots:
		void createButtons ( void );

		void displayNumber ( void );

		void buttonC1Function ( void );
		void buttonC2Function ( void );
		void buttonC3Function ( void );

		void sendChar ( int indexOfCharToSend );
		void buttonPressed ( int indexOfCharToSend );

	private:
		QWidget* parentWidget;

//buttons and signal mappers
		QSignalMapper* signalMapper;
		QSignalMapper* signalMapper2;

		QVector<QPushButton*> button;

		QPushButton* buttonC1;
		QPushButton* buttonC2;
		QPushButton* buttonC3;
		QPushButton* labelDisplayChar;

		QLabel* backgroundLabel;

		QWidget* verticalLayoutWidget;
		QVBoxLayout* mainVerticalLayout;
		QHBoxLayout* horizontalLayout_1;
		QHBoxLayout* horizontalLayout_2;
		QHBoxLayout* horizontalLayout_3;

		QSpacerItem* horizontalSpacer;
		QSpacerItem* horizontalSpacer_2;

		int kw; // Keyboard custom width
		int kh; // Keyboard custom height

		QHBoxLayout* horizontalLayout;
};

#endif // __CHUMBYVKBD_H__
