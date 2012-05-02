#ifndef AUDIOPLUGIN_H
#define AUDIOPLUGIN_H

#include "plugin.h"

namespace Msg
{
    class AudioPlugin : public Plugin
    {
        Q_OBJECT
    public:
        explicit AudioPlugin(QObject *parent = 0);

        virtual pluginType getType()
        {
            return AUDIO_PLUGIN;
        }

        virtual bool play(QString source) = 0;
        virtual bool stop() = 0;

        //virtual QList<QString> getSources() = 0;
        virtual QList<QString> getSourceList() = 0;

    signals:

    public slots:

    };
}

#endif // AUDIOPLUGIN_H
