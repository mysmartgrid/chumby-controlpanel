#ifndef CLOCKPLUGIN_H
#define CLOCKPLUGIN_H

#include "../../controlpanel/plugin.h"

#include <QtCore/QSettings>
#include <QtCore/QDir>

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

        QDir *_bldir;
        QSettings *_settings;
        bool _dimmed;
        int _maxBrightness;
        double _bright;
        double _dark;
        QTimer *_sleep;
    };

    class ClockPluginFactory : public PluginFactory
    {
    public:
        virtual Plugin* CreatePlugin();
    };
}

#endif //CLOCKPLUGIN_H
