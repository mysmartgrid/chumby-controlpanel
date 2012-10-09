#include "clockwidget.h"
#include "ui_clockwidget.h"

#include <QtCore/QTime>

#include <QtCore/QDebug>

#ifdef Q_WS_QWS
    #include <QWSEvent>
#endif

ClockWidget::ClockWidget(QWidget *parent, Msg::ClockPlugin *plugin) :
    QWidget(parent),
    _timer(new QTimer),
    _ui(new Ui::ClockWidget),
    _plugin(plugin),
    _alarm(new AlarmForm),
    _clicked(false)
{
    _ui->setupUi(this);
		
		connect(_timer, SIGNAL(timeout()), this, SLOT(updateClock()));
		updateClock();
		_timer->start();

        connect(this, SIGNAL(clicked()), this, SLOT(dimAction()));
		connect(_ui->alarmButton, SIGNAL(clicked()), _alarm, SLOT(showFullScreen()));
        connect(_ui->alarmButton, SIGNAL(clicked()), _alarm, SLOT(raise()));
        connect(_ui->alarmButton, SIGNAL(clicked()), _alarm, SLOT(refresh()));
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

void ClockWidget::mousePressEvent(QMouseEvent *)
{
    _clicked = true;
    emit pressed();
}

void ClockWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (_clicked)
    {
        _clicked = false;
        emit clicked();
    }
    emit released();
}

void ClockWidget::leaveEvent(QEvent *)
{
    if ( _plugin->isDimmed() )
        _plugin->brighten();
}

#ifdef Q_WS_QWS
bool ClockWidget::qwsEvent(QWSEvent *event)
{
    if ( event->type == QWSEvent::Focus)
        _plugin->brighten();
    return false;
}
#endif
