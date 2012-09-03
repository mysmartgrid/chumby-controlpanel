#ifndef NIGHTPLUGIN_H
#define NIGHTPLUGIN_H

#include "../../controlpanel/plugin.h"

#include <QtGui/QLabel>

#include <QCloseEvent>

namespace Msg
{
#if 0
    class NightWidget : public QWidget
    {
	Q_OBJECT
	
	public:
		NightWidget(QObject *parent);
		
	signals:
		void closed();
		
	public slots:
		void updateClock();
		
	protected:
		virtual void closeEvent(QCloseEvent *event);
	    
	private:
	    QLabel* timeLabel;
    };
#endif
    
    class NightPlugin : public Plugin
    {
	Q_OBJECT
	
        public:
						~NightPlugin();
			
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

    class NightPluginFactory : public PluginFactory
    {
        public:
        virtual Plugin* CreatePlugin();
    };
}

#endif //NIGHTPLUGIN_H
