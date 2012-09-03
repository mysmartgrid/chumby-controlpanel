#ifndef __WIZARD_H__
#define __WIZARD_H__

#include <QDialog>

class QPushButton;
class QStackedWidget;
class QCheckBox;
class QListWidget;
class QProcess;
class QProgressBar;
class QListWidgetItem;
class QTextEdit;
class QLabel;
class QLineEdit;
class QRadioButton;

//class IntroPage;
//class ScanPage;
//class ShowCfgPage;
//class CfgSelectPage;
//class WifiKeyPage;

//-----------------
class IntroPage : public QWidget
{
	public:
		IntroPage ( QWidget* parent = 0 );

};

//---------------
class ScanPage : public QWidget
{
		Q_OBJECT

	public:
		ScanPage ( QWidget* parent = 0 );
		QPushButton* scanbtn;
		QListWidget* aplist;
		QMap<QString, QVariant>* selectedap;
		bool securedap;

		const QListWidgetItem* getListSelection();
		void getAPData( QMap<QString, QVariant>* );

	private:
		bool scanrun;
		QProcess* scanscript;
		QProgressBar* bar;

	private slots:
		void scanAP();
		void parseAP();
		void enableNext ( QListWidgetItem* );

	signals:
		void scanning ( bool );
		void selected ( bool );
};

//---------------------
class WifiKeyPage : public QWidget
{
		Q_OBJECT
	public:
		WifiKeyPage ( QWidget* parent = 0 );
		void setLabel ( ScanPage* );
		void saveKey ( ScanPage* );
		QLineEdit* key;

	private:
		QLabel* lab;

		//void getAPInfo(ScanPage *page);
//private:
		//QTextEdit *textEdit;
};

//---------------------
class CfgSelectPage : public QWidget
{
	public:
		CfgSelectPage ( QWidget* parent = 0 );
		QRadioButton* radio1;
		QRadioButton* radio2;
		void setAllocation ( ScanPage* );

};

//---------------------
class InputPage : public QWidget
{
	public:
		InputPage ( QString, QWidget* parent = 0 );
		QLineEdit* input;
		void saveInput ( ScanPage*, QString );

};
//---------------------
class ShowCfgPage : public QWidget
{
	public:
		ShowCfgPage ( QWidget* parent = 0 );
		void getAPInfo ( ScanPage* );
		void writeCfg ( ScanPage* );

	private:
		QTextEdit* textEdit;
		//QTextEdit* textEdit2;
};

//-----------------------
class ConnectionTest : public QWidget
{
	Q_OBJECT
	
	public:
	ConnectionTest ( QWidget* parent = 0 );
	//QPushButton* scanbtn;
	QLabel* title;
		
	private:
	bool testrun;
	QTextEdit* result;
	QProcess* connectscript;
	QProcess* checkscript;
	
	public slots:
	void doConnect();
	void checkStatus();
	void parseStatus();
	//void enbtn();
	void err();
	
	signals:
	void connecting( bool );
	
};

//----------------------
class Wizard : public QDialog
{
		Q_OBJECT

	public:
		Wizard();
		QMap<QString, QVariant>* selectedAP;

	private slots:
		void doNext();
		void doPrev();
		void nextEnable ( QString );

	private:
		QPushButton* next;
		QPushButton* previous;

		QStackedWidget* pages;

		IntroPage* introPg;
		ScanPage* scanPg;
		ShowCfgPage* showcfgPg;
		WifiKeyPage* wifikeyPg;
		CfgSelectPage* cfgselectPg;
		InputPage* ipinputPg;
		InputPage* netminputPg;
		InputPage* gateinputPg;
		InputPage* nameserv1Pg;
		InputPage* nameserv2Pg;
		ConnectionTest* connectPg;
};

#endif // __WIZARD_H__
