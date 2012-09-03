#include "nightwidget.h"
#include "ui_nightwidget.h"

#include <QtCore/QTime>

NightWidget::NightWidget(QWidget *parent, Msg::NightPlugin *plugin) :
    QWidget(parent),
    _timer(new QTimer),
    _ui(new Ui::NightWidget),
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

NightWidget::~NightWidget()
{
    delete _ui;
}

void NightWidget::updateClock()
{
	QTime now = QTime::currentTime();
	_ui->timeLabel->setText(now.toString("hh:mm"));
	
	_timer->setInterval(((60 - now.second()) * 1000) + (1000 - now.msec()));
}

void NightWidget::dimAction()
{
	if ( _plugin->isDimmed() )
	{
		_plugin->brighten();
	} else {
		_plugin->dim();
	}
}
