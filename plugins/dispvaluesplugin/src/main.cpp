#include <iostream>

#include <QIcon>

#include "dispval.h"
#include "controlpanel/plugin.h"

namespace Msg
{
    class DispValuesPlugin : public Plugin
    {
     public:
            DispValuesPlugin()
            {
                    std::cout << "DispValuesPlugin created" << std::endl;
                    dlg = NULL;
            }

            virtual ~DispValuesPlugin()
            {
										if ( dlg )
                    	delete dlg;
                    std::cout << "DispValuesPlugin destroyed" << std::endl;
            }

            virtual std::string getName()
            {
                    //std::cout << "I am a Fun PlugIn" << std::endl;
                return "Fluksometer";
            }

            virtual QWidget* getWidget()
            {
                if ( dlg == NULL )
                {
                    qDebug() << "Creating new Display";
                    dlg = new Display();
                    //connect ( dlg, SIGNAL( closeDisplay() ), this, SIGNAL( stopWidget() ) );
                }
                return dlg;
            }

            virtual QIcon* getIcon() {
                return new QIcon(":/dispval/resources/msg.png");
            }
    private:
            Display* dlg;
    };

    //
    // The PlugInFunFactory class inherits from PlugInFactory
    // and creates a PlugInFun object when requested.
    //


    class DispValuesPluginFactory : public PluginFactory
    {
     public:
            DispValuesPluginFactory()
            {
            }

            ~DispValuesPluginFactory()
            {
            }

            virtual Plugin * CreatePlugin()
            {
                    return new DispValuesPlugin;
            }

    };


    //
    // The "C" linkage factory0() function creates the PlugInFunFactory
    // class for this library
    //


}

extern "C" void * factory0( void )
{
				return new Msg::DispValuesPluginFactory;
}
