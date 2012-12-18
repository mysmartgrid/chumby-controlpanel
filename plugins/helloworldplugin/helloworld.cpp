#include "helloworld.h"

#include <QtCore/QDebug>

std::string Msg::HelloWorld::getName()
{
    return "HelloWorld";
}

QWidget* Msg::HelloWorld::getWidget()
{
    if ( _widget == NULL )
        _widget = new QWidget();

    return _widget;
}

Msg::Plugin* Msg::HelloWorldFactory::CreatePlugin()
{
    return new Msg::HelloWorld;
}

extern "C" void * factory0( void )
{
        return new Msg::HelloWorldFactory;
}
