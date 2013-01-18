#include "controlpanel/audioplugin.h"

namespace Msg
{
    class RingTonePlugin : public AudioPlugin
    {
        public:
            virtual std::string getName();

            virtual void init() {
            }

            virtual QWidget* getWidget();

        virtual bool play(QString source);
        virtual bool stop();
        virtual bool isFinal(QString source);

        virtual QList<QString> getSourceList();
    };

    class RingTonePluginFactory : public PluginFactory
    {
        public:
        virtual Plugin* CreatePlugin();
    };
}
