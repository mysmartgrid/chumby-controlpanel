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
        void stop();

    private:
        BLT_Decoder* _decoder;
        volatile bool _running;
    };
}

#endif // BTWRAPPER_H
