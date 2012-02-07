#include "musiccontrol.h"

MusicControl::MusicControl()
{
}

void MusicControl::setMasterVolume(long int volume)
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "DAC";

    ::snd_mixer_open(&handle, 0);
    ::snd_mixer_attach(handle, card);
    ::snd_mixer_selem_register(handle, NULL, NULL);
    ::snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(handle);
}

void MusicControl::increaseMasterVolume()
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "DAC";

    ::snd_mixer_open(&handle, 0);
    ::snd_mixer_attach(handle, card);
    ::snd_mixer_selem_register(handle, NULL, NULL);
    ::snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    long int volume;
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    qDebug() << min << " <= " << volume << " <= " << max;
    if ( volume <= max - 3 )
        snd_mixer_selem_set_playback_volume_all(elem, volume + 3 );

    snd_mixer_close(handle);
}

void MusicControl::lowerMasterVolume()
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "DAC";

    ::snd_mixer_open(&handle, 0);
    ::snd_mixer_attach(handle, card);
    ::snd_mixer_selem_register(handle, NULL, NULL);
    ::snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    long int volume;
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    qDebug() << min << " <= " << volume << " <= " << max;
    if ( volume >= min + 3 )
        snd_mixer_selem_set_playback_volume_all(elem, volume - 3);

    snd_mixer_close(handle);
}

void MusicControl::setMasterMute(int value)
{
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "DAC Playback Switch";

    ::snd_mixer_open(&handle, 0);
    ::snd_mixer_attach(handle, card);
    ::snd_mixer_selem_register(handle, NULL, NULL);
    ::snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_set_playback_switch_all(elem, value);

    snd_mixer_close(handle);
}
