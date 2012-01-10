#include <QtGui/QApplication>
#include "controlpanel.h"

#include "dllmanager.h"
#include "plugin.h"

int main(int argc, char *argv[])
{
    int blubb;
    // Now create a DLLFactory for our PlugInFactory attached
    // to the requested file

    DLLFactory<PluginFactory> dll( "/home/stephan/projects/mysmartgrid/chumby/chumby-controlpanel-build-desktop-Qt_4_8_0_in_PATH__System__Release/testplugin.so" );

    //
    // If it worked we should have dll.factory pointing
    // to a subclass of PlugInFactory
    //

    if( dll.factory )
    {
            //
            // yes, we have a factory. Ask the factory to create a
            // PlugIn object for us.
            //

            Plugin *c=dll.factory->CreatePlugin();

            // tell it to show itself

            //c->Show();
            blubb = c->init();
            std::cout << blubb << std::endl;

            // show the C++ RTTI typeid name to the user

            // kill the plug in.

            delete c;
    }
    else
    {
            std::cout << "Error opening dll!" << std::endl;

            // show the error message if any

  if( dll.LastError() )
            {
                std::cout << "DLL Error: " << dll.LastError() << std::endl;
            }

    }



    QApplication a(argc, argv);
    Controlpanel w;
    w.show();
    w.setContent(blubb);
    
    return a.exec();
}

