#include "nightplugin.h"

#include "nightwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QTime>
#include <QtCore/QTimer>

QString Msg::NightPlugin::backlight_file = "/sys/devices/platform/stmp3xxx-bl/backlight/stmp3xxx-bl/brightness";

Msg::NightPlugin::~NightPlugin()
{
}

std::string Msg::NightPlugin::getName()
{
    return "Night";
}

QWidget* Msg::NightPlugin::getWidget()
{
    QWidget* widget = new NightWidget(0, this);

    maxBrightness = getMaxBrightness();

    QSettings* settings = new QSettings("/mnt/usb/night.conf", QSettings::NativeFormat);
    bright = settings->value("bright", QVariant(0.7)).toDouble();
    dark = settings->value("dark", QVariant(0.4)).toDouble();

    connect(widget, SIGNAL(destroyed()), this, SLOT(brighten()));

    return widget;
}

int Msg::NightPlugin::getMaxBrightness()
{
    QFile* file = new QFile(backlight_file);
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    int max_backlight = file->readLine().trimmed().toInt();
    file->close();
    qDebug() << max_backlight;

    return max_backlight;
}

int Msg::NightPlugin::getBrightness()
{
    QFile* file = new QFile(backlight_file);
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    int backlight = file->readLine().trimmed().toInt();
    file->close();
    qDebug() << backlight;

    return backlight;
}

void Msg::NightPlugin::setBrightness(int brightness)
{
    QFile* file = new QFile(backlight_file);
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    //int backlight = file->readLine().trimmed().toInt();
    file->write(QString::number(brightness).toAscii());
    file->close();
}

void Msg::NightPlugin::dim()
{
    qDebug() << "dim";
        _dimmed = true;
    setBrightness(dark*maxBrightness);
}

void Msg::NightPlugin::brighten()
{
    qDebug() << "brighten";
        _dimmed = false;
    setBrightness(bright*maxBrightness);
}

bool Msg::NightPlugin::isDimmed()
{
    return _dimmed;
}

Msg::Plugin* Msg::NightPluginFactory::CreatePlugin()
{
    return new Msg::NightPlugin;
}

extern "C" void * factory0( void )
{
        return new Msg::NightPluginFactory;
}
