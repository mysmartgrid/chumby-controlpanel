#ifndef CLOCKPLUGIN_H
#define CLOCKPLUGIN_H

#include "../../controlpanel/plugin.h"

#include <QtCore/QSettings>

#include <QtGui/QLabel>

#include <QCloseEvent>

namespace Msg
{
    class ClockPlugin : public Plugin
    {
        Q_OBJECT

    public:
        explicit ClockPlugin();
        ~ClockPlugin();

        virtual QIcon* getIcon() {
            return new QIcon(":/icons/resources/clock.png");
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
        void sleep();

    private:
        int getMaxBrightness();
        int getBrightness();
        void setBrightness(int brightness);

        bool _dimmed;
        int _maxBrightness;
        double _bright;
        double _dark;
        QSettings *_settings;
        QTimer *_sleep;

        static QString backlight_file;
    };

    class ClockPluginFactory : public PluginFactory
    {
    public:
        virtual Plugin* CreatePlugin();
    };
}

#endif //CLOCKPLUGIN_H
