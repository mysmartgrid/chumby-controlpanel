#include "btwrapper.h"

#include <QtCore/QDebug>

namespace Msg
{
    BtWrapper::BtWrapper()
    {
        BLT_Result result = BLT_Decoder_Create(&_decoder);

        if ( BLT_FAILED(result) )
        {
            qDebug() << "BlueTune decoder creation failed! (" << result << ")";
            //TODO: Throw an exception
        }

        result = BLT_Decoder_RegisterBuiltins(_decoder);

        if ( BLT_FAILED(result) )
        {
            qDebug() << "BlueTune decoder registration of builtins failed! (" << result << ")";
        }

        result = BLT_Decoder_SetOutput(_decoder, "alsa:default", 0);

        if ( BLT_FAILED(result) )
        {
            qDebug() << "BlueTune decoder set output failed! (" << result << ")";
        }
    }

    BtWrapper::~BtWrapper()
    {
        BLT_Decoder_Destroy(_decoder);
    }

    void BtWrapper::play(QString source)
    {
        BLT_Result result = BLT_Decoder_SetInput(_decoder, source.toAscii(), 0);

        if ( BLT_FAILED(result) )
        {
            qDebug() << "BlueTune decoder set input failed! (" << result << ")";
        }

        do {
            result = BLT_Decoder_PumpPacket(_decoder);
            if ( BLT_FAILED(result) && result != ATX_ERROR_EOS )
                qDebug() << "PumpPacket failed:" << result;
        } while (BLT_SUCCEEDED(result));
    }
}
