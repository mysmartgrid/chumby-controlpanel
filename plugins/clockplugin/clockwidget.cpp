#include "clockwidget.h"
#include "ui_clockwidget.h"

#include "alarmdaemon.h"

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
    _snoozed(new SnoozedAlarmsPage),
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
        connect(_ui->snoozeButton, SIGNAL(clicked()), _snoozed, SLOT(showFullScreen()));
        connect(_ui->snoozeButton, SIGNAL(clicked()), _snoozed, SLOT(raise()));
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

void ClockWidget::paintEvent(QPaintEvent *)
{
    std::list<Msg::Alarm*> alarms = Msg::AlarmDaemon::getInstance().getAlarms();
    std::list<Msg::Alarm*> snoozed;
    for ( std::list<Msg::Alarm*>::iterator it = alarms.begin(); it != alarms.end(); it++ )
    {
        if ( (*it)->isSnoozed() )
            snoozed.push_back(*it);
    }

    if ( snoozed.empty() ) {
        _ui->snoozeButton->hide();
    } else {
        if ( snoozed.size() == 1 )
            _ui->snoozeButton->setText("1 alarm snoozed");
        else
            _ui->snoozeButton->setText(QString::number(snoozed.size()) + " alarms snoozed");
        _ui->snoozeButton->show();
        _snoozed->setSnoozed(&snoozed);
    }
}

#ifdef Q_WS_QWS
bool ClockWidget::qwsEvent(QWSEvent *event)
{
    if ( event->type == QWSEvent::Focus)
        _plugin->brighten();
    return false;
}
#endif
