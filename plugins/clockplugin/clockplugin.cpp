#include "clockplugin.h"

#include "clockwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include "musiccontrol.h"

QString Msg::ClockPlugin::backlight_file = "/sys/devices/platform/stmp3xxx-bl/backlight/stmp3xxx-bl/brightness";

Msg::ClockPlugin::ClockPlugin() :
    _settings(new QSettings("/mnt/usb/night.conf", QSettings::NativeFormat)),
    //_bright(_settings->value("bright", QVariant(1.0)).toDouble()),
    //_dark(_settings->value("dark", QVariant(0.4)).toDouble()),
    //_sleep(new QTimer(_settings->value("sleep", QVariant(0)).toInt())),
    _sleep(new QTimer()),
    _dimmed(getBrightness() < getMaxBrightness())
{
    _bright = _settings->value("bright", QVariant(1.0)).toDouble();
    _dark = _settings->value("dark", QVariant(0.4)).toDouble();
    _sleep->setInterval(_settings->value("sleep", QVariant(0)).toInt() * 60 * 1000);
    connect(_sleep, SIGNAL(timeout()), this, SLOT(sleep()));
}

Msg::ClockPlugin::~ClockPlugin()
{
}

std::string Msg::ClockPlugin::getName()
{
    return "Clock";
}

QWidget* Msg::ClockPlugin::getWidget()
{
    if ( _widget == NULL )
        _widget = new ClockWidget(0, this);

    _maxBrightness = getMaxBrightness();

    connect(_widget, SIGNAL(destroyed()), this, SLOT(brighten()));

    return _widget;
}

int Msg::ClockPlugin::getMaxBrightness()
{
    QFile* file = new QFile(backlight_file);
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    int max_backlight = file->readLine().trimmed().toInt();
    file->close();
    qDebug() << max_backlight;

    return max_backlight;
}

int Msg::ClockPlugin::getBrightness()
{
    QFile* file = new QFile(backlight_file);
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    int backlight = file->readLine().trimmed().toInt();
    file->close();
    qDebug() << backlight;

    return backlight;
}

void Msg::ClockPlugin::setBrightness(int brightness)
{
    QFile* file = new QFile(backlight_file);
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    //int backlight = file->readLine().trimmed().toInt();
    file->write(QString::number(brightness).toAscii());
    file->close();
}

void Msg::ClockPlugin::dim()
{
    qDebug() << "dim";
        _dimmed = true;
    setBrightness(_dark*_maxBrightness);

    if ( _sleep->interval() > 0 )
    {
        _sleep->start();
        connect(_sleep, SIGNAL(timeout()), _sleep, SLOT(stop()));
    }
}

void Msg::ClockPlugin::brighten()
{
    qDebug() << "brighten";
        _dimmed = false;
    setBrightness(_bright*_maxBrightness);
}

void Msg::ClockPlugin::sleep()
{
    Msg::MusicControl::getInstance().stop();
}

bool Msg::ClockPlugin::isDimmed()
{
    return _dimmed;
}

Msg::Plugin* Msg::ClockPluginFactory::CreatePlugin()
{
    return new Msg::ClockPlugin;
}

extern "C" void * factory0( void )
{
        return new Msg::ClockPluginFactory;
}
