#ifndef BTWRAPPER_H
#define BTWRAPPER_H

#include "BlueTune.h"

#include <QtCore/QString>

namespace Msg
{
    class BtWrapper
    {
    public:
        BtWrapper();
        ~BtWrapper();
        void play(QString source);

    private:
        BLT_Decoder* _decoder;
    };
}

#endif // BTWRAPPER_H
