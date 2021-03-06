#ifndef CLOCKPLUGIN_H
#define CLOCKPLUGIN_H

#include "../../controlpanel/plugin.h"

#include <QtGui/QLabel>

#include <QCloseEvent>

namespace Msg
{
    class ClockPlugin : public Plugin
    {
	Q_OBJECT
	
        public:
						~ClockPlugin();
			
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
			
						bool isDimmed();
	
	public slots:
	    void dim();
	    void brighten();

        private:
            int getMaxBrightness();
            int getBrightness();
            void setBrightness(int brightness);

						bool _dimmed;
            int maxBrightness;
            double bright;
            double dark;
						
						static QString backlight_file;
    };

    class ClockPluginFactory : public PluginFactory
    {
        public:
        virtual Plugin* CreatePlugin();
    };
}

#endif //CLOCKPLUGIN_H
