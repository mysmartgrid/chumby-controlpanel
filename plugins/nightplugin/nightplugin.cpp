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

#if 0
Msg::NightWidget::NightWidget(QObject* parent)
{
    QVBoxLayout* layout = new QVBoxLayout();

    timeLabel = new QLabel(QString("00:00"));
    updateClock();

    QTimer* timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));
    timer->start();
    //TODO: update clock <- same as controlpanel

    QPushButton* dimmer = new QPushButton("Dim");
    connect(dimmer, SIGNAL(clicked()), parent, SLOT(dim()));

    QPushButton* brightener = new QPushButton("Brighten");
    connect(brightener, SIGNAL(clicked()), parent, SLOT(brighten()));

    layout->addWidget(timeLabel);
    layout->addWidget(dimmer);
    layout->addWidget(brightener);

    setLayout(layout);
}

void Msg::NightWidget::updateClock()
{
    QTime time = QTime::currentTime();
    timeLabel->setText(time.toString("hh:mm"));
}

void Msg::NightWidget::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
}
#endif

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
