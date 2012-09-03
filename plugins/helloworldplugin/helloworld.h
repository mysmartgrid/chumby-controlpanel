#include "../../controlpanel/plugin.h"

namespace Msg
{
    class HelloWorld : public Plugin
    {
        public:
            virtual QIcon* getIcon() {
                return new QIcon(":/icon/resources/plugin.png");
            }

            virtual std::string getName();

            virtual pluginType getType()
            {
                return GENERIC_PLUGIN;
            }

            virtual void init() {
            }

            virtual QWidget* getWidget();
    };

    class HelloWorldFactory : public PluginFactory
    {
        public:
        virtual Plugin* CreatePlugin();
    };
}
