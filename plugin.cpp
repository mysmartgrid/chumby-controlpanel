
#include <typeinfo>
#include <iostream>
#include "plugin.h"

//
// Announce to the world that the PlugIn base
// class has been created or destroyed
//

Plugin::Plugin()
{
   std::cout << "PlugIn Created" << std::endl;
}

Plugin::~Plugin()
{
   std::cout << "PlugIn Destroyed" << std::endl;
}

