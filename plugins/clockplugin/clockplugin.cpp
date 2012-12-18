#include "clockplugin.h"

#include "clockwidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QPalette>
#include <QtGui/QApplication>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include "musiccontrol.h"

Msg::ClockPlugin::ClockPlugin() :
    _bldir(new QDir("/sys/class/backlight", "*", QDir::SortFlags(QDir::Name|QDir::IgnoreCase), QDir::AllEntries | QDir::NoDotAndDotDot)),
    _settings(new QSettings("/mnt/usb/night.conf", QSettings::NativeFormat)),
    _dimmed(getBrightness() < getMaxBrightness()),
    _sleep(new QTimer())
{
    qDebug() << "Backlight directory" << _bldir->absolutePath();
    _bright = _settings->value("bright", QVariant(1.0)).toDouble();
    _dark = _settings->value("dark", QVariant(0.4)).toDouble();
    brighten();
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
    if ( _bldir->entryList().isEmpty() )
        return 0;

    QFile* file = new QFile(_bldir->absolutePath() + "/" + _bldir->entryList().first() + "/max_brightness");
    qDebug() << "backlight_file" << file->fileName();
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    int max_backlight = file->readLine().trimmed().toInt();
    file->close();
    delete file;
    qDebug() << "max_backlight" << max_backlight;

    return max_backlight;
}

int Msg::ClockPlugin::getBrightness()
{
    if ( _bldir->entryList().isEmpty() )
        return 0;

    QFile* file = new QFile(_bldir->absolutePath() + "/" + _bldir->entryList().first() + "/brightness");
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    int backlight = file->readLine().trimmed().toInt();
    file->close();
    delete file;
    qDebug() << backlight;

    return backlight;
}

void Msg::ClockPlugin::setBrightness(int brightness)
{
    if ( _bldir->entryList().isEmpty() )
        return;

    QFile* file = new QFile(_bldir->absolutePath() + "/" + _bldir->entryList().first() + "/brightness");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    file->write(QString::number(brightness).toAscii());
    file->close();
    delete file;
}

void Msg::ClockPlugin::dim()
{
    qDebug() << "dim";
        _dimmed = true;
    setBrightness(_dark*_maxBrightness);
    if ( _widget )
    {
        QPalette palette = _widget->palette();
        palette.setColor(QPalette::Background, QColor(40, 40, 40));
        palette.setColor(QPalette::WindowText, QColor("gray"));
        palette.setColor(QPalette::ButtonText, QColor("gray"));
        palette.setColor(QPalette::Button, QColor(40, 40, 40));
        _widget->setPalette(palette);
    }

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

    if ( _widget )
    {
        QPalette palette = QApplication::palette();
        _widget->setPalette(palette);
    }
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
