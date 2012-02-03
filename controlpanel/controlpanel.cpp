#include "controlpanel.h"

#include <QtCore/QDir>
#include <QtCore/QDirIterator>

#include <QDebug>

namespace Msg
{
    Controlpanel::Controlpanel(QWidget *parent)
        : QDialog(parent)
    {
        this->grabKeyboard();
        plugins = QMap< QString, QPair<DLLFactory<PluginFactory>*, Plugin*> >();
        this->getPlugins();
        layout = new QVBoxLayout( this );
        header = new QHBoxLayout();
        layout->addLayout( header );
        header->addWidget( new QLabel(), 10 );
        list = new QListWidget();
        layout->addWidget( list );
        list->setGridSize(QSize(110, 65));
        list->setViewMode(QListWidget::IconMode);
        list->setMovement(QListWidget::Static);
        for ( QMap<QString, QPair<DLLFactory<PluginFactory>*, Plugin*> >::iterator i = plugins.begin(); i != plugins.end(); i++ )
        {
            list->addItem(new QListWidgetItem(*(i.value().second->getIcon()), i.key()));
        }
        connect(list, SIGNAL( clicked(QModelIndex) ), this, SLOT( startPlugin() ));

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
        qDebug() << "key pressed: "
                 << event->key()
                 << "("
                 << event->text()
                 << ")";
        switch ( event->key() )
        {
            case 28:
                this->stopPlugin();
                break;
            case 8:
                // volume
                std::cout << "adjusting volume" << std::endl;
                this->setMasterVolume(100);
                break;
        }
    }
    }
}
