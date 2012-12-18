#include "ringtoneplugin.h"

#include "musiccontrol.h"

#include <QtCore/QDirIterator>
#include <QtCore/QDebug>

std::string Msg::RingTonePlugin::getName()
{
    return "Ringtone";
}

QWidget* Msg::RingTonePlugin::getWidget()
{
    return _widget;
}

bool Msg::RingTonePlugin::play(QString source)
{
    qDebug() << "play" << source;
    Msg::MusicControl::getInstance().play("/usr/chumby/alarmtones/" + source + ".mp3");
    return true;
}

bool Msg::RingTonePlugin::stop()
{
    qDebug() << "stop";
    Msg::MusicControl::getInstance().stopPlaybackThread();
    return true;
}

bool Msg::RingTonePlugin::isFinal(QString source)
{
    return !source.isEmpty();
}

QList<QString> Msg::RingTonePlugin::getSourceList()
{
    QStringList filter;
    filter << "*.mp3";
    QDirIterator directory_walker("/usr/chumby/alarmtones", filter, QDir::Files, QDirIterator::Subdirectories);
    QList<QString> ringtones;

    while ( directory_walker.hasNext() )
    {
        directory_walker.next();

        QString fileName = directory_walker.fileName();
        fileName.chop(4);
        qDebug() << fileName;
        ringtones.append(fileName);
    }
    return ringtones;
}

Msg::Plugin* Msg::RingTonePluginFactory::CreatePlugin()
{
    return new Msg::RingTonePlugin;
}

extern "C" void * factory0( void )
{
        return new Msg::RingTonePluginFactory;
}
