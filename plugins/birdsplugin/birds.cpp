#include "birds.h"

#include "birdswidget.h"

#include <QtCore/QDebug>

std::string Msg::Birds::getName()
{
    return "Birds";
}

QWidget* Msg::Birds::getWidget()
{
    if ( _widget == NULL )
        _widget = new BirdsWidget();

    return _widget;
}

Msg::Plugin* Msg::BirdsFactory::CreatePlugin()
{
    return new Msg::Birds;
}

extern "C" void * factory0( void )
{
        return new Msg::BirdsFactory;
}
