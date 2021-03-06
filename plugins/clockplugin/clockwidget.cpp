#include "clockwidget.h"
#include "ui_clockwidget.h"

#include <QtCore/QTime>

ClockWidget::ClockWidget(QWidget *parent, Msg::ClockPlugin *plugin) :
    QWidget(parent),
    _timer(new QTimer),
    _ui(new Ui::ClockWidget),
    _plugin(plugin),
    _alarm(new AlarmForm)
{
    _ui->setupUi(this);
		
		connect(_timer, SIGNAL(timeout()), this, SLOT(updateClock()));
		updateClock();
		_timer->start();
		
		connect(_ui->dimButton, SIGNAL(clicked()), this, SLOT(dimAction()));
		connect(_ui->alarmButton, SIGNAL(clicked()), _alarm, SLOT(showFullScreen()));
}

ClockWidget::~ClockWidget()
{
    delete _ui;
}

void ClockWidget::updateClock()
{
	QTime now = QTime::currentTime();
	_ui->timeLabel->setText(now.toString("hh:mm"));
	
	_timer->setInterval(((60 - now.second()) * 1000) + (1000 - now.msec()));
}

void ClockWidget::dimAction()
{
	if ( _plugin->isDimmed() )
	{
		_plugin->brighten();
	} else {
		_plugin->dim();
	}
}
