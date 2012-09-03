#include <QApplication>

#include "wizard.h"

#include "../../../controlpanel/plugin.h"

/*int main ( int argc, char **argv )
{
	QApplication app ( argc, argv );
	app.setOverrideCursor( QCursor( Qt::BlankCursor ) );

	Wizard dlg;
	dlg.showFullScreen();
	dlg.show();

	return app.exec();
}*/

namespace Msg
{
    class NetConfigPlugin : public Plugin
    {
     public:
            NetConfigPlugin()
            {
                    std::cout << "NetConfigPlugin created" << std::endl;
            }

            virtual ~NetConfigPlugin()
            {
                    std::cout << "NetConfigPlugin destroyed" << std::endl;
            }

            virtual std::string getName()
            {
                    //std::cout << "I am a Fun PlugIn" << std::endl;
                return "Network";
            }

            virtual QWidget* getWidget()
            {
                        Wizard* dlg = new Wizard();
                        connect ( dlg, SIGNAL( closeDisplay() ), this, SIGNAL( stopWidget() ) );
                        return dlg;
            }
    };

    //
    // The PlugInFunFactory class inherits from PlugInFactory
    // and creates a PlugInFun object when requested.
    //


    class NetConfigPluginFactory : public PluginFactory
    {
     public:
            NetConfigPluginFactory()
            {
            }

            ~NetConfigPluginFactory()
            {
            }

            virtual Plugin * CreatePlugin()
            {
                    return new NetConfigPlugin;
            }

    };


    //
    // The "C" linkage factory0() function creates the PlugInFunFactory
    // class for this library
    //


}

extern "C" void * factory0( void )
{
				return new Msg::NetConfigPluginFactory;
}
