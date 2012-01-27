#ifndef PLUGIN_H
#define PLUGIN_H

#include <iostream>
#include <QtGui/QWidget>
#include <QtCore/QObject>

namespace Msg
{
    #define m_pluginVersion 0;
    //
    // PlugIn is an abstract class.
    //
    // This is an example plug in.  This plug in only has one method, Show(),
    // which we will use to show its name.
    //
    //

    class Plugin : public QObject
    {
        Q_OBJECT
     public:
            Plugin();

            virtual ~Plugin() = 0;

            int getPluginVersion() const {
                return m_pluginVersion;
            }

            virtual std::string init() = 0;

            virtual QWidget* getWidget() = 0;

    signals:
            void stopWidget();
    };


    //
    // The is an example factory for plug ins.
    //
    // This example factory only announces when it is created/destroyed and
    // has the single abstract method CreatePlugIn() which returns a type
    // of plug in.
    //
    // In the real world, you may have multiple different classes in each
    // shared library that are made to work together.  All these classes
    // must be created by the Factory class.
    //
    // You may find it useful to have the objects that you create with
    // the factory class be given a pointer to the factory class so
    // they can create their own objects that they need, using the same
    // factory class.  Compiler support of covariant return types is
    // real useful here.
    //


    class PluginFactory
    {
     public:
            PluginFactory()
            {
          std::cout << "PlugInFactory Created" << std::endl;
            }

            virtual ~PluginFactory()
            {
          std::cout << "PlugInFactory Destroy" << std::endl;
            }

            virtual Plugin * CreatePlugin() = 0;

    };
}

#endif // PLUGIN_H
