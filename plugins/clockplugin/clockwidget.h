#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include "clockplugin.h"
#include "alarmform.h"

#include <QtGui/QWidget>

#include <QtCore/QTimer>

namespace Ui {
class ClockWidget;
}

class ClockWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClockWidget(QWidget *parent = 0, Msg::ClockPlugin *plugin = 0);
    ~ClockWidget();
    
protected slots:
		void updateClock();
		void dimAction();
		
private:
		QTimer *_timer;
		
    Ui::ClockWidget *_ui;
		Msg::ClockPlugin *_plugin;
		AlarmForm *_alarm;
};

#endif // CLOCKWIDGET_H
