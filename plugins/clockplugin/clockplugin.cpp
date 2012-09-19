#include "clockplugin.h"

#include "clockwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QTime>
#include <QtCore/QTimer>

QString Msg::ClockPlugin::backlight_file = "/sys/devices/platform/stmp3xxx-bl/backlight/stmp3xxx-bl/brightness";

Msg::ClockPlugin::~ClockPlugin()
{
}

std::string Msg::ClockPlugin::getName()
{
    return "Clock";
}

QWidget* Msg::ClockPlugin::getWidget()
{
    QWidget* widget = new ClockWidget(0, this);

    maxBrightness = getMaxBrightness();

    QSettings* settings = new QSettings("/mnt/usb/night.conf", QSettings::NativeFormat);
    bright = settings->value("bright", QVariant(0.7)).toDouble();
    dark = settings->value("dark", QVariant(0.4)).toDouble();

    connect(widget, SIGNAL(destroyed()), this, SLOT(brighten()));

    return widget;
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
    setBrightness(dark*maxBrightness);
}

void Msg::ClockPlugin::brighten()
{
    qDebug() << "brighten";
        _dimmed = false;
    setBrightness(bright*maxBrightness);
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
