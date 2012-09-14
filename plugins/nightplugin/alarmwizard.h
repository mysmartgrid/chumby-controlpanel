#ifndef ALARMWIZARD_H
#define ALARMWIZARD_H

#include <QWizard>

#include "../../controlpanel/alarmdaemon.h"

class AlarmWizard : public QWizard
{
    Q_OBJECT
public:
	enum {
		TIMEPAGE,
		DAYPAGE,
		SOURCEPAGE,
		SNOOZEPAGE,
		NAMEPAGE,
		SUMMARYPAGE
	};
    explicit AlarmWizard(QWidget *parent = 0, Msg::Alarm *alarm = 0);
	void accept();
	int getNextPage();
	void addPage(QWizardPage* page);
	QString getSource();
	void setSource(QString source);
    
signals:
    
public slots:
	
private:
    Msg::Alarm* _alarm;
	int _nextPage;
	QString _source;
};

#endif // ALARMWIZARD_H
