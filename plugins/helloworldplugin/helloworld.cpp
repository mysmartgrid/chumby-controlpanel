#include "helloworld.h"

#include <QtCore/QDebug>

std::string Msg::HelloWorld::getName()
{
    return "HelloWorld";
}

QWidget* Msg::HelloWorld::getWidget()
{
    QWidget* widget = new QWidget();

    return widget;
}

Msg::Plugin* Msg::HelloWorldFactory::CreatePlugin()
{
    return new Msg::HelloWorld;
}

extern "C" void * factory0( void )
{
        return new Msg::HelloWorldFactory;
}
