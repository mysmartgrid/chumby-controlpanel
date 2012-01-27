#include "controlpanel.h"

#include <QtCore/QDir>
#include <QtCore/QDirIterator>

namespace Msg
{
    Controlpanel::Controlpanel(QWidget *parent)
        : QDialog(parent)
    {
        this->grabKeyboard();
        plugins = QMap< QString, QPair<DLLFactory<PluginFactory>*, Plugin*> >();
        this->getPlugins();
        list = new QListWidget();
        for ( QMap<QString, QPair<DLLFactory<PluginFactory>*, Plugin*> >::iterator i = plugins.begin(); i != plugins.end(); i++ )
        {
            list->addItem(new QListWidgetItem(i.key()));
        }
        button = new QPushButton("foobar", this);

        QHBoxLayout* mainlayout = new QHBoxLayout( this );
        mainlayout->addWidget(list);
        mainlayout->addWidget(button);

        QVBoxLayout* foobar = new QVBoxLayout();
        mainlayout->addLayout(foobar);
        stack = new QStackedWidget();
        foobar->addWidget(stack);

        connect(button, SIGNAL(clicked()), this, SLOT(startPlugin()));
    }

    Controlpanel::~Controlpanel()
    {
        this->releaseKeyboard();
    }

    void Controlpanel::startPlugin()
    {
        QString key = list->selectedItems().first()->text();
        std::cout << plugins.value(key).second->init() << std::endl;
        //stack->addWidget(plugins.value(key).second->getWidget());
        QWidget* test = plugins.value(key).second->getWidget();
        connect( plugins.value(key).second, SIGNAL( stopWidget() ), this, SLOT( stopPlugin() ) );
        this->hide();
        test->showFullScreen();
        test->show();
    }

    void Controlpanel::stopPlugin()
    {
        std::cout << "Widget stopped" << std::endl;
        this->show();
    }

    void Controlpanel::getPlugins()
    {
        QDirIterator directory_walker("/mnt/usb/chumby-controlpanel/plugins/", QDir::Files);

        while ( directory_walker.hasNext() )
        {
            directory_walker.next();

            std::cout << directory_walker.fileName().toStdString() << std::endl;

            if ( directory_walker.fileInfo().completeSuffix() == "so" )
            {
                // Now create a DLLFactory for our PlugInFactory attached
                // to the requested file

                DLLFactory<PluginFactory>* dll = new DLLFactory<PluginFactory>( directory_walker.filePath().toAscii() );

                //
                // If it worked we should have dll.factory pointing
                // to a subclass of PlugInFactory
                //

                if( dll->factory )
                {
                    //
                    // yes, we have a factory. Ask the factory to create a
                    // PlugIn object for us.
                    //

                    Plugin *c=dll->factory->CreatePlugin();

                    // tell it to show itself

                    //c->Show();
                    std::cout << c->init() << std::endl;

                    plugins.insert(QString::fromStdString(c->init()), QPair<DLLFactory<PluginFactory>*, Plugin*>(dll, c));
                }
                else
                {
                    std::cout << "Error opening dll!" << std::endl;

                    // show the error message if any

                    if( dll->LastError() )
                    {
                        std::cout << "DLL Error: " << dll->LastError() << std::endl;
                    }

                }
            }
        }
    }

    void Controlpanel::keyPressEvent(QKeyEvent *event)
    {
        std::cout << "key pressed: " << event->key() << "(" << event->text().toStdString() << ")" << std::endl;
        this->stopPlugin();
    }
}
