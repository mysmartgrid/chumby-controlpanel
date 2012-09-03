#ifndef NIGHTWIDGET_H
#define NIGHTWIDGET_H

#include "nightplugin.h"
#include "alarmform.h"

#include <QtGui/QWidget>

#include <QtCore/QTimer>

namespace Ui {
class NightWidget;
}

class NightWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit NightWidget(QWidget *parent = 0, Msg::NightPlugin *plugin = 0);
    ~NightWidget();
    
protected slots:
		void updateClock();
		void dimAction();
		
private:
		QTimer *_timer;
		
    Ui::NightWidget *_ui;
		Msg::NightPlugin *_plugin;
		AlarmForm *_alarm;
};

#endif // NIGHTWIDGET_H
