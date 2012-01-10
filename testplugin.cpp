#include <typeinfo>
#include <iostream>
#include "plugin.h"

// Class PlugInFun inherits from PlugIn
// and shows the world when one is created/destroyed
// and supplies a Show() method that announces its name


class TestPlugin : public Plugin
{
 public:
        TestPlugin()
        {
                std::cout << "PlugInFun created" << std::endl;
        }

        virtual ~TestPlugin()
        {
                std::cout << "PlugInFun destroyed" << std::endl;
        }

        virtual int init()
        {
                //std::cout << "I am a Fun PlugIn" << std::endl;
            return 128;
        }
};

//
// The PlugInFunFactory class inherits from PlugInFactory
// and creates a PlugInFun object when requested.
//


class TestPluginFactory : public PluginFactory
{
 public:
        TestPluginFactory()
        {
        }

        ~TestPluginFactory()
        {
        }

        virtual Plugin * CreatePlugin()
        {
                return new TestPlugin;
        }

};


//
// The "C" linkage factory0() function creates the PlugInFunFactory
// class for this library
//


extern "C" void * factory0( void )
{
        return new TestPluginFactory;
}
