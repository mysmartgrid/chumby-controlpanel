#ifndef MUSICCONTROL_H
#define MUSICCONTROL_H

#include <QtCore/QDebug>

#include <alsa/asoundlib.h>

class MusicControl
{
public:
    MusicControl();
    void setMasterVolume(long int volume);
    void setMasterMute(int value);
    void increaseMasterVolume();
    void lowerMasterVolume();
};

#endif // MUSICCONTROL_H
