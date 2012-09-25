
#include <typeinfo>
#include <iostream>
#include "plugin.h"

//
// Announce to the world that the PlugIn base
// class has been created or destroyed
//

namespace Msg
{
    Plugin::Plugin()
        : _widget(NULL)
    {
    }

    Plugin::~Plugin()
    {
        if ( _widget )
            delete _widget;
    }
}
