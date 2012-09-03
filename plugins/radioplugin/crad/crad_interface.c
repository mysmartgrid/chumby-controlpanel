/*
    crad_interface.c
    Duane Maxwell, Aaron "Caustik" Robinson Chumby Industries
    
    This file is part of chumbyradio.
    Copyright (c) Chumby Industries, 2007
    
    chumbyradio is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    chumbyradio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with chumbyradio; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

/*
 This program gives command-line control over the SiLabs USB FM Radio dongle

 We're only interested here in accessing and modifying the radio's registers in order to tune the
 radio - because the device follows the USB Audio standards, you can use ALSA tools such as
 arecord to capture the audio.  For instance, if your ALSA system sees the device as "card1",
 you can pipe the radio to audio out with:

 arecord -f cd -Dplughw:1,0 | aplay

 This code takes advantage of the fact that the device presents a Human Interface Device (HID) interface.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <asm/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/hiddev.h>
#include <alsa/asoundlib.h>

#include "qndriver.h"
#include "crad_interface.h"
//#include "crad_internal.h"

/*! probe for a radio, return file number if found, otherwise -1 */
static int find_radio(char *path);
static int get_radio_register(crad_t *p_crad, int register_index);
static void set_radio_register(crad_t *p_crad, int register_index,int value);
extern void set_radio_led(crad_t *p_crad,int value);
extern int tune_radio(crad_t *p_crad, int station);
extern void seek_radio(crad_t *p_crad, int up, int strength);
static int get_radio_station(crad_t *p_crad);
static char *get_radio_name(crad_t *p_crad);
extern void set_radio_volume(crad_t *p_crad,int volume);
extern void dump_radio_xml(crad_t *p_crad);
char *get_radio_stations(crad_t *p_crad);
int crad_refresh_station_list(crad_t *p_crad);
int crad_set_power(crad_t *p_crad, int power);
int crad_set_rds(crad_t *p_crad, int rds);

/*! exported for chumbyradio cmd line (kinda hacky) */
struct hiddev_devinfo g_device_info;
extern void dump_radio_registers(crad_t *p_crad);

static int debug = 0;
int i2c_file = 0;

static void *rds_reader(void *data) {
    struct _crad_t *p_crad = (struct _crad_t *)data;
    struct rds_data *rds_data = &(p_crad->rds_data);
    int rds_status = 0;
    int this_rds_status;
    int last_callsign_hash;
    int callsign_hash_change_count = 0;

    QND_RDSEnable(QND_RDS_ON);
    while(p_crad->rds_thread_running) {
        int callsign_hash;
        char raw_rds_data[8];
        int waited = 0;

        // Wait for the RDS signal to toggle.
        while(this_rds_status == rds_status) {
            waited++;
            // Sleep value derived from the fact that 50.4 packets/sec are
            // permissible, 
            usleep(20000);
            this_rds_status = QND_ReadReg(STATUS3)&RDS_RXUPD;

            // If we wait more than 100 times, reset RDS data, because it
            // probably means we've changed to a station that doesn't
            // support RDS.
            if(waited == 100) {
                pthread_mutex_lock(&p_crad->rds_mutex);
                bzero(rds_data, sizeof(struct rds_data));
                pthread_mutex_unlock(&p_crad->rds_mutex);

                callsign_hash_change_count = 0;
            }
        }
        rds_status = this_rds_status;

        // Read the registers from the FM chip.
        QND_RDSLoadData(raw_rds_data, 0);


        pthread_mutex_lock(&p_crad->rds_mutex);
        crad_decode_rds(rds_data, raw_rds_data);
        pthread_mutex_unlock(&p_crad->rds_mutex);

        // Convert the RDS data to an integer.  This works because it's
        // four characters (or less).  If the callsign is different,
        // increment the callsign_hash_change_count.  If it reaches a
        // certain threshold, reset the RDS data due to station change.
        callsign_hash = *(((int *)rds_data->callsign));

        if(callsign_hash != last_callsign_hash)
            callsign_hash_change_count++;

        // If the callsign has been different for more than 10 tries in a
        // row, assume we've switched stations, and switch to the new
        // callsign.
        if(callsign_hash_change_count > 10) {

            pthread_mutex_lock(&p_crad->rds_mutex);
            bzero(rds_data, sizeof(struct rds_data));
            pthread_mutex_unlock(&p_crad->rds_mutex);

            last_callsign_hash = callsign_hash;
            callsign_hash_change_count = 0;
        }
    }
    QND_RDSEnable(QND_RDS_OFF);

    fprintf(stderr, "Quitting RDS thread...\n");
    pthread_exit(NULL);
}




static int *regutil_mem = NULL;
int read_kernel_memory(long offset) {
    static int *regutil_prev_mem_range = NULL;
    static int regutil_fd = 0;
    int scaled_offset;
    int result;

    int *mem_range = (int *)(offset & 0xFFFF0000L);
    if( mem_range != regutil_prev_mem_range ) {
        regutil_prev_mem_range = mem_range;

        if(regutil_mem)
            munmap(regutil_mem, 0xFFFF);
        if(regutil_fd)
            close(regutil_fd);

        regutil_fd = open("/dev/mem", O_RDWR);
        if( regutil_fd < 0 ) {
            perror("Unable to open /dev/mem");
            regutil_fd = 0;
            exit(1);
            return -1;
        }

        regutil_mem = mmap(0, 0xFFFF, PROT_READ | PROT_WRITE,
                           MAP_SHARED, regutil_fd, offset & 0xFFFF0000L);
        if( -1 == ((int)regutil_mem) ) {
            perror("Unable to mmap file");

            if( -1 == close(regutil_fd) )
                perror("Also couldn't close file");

            regutil_fd  = 0;
            regutil_mem = NULL;
            exit(1);
            return -1;
        }
    }

    scaled_offset = (offset-(offset & 0xFFFF0000L));
    result = regutil_mem[scaled_offset/sizeof(long)];

    return result;
}   

int write_kernel_memory(long offset, long value) {
    int old_value = read_kernel_memory(offset);
    int scaled_offset = (offset-(offset & 0xFFFF0000L));
    if(regutil_mem)
        regutil_mem[scaled_offset/sizeof(long)] = value;
    return old_value;
}


#define HW_AUDIOIN_ADCVOL     0x8004c050
#define HW_AUDIOIN_ADCVOL_SET 0x8004c054
#define HW_AUDIOIN_ADCVOL_CLR 0x8004c058
#define HW_AUDIOIN_ADCVOL_TOG 0x8004c05c

#define INPUT_MIC   0
#define INPUT_LINE1 1
#define INPUT_HP    2
#define INPUT_LINE2 3
static int select_input(int input) {
    if(INPUT_MIC == input) {
        write_kernel_memory(HW_AUDIOIN_ADCVOL_CLR, 0x00003030);
    }
    else if(INPUT_LINE1 == input) {
        write_kernel_memory(HW_AUDIOIN_ADCVOL_CLR, 0x00002020);
        write_kernel_memory(HW_AUDIOIN_ADCVOL_SET, 0x00001010);
    }
    else if(INPUT_HP == input) {
        write_kernel_memory(HW_AUDIOIN_ADCVOL_CLR, 0x00001010);
        write_kernel_memory(HW_AUDIOIN_ADCVOL_SET, 0x00002020);
    }
    else {
        write_kernel_memory(HW_AUDIOIN_ADCVOL_SET, 0x00003030);
    }
    return 0;
}





static snd_pcm_t *alsa_open(snd_pcm_t *pcm_handle, int stream_type) {

    /* This structure contains information about    */
    /* the hardware and can be used to specify the  */
    /* configuration to be used for the PCM stream. */
    snd_pcm_hw_params_t *hwparams;

    /* Name of the PCM device, like plughw:0,0          */
    /* The first number is the number of the soundcard, */
    /* the second number is the number of the device.   */
    char *pcm_name;

    /* Init pcm_name. Of course, later you */
    /* will make this configurable ;-)     */
    pcm_name = strdup("chumix");


    /* Allocate the snd_pcm_hw_params_t structure on the stack. */
    snd_pcm_hw_params_alloca(&hwparams);

    /* Open PCM. The last parameter of this function is the mode. */
    /* If this is set to 0, the standard mode is used. Possible   */
    /* other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.       */
    /* If SND_PCM_NONBLOCK is used, read / write access to the    */
    /* PCM device will return immediately. If SND_PCM_ASYNC is    */
    /* specified, SIGIO will be emitted whenever a period has     */
    /* been completely processed by the soundcard.                */
    if (snd_pcm_open(&pcm_handle, pcm_name, stream_type, 0) < 0) {
        fprintf(stderr, "Error opening PCM device %s\n", pcm_name);
        return(-1);
    }

    /* Init hwparams with full configuration space */
    if (snd_pcm_hw_params_any(pcm_handle, hwparams) < 0) {
        fprintf(stderr, "Can not configure this PCM device.\n");
        return(-1);
    }

    unsigned int rate = 44100; /* Sample rate */
    unsigned int exact_rate;   /* Sample rate returned by */
    /* snd_pcm_hw_params_set_rate_near */
    int dir;          /* exact_rate == rate --> dir = 0 */
    /* exact_rate < rate  --> dir = -1 */
    /* exact_rate > rate  --> dir = 1 */
    int periods = 8;       /* Number of periods */
    snd_pcm_uframes_t periodsize = 1024; /* Periodsize (bytes) */

    /* Set access type. This can be either    */
    /* SND_PCM_ACCESS_RW_INTERLEAVED or       */
    /* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
    /* There are also access types for MMAPed */
    /* access, but this is beyond the scope   */
    /* of this introduction.                  */
    if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
        fprintf(stderr, "Error setting access.\n");
        return(-1);
    }

    /* Set sample format */
    if (snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0) {
        fprintf(stderr, "Error setting format.\n");
        return(-1);
    }

    /* Set sample rate. If the exact rate is not supported */
    /* by the hardware, use nearest possible rate.         */
    exact_rate = rate;
    if (snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, 0) < 0) {
        fprintf(stderr, "Error setting rate.\n");
        return(-1);
    }
    if (rate != exact_rate) {
        fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n \
            ==> Using %d Hz instead.\n", rate, exact_rate);
    }

    /* Set number of channels */
    if (snd_pcm_hw_params_set_channels(pcm_handle, hwparams, 2) < 0) {
        fprintf(stderr, "Error setting channels.\n");
        return(-1);
    }

    /* Set number of periods. Periods used to be called fragments. */
    if (snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0) < 0) {
        fprintf(stderr, "Error setting periods.\n");
        return(-1);
    }

    /* Set buffer size (in frames). The resulting latency is given by */
    /* latency = periodsize * periods / (rate * bytes_per_frame)     */
    if (snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, 16384) < 0) {
        fprintf(stderr, "Error setting buffersize.\n");
        return(-1);
    }

    /* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    if (snd_pcm_hw_params(pcm_handle, hwparams) < 0) {
        fprintf(stderr, "Error setting HW params.\n");
        return(-1);
    }

    int status = snd_pcm_prepare(pcm_handle);
		fprintf(stdout, "Status: %i\n", status);
    if(status<0) {
        fprintf(stderr, "Unable to prepare audio device: %s\n",
                snd_strerror(status));
        goto error;
    }

    return pcm_handle;
/*

    snd_pcm_hw_params_get_period_size(hwparams, chunk_size, 0);
    snd_pcm_hw_params_get_buffer_size(hwparams, buffer_size);
    fprintf(stderr, "Chunk size: %d  Buffer size: %d\n", *chunk_size, *buffer_size);



    // Now that we have the hardware parameters set, setup the software ones.
    fprintf(stderr, "Allocating sw params\n");
    snd_pcm_sw_params_alloca(&swparams);
    status = snd_pcm_sw_params_current(*pcm_handle, swparams);
    if(status<0) {
        fprintf(stderr, "Unable to get software parameters: %s\n",
                snd_strerror(status));
        goto error;
    }


    status = snd_pcm_sw_params_get_xfer_align(swparams, &xfer_align);
    if (status<0) {
        fprintf(stderr, "Unable to obtain xfer align: %s\n",
                snd_strerror(status));
        goto error;
    }

    status = snd_pcm_sw_params_set_sleep_min(*pcm_handle, swparams, 0);
    if(status<0) {
        fprintf(stderr, "Unable to set sleep min: %s\n",
                snd_strerror(status));
        goto error;
    }





    size_t n = (*buffer_size / xfer_align) * xfer_align;



    fprintf(stderr, "Setting avail min\n");
    status = snd_pcm_sw_params_set_avail_min(*pcm_handle, swparams, *chunk_size);
    if(status<0) {
        fprintf(stderr, "Unable to set start threshold: %s\n",
                snd_strerror(status));
        goto error;
    }


    snd_pcm_uframes_t start_threshold = (double) rate * 30 / 1000000;
    if (start_threshold < 1)
        start_threshold = 1;
    if (start_threshold > n)
        start_threshold = n;
    fprintf(stderr, "Setting start threshold to %d\n", start_threshold);
    status = snd_pcm_sw_params_set_start_threshold(*pcm_handle, swparams, start_threshold);
    if(status<0) {
        fprintf(stderr, "Unable to set start threshold: %s\n",
                snd_strerror(status));
        goto error;
    }


    snd_pcm_uframes_t stop_threshold = *buffer_size + (double) rate * 0 / 1000000;
    status = snd_pcm_sw_params_set_stop_threshold(*pcm_handle, swparams, stop_threshold);
    assert(status >= 0);


    status = snd_pcm_sw_params_set_xfer_align(*pcm_handle, swparams, xfer_align);
    assert(status >= 0);



    // Write the software parameters out.
    fprintf(stderr, "Setting sw params\n");
    status = snd_pcm_sw_params(*pcm_handle, swparams);
    if(status<0) {
        fprintf(stderr, "Unable to write software parameters: %s\n",
                snd_strerror(status));
        goto error;
    }

    fprintf(stderr, "Setting blocking mode\n");
    snd_pcm_nonblock(*pcm_handle, 0);

    int bits_per_sample = snd_pcm_format_physical_width(SND_PCM_FORMAT_S16_LE);
    int bits_per_frame  = bits_per_sample * 2;
    *chunk_size         = *chunk_size * bits_per_frame / 8;
fprintf(stderr, "Bits per frame: %d\n", bits_per_frame);

    return *pcm_handle;
*/

error:
    if(pcm_handle) {
        snd_pcm_drain(pcm_handle);
        snd_pcm_close(pcm_handle);
    }
    return NULL;
}



#define timersub(a, b, result) \
do { \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
    if ((result)->tv_usec < 0) { \
        --(result)->tv_sec; \
        (result)->tv_usec += 1000000; \
    } \
} while (0)

int alsa_recover(snd_pcm_t *stream) {
    snd_pcm_status_t *status;
    int res;

    snd_pcm_status_alloca(&status);
    if ((res = snd_pcm_status(stream, status))<0) {
        error("status error: %s", snd_strerror(res));
        return -1;
    }

    if (snd_pcm_status_get_state(status) == SND_PCM_STATE_XRUN) {
        struct timeval now, diff, tstamp;
        gettimeofday(&now, 0);
        snd_pcm_status_get_trigger_tstamp(status, &tstamp);
        timersub(&now, &tstamp, &diff);
        fprintf(stderr, "%s!!! (at least %.3f ms long)\n",
            stream == SND_PCM_STREAM_PLAYBACK ? "underrun" : "overrun",
            diff.tv_sec * 1000 + diff.tv_usec / 1000.0);
        if ((res = snd_pcm_prepare(stream))<0) {
            error("xrun: prepare error: %s", snd_strerror(res));
            return -1;
        }
        return 0;     /* ok, data should be accepted again */
    }
    
    if (snd_pcm_status_get_state(status) == SND_PCM_STATE_DRAINING) {
        if(stream == SND_PCM_STREAM_CAPTURE) {
            fprintf(stderr, "capture stream format change? attempting recover...\n");
            if ((res = snd_pcm_prepare(stream))<0) {
                error("xrun(DRAINING): prepare error: %s", snd_strerror(res));
                return -1;
            }
            return 0;
        }
    }
    error("read/write error, state = %s", snd_pcm_state_name(snd_pcm_status_get_state(status)));
    return -1;
}



static int fill_rec_buffer(snd_pcm_t *rec, snd_pcm_uframes_t *data, int c) {
    int frames_read = 0;
    while(frames_read < c && frames_read >= 0) {
        int frames_read_now = snd_pcm_readi(rec, data, c-frames_read);

        if(frames_read_now > 0) {
            data        += frames_read_now;
            frames_read += frames_read_now;
        }
        else
            frames_read = frames_read_now;
    }

    return frames_read;
}


static void *playback_thread(void *cr) {
    struct _crad_t *p_crad = (struct _crad_t *)cr;

    snd_pcm_t *playback, *recording;

    alsa_open(recording, SND_PCM_STREAM_CAPTURE);
    alsa_open(playback,  SND_PCM_STREAM_PLAYBACK);

		fprintf(stdout, "Alsa devices opened!");



    {
            printf("foobar");
        // 32 bits per frame (stereo, 16 bit audio).
        snd_pcm_uframes_t data[4096];
        while(p_crad->playback_thread_running) {
            int bytes_read = fill_rec_buffer(recording,
                                             data,
                                             sizeof(data)/sizeof(data[0]));

            // If we didn't get enough data, wait, then try again.
            if(bytes_read == -EAGAIN) {
                fprintf(stderr, "Read error, trying again: %s\n",
                        snd_strerror(bytes_read));
                continue;
            }

            else if(bytes_read == -EPIPE) {
                fprintf(stderr, "Read error: (Over/Under)run\n");
                if(alsa_recover(recording)) {
                    p_crad->playback_thread_running = 0;
                    break;
                }
                continue;
            }

            // Or if we had a different error, bail.
            else if(bytes_read < 0) {
                fprintf(stderr, "Read error: %s\n", snd_strerror(bytes_read));
                p_crad->playback_thread_running = 0;
                break;
            }




            int bytes_written;
            bytes_written = snd_pcm_writei(playback, data, bytes_read);

            if(bytes_written == -EPIPE) {
                fprintf(stderr, "Write error: (Over/Under)run\n");
                if(alsa_recover(playback)) {
                    p_crad->playback_thread_running = 0;
                    break;
                }
                continue;
            }

            else if(bytes_written <= 0) {
                fprintf(stderr, "Write error (%d): %s\n",
                        bytes_written, snd_strerror(bytes_written));
                p_crad->playback_thread_running = 0;
                break;
            }
        }
    }

error:
    fprintf(stderr, "Quitting audio playback thread...\n");

    //snd_pcm_drain(recording);
    snd_pcm_close(recording);

    //snd_pcm_drain(playback);
    snd_pcm_close(playback);


    pthread_exit(NULL);
}



extern UINT8 chumby_XCLK;

int crad_create(struct _crad_info_t *p_crad_info, struct _crad_t **pp_crad)
{
    /*! sanity check - null ptr */
    if(pp_crad == 0) { return CRAD_INVALID_PARAM; }

    /*! allocate associated context */
    crad_t *p_crad = (crad_t*)malloc(sizeof(crad_t));

    /*! return allocated context */
    *pp_crad = p_crad;

    /*! set context to default state */
    memset(*pp_crad, 0, sizeof(crad_t));
    /*! default state - invalid file */
    p_crad->device_file = -1;

    // Enable PWM3 of the CPU to run at 24 MHz.
    {
        int fd = open("/psp/fmradio_xclk", O_RDONLY);
        if(fd > 0) {
            chumby_XCLK = 1;
            write_kernel_memory(0x80018138, 0x0c000000); // Change pin to PWM3 from GPIO.
            write_kernel_memory(0x80064080, 0x01800000); // Set to 24 MHz mode.
            write_kernel_memory(0x80064004, 0x00000008); // Enable PWM3.
            close(fd);
        }
    }

    fprintf(stderr, "Initializing...\n");
    QND_Init();

    fprintf(stderr, "Setting system mode...\n");
    QND_SetSysMode(QND_MODE_FM|QND_MODE_RX);

    fprintf(stderr, "Setting country...\n");
    QND_SetCountry(COUNTRY_USA);

    // Refresh the list of known-available channels.  This can take a
    // while, so we do it during init.
    fprintf(stderr, "Refreshing station list...\n");
    crad_refresh_station_list(p_crad);
    fprintf(stderr, "Done with refresh.\n");

    return CRAD_OK;
}

int crad_close(struct _crad_t *p_crad)
{
    /*! sanity check - null ptr */
    if(p_crad == 0) { return CRAD_INVALID_PARAM; }

    /*! cleanup device file */
    if(p_crad->device_file != -1)
    {
        /*! close device file */
        close(p_crad->device_file);
    }

    /*! free associated context */
    free(p_crad);

    return CRAD_OK;
}

int crad_refresh(struct _crad_t *p_crad, char *device_path)
{
    /*! sanity check - null ptr */
    if(p_crad == 0) { return CRAD_INVALID_PARAM; }

    /*! attempt to open device */
//    p_crad->device_file = find_radio(device_path);

    /*! we're all initialized now */
    p_crad->is_initialized = 1;

    return CRAD_OK;
}

static char *html_escape(char *output, int size, char *input) {
    int offset = 0;
    char *base = output;
    bzero(output, size);

    // If the user passed us an empty string, just return an empty string.
    if(!input) {
        *output = '\0';
        return base;
    }

    while(offset < size && *input) {
        if(*input == '&') {
            strncpy(output, "&amp;", size-offset);
            offset += strlen("&amp;");
        }
        else if(*input == '"') {
            strncpy(output, "&quot;", size-offset);
            offset += strlen("&quot;");
        }
        else if(*input == '\'') {
            strncpy(output, "&apos;", size-offset);
            offset += strlen("&apos;");
        }
        else if(*input == '<') {
            strncpy(output, "&lt;", size-offset);
            offset += strlen("&lt;");
        }
        else if(*input == '>') {
            strncpy(output, "&gt;", size-offset);
            offset += strlen("&gt;");
        }
        else {
            *output = *input;
            offset++;
        }
        input++;
        output = base + offset;
    }
    return base;
}

static char rds_string[2048];
static char psnescaped[256];
static char ptcescaped[256];
static char rt0escaped[256];
static char rt1escaped[256];

int crad_get_status_xml(struct _crad_t *p_crad, char *xml_str, int max_size) {
    /*! sanity check - null ptr */
    if(p_crad == 0) { return CRAD_INVALID_PARAM; }

    /*! sanity check - initialization */
    if(!p_crad->is_initialized) { return CRAD_INVALID_CALL; }

    /*! write XML header */
    strncpy(xml_str, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>", max_size);

    int status1     = QND_ReadReg(STATUS1);
    int strength    = QND_ReadReg(RSSISIG);
    int step        = QND_ReadReg(CH_STEP);
    struct rds_data data_copy;
    int radio_station = get_radio_station(p_crad);


    // If the RDS thread isn't running, make the RDS string be empty.
    // Otherwise, fill it in.
    if(!p_crad->rds_thread_running) {
        rds_string[0] = '\0';
    }
    else {

        pthread_mutex_lock(&p_crad->rds_mutex);
        data_copy = p_crad->rds_data;
        html_escape(psnescaped, sizeof(psnescaped), data_copy.program_service_name),
        html_escape(ptcescaped, sizeof(ptcescaped), data_copy.program_type_code),
        html_escape(rt0escaped, sizeof(rt0escaped), data_copy.radiotext_filled[0]),
        html_escape(rt1escaped, sizeof(rt1escaped), data_copy.radiotext_filled[1]),
        pthread_mutex_unlock(&p_crad->rds_mutex);

        snprintf(rds_string, sizeof(rds_string), 
                "callsign='%s' "
                "programservice='%s' "
                "ptycode='%s' "
                "radiotext0='%s' "
                "radiotext1='%s' "
                "juliandate='%d' "
                "hour='%d' "
                "minute='%d' "
                "localtime='%d:%02d' "
                ,
                data_copy.callsign,
                psnescaped,
                ptcescaped,
                rt0escaped,
                rt1escaped,
                data_copy.julian_date,
                data_copy.hour_code,
                data_copy.minute,
                data_copy.localtime_hours,
                data_copy.localtime_minutes
        );
    }


    snprintf(xml_str, max_size, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<radio found='1' tuned='%c' station='%d.%d' stereo='%c' "
        "signal='%d' signal_max='256' "
//            "volume='%d' volume_max='15' "
        "seek_threshold='%d' seek_threshold_max='255' "
        "spacing='%d' "
        "band='%s' "
        "%s"
        ">\n"
        "%s"
        "</radio>\n"
        ,

        // tuned in or not?
        (strength>85?'1':'0'),

        // Current station frequency
        radio_station/100,
        radio_station-(100*(radio_station/100)),

        // Whether we're in stereo or not.  (Inverted bit, 0 = stereo.)
        (status1&1?'0':'1'),

        // Signal strength (in mysterious moon-units?)
        (strength),

//            // Current volume
//            (15),

        // Seek threshold (?)
        (0),

        // Channel spacing
        steparray[QND_CH_STEP]*10,

        // Current band setting
        (qnd_Country==COUNTRY_CHINA?"China":
         (qnd_Country==COUNTRY_USA?"US/Europe":"Japan")),

        // If we're monitoring RDS data, copy that over.
        rds_string,

        // Append the list of stations we've found.
        get_radio_stations(p_crad)

    );

    /*! if we didn't have room for a null terminator, we should just 
     *  fail instead of risking corrupting the XML with one */
    if(xml_str[max_size-1] != '\0') { return CRAD_FAIL; }

    return CRAD_OK;
}

int crad_tune_radio(struct _crad_t *p_crad, double station)
{
    /*! sanity check - null ptr */
    if(p_crad == 0) { return CRAD_INVALID_PARAM; }

    /*! sanity check - initialization */
    if(!p_crad->is_initialized) { return CRAD_INVALID_CALL; }

    /*! santiy check - device file handle */
//    if(p_crad->device_file == -1) { return CRAD_INVALID_CALL; }

    /*! check range of station */
    if( (station < 87.5) || (station > 108.0) ) { return CRAD_INVALID_CALL; }

    if(tune_radio(p_crad, station*100) != 1) { return CRAD_FAIL; }

    return CRAD_OK;
}

int crad_seek_radio(struct _crad_t *p_crad, int direction, int strength)
{
    /*! sanity check - null ptr */
    if(p_crad == 0) { return CRAD_INVALID_PARAM; }

    /*! sanity check - initialization */
    if(!p_crad->is_initialized) { return CRAD_INVALID_CALL; }

    /*! santiy check - device file handle */
//    if(p_crad->device_file == -1) { return CRAD_INVALID_CALL; }

    seek_radio(p_crad, (direction == CRAD_SEEK_DIR_UP) ? 1 : 0, strength);

    return CRAD_OK;
}

int crad_set_radio_volume(struct _crad_t *p_crad, int volume)
{
#if 0
    /*! sanity check - null ptr */
    if(p_crad == 0) { return CRAD_INVALID_PARAM; }

    /*! sanity check - initialization */
    if(!p_crad->is_initialized) { return CRAD_INVALID_CALL; }

    /*! santiy check - device file handle */
    if(p_crad->device_file == -1) { return CRAD_INVALID_CALL; }

    /*! check range of volume */
    if( (volume < 0) || (volume > 15) ) { return CRAD_INVALID_PARAM; }

    /*! @todo this utility function does not handle failures */
    set_radio_volume(p_crad, volume);
#endif

    return CRAD_OK;
}

int crad_set_radio_led(struct _crad_t *p_crad, int led_value)
{
#if 0
    /*! sanity check - null ptr */
    if(p_crad == 0) { return CRAD_INVALID_PARAM; }

    /*! sanity check - initialization */
    if(!p_crad->is_initialized) { return CRAD_INVALID_CALL; }

    /*! santiy check - device file handle */
    if(p_crad->device_file == -1) { return CRAD_INVALID_CALL; }

    /*! check range of LED value */
    if( (led_value < 0) || (led_value > 7) ) { return CRAD_INVALID_PARAM; }

    /*! @todo this utility function does not handle failures */
    set_radio_led(p_crad, led_value);
#endif

    return CRAD_OK;
}

static int find_radio(char *path) {
#if 0
    int i;
    char hiddev_filename[200];
    int fd;
    for (i=0;i<HIDDEV_COUNT;i++) {
        snprintf(hiddev_filename,200,"%s/hiddev%d",path,i);
        if (debug) fprintf(stderr,"checking %s...\n",hiddev_filename);
        if ((fd = open(hiddev_filename,O_RDONLY))<0) {
            if (debug) fprintf(stderr,"no device at %s\n",hiddev_filename);
            continue;
        } else {
            ioctl(fd,HIDIOCGDEVINFO,&g_device_info);
            if (debug) fprintf(stderr,"vendor 0x%04hx product 0x%04hx version 0x%04hx\n",
                    g_device_info.vendor,g_device_info.product,g_device_info.version);
            if (g_device_info.vendor==USB_VENDOR_SILABS && g_device_info.product==USB_PRODUCT_AN264) {
                if (debug) fprintf(stderr,"!! found radio !!\n");
                ioctl(fd,HIDIOCINITREPORT,0);
                if (debug) {
                    fprintf(stderr,"vendor 0x%04hx product 0x%04hx version 0x%04hx\n",
                        g_device_info.vendor, g_device_info.product, g_device_info.version);
                    fprintf(stderr," on bus %d devnum %d ifnum %d\n",
                        g_device_info.busnum, g_device_info.devnum, g_device_info.ifnum);
                }
                return fd;
            } else {
                close(fd);
            }
            
        }
    }
    return -1;
#endif
    return 3;
}


#if 0
static int get_radio_register(crad_t *p_crad, int register_index) {
    struct hiddev_report_info rinfo;
    struct hiddev_usage_ref_multi mref;

    rinfo.report_type = HID_REPORT_TYPE_FEATURE;
    rinfo.report_id = register_index+1;
    ioctl(p_crad->device_file,HIDIOCGREPORT,&rinfo);
    mref.uref.report_type = rinfo.report_type;
    mref.uref.report_id = rinfo.report_id;
    mref.uref.field_index = 0;
    mref.uref.usage_index = 0;
    mref.num_values = 2;
    ioctl(p_crad->device_file,HIDIOCGUSAGES,&mref);
    return (mref.values[0]<<8)+mref.values[1];
}

static void set_radio_register(crad_t *p_crad, int register_index,int value) {
    struct hiddev_report_info rinfo;
    struct hiddev_usage_ref_multi mref;

    rinfo.report_type = HID_REPORT_TYPE_FEATURE;
    rinfo.report_id = register_index+1;
    ioctl(p_crad->device_file,HIDIOCGREPORT,&rinfo);
    mref.uref.report_type = rinfo.report_type;
    mref.uref.report_id = rinfo.report_id;
    mref.uref.field_index = 0;
    mref.uref.usage_index = 0;
    mref.num_values = 2;
    mref.values[0] = value>>8;
    mref.values[1] = value&0xff;
    ioctl(p_crad->device_file,HIDIOCSUSAGES,&mref);
    ioctl(p_crad->device_file,HIDIOCSREPORT,&rinfo);
}
#endif

void set_radio_led(crad_t *p_crad,int value) {
#if 0
    struct hiddev_report_info rinfo;
    struct hiddev_usage_ref_multi mref;

    rinfo.report_type = HID_REPORT_TYPE_FEATURE;
    rinfo.report_id = 19;
    ioctl(p_crad->device_file,HIDIOCGREPORT,&rinfo);
    mref.uref.report_type = rinfo.report_type;
    mref.uref.report_id = rinfo.report_id;
    mref.uref.field_index = 0;
    mref.uref.usage_index = 0;
    mref.num_values = 3;
    mref.values[0] = 0x35;
    mref.values[1] = value;
    mref.values[2] = 0xff;
    ioctl(p_crad->device_file,HIDIOCSUSAGES,&mref);
    ioctl(p_crad->device_file,HIDIOCSREPORT,&rinfo);
#endif
}

int tune_radio(crad_t *p_crad, int station) {
    QND_TuneToCH(station);
    QND_WriteReg(REG_PD2,  UNMUTE);
    p_crad->frequency = station;
    return 1;
}

void seek_radio(crad_t *p_crad, int up, int strength) {
    int channel = QNF_GetCh();
    int st = 0;

    // Mute the radio before we go and muck with seeking.
    QND_WriteReg(REG_PD2,  MUTE);
    if(up) {
        st = QND_RXSeekCH(channel+steparray[QND_CH_STEP], QND_CH_STOP,
                          QND_CH_STEP, strength, up);

        if(st <= 0)
            st = QND_RXSeekCH(QND_CH_START, channel, QND_CH_STEP, strength, up);

        if(st <= 0 )
            st = channel;
    }
    else {
        st = QND_RXSeekCH(channel-steparray[QND_CH_STEP], QND_CH_START,
                          QND_CH_STEP, strength, up);

        if(st <= 0)
            st = QND_RXSeekCH(QND_CH_STOP, channel, QND_CH_STEP, strength, up);

        if(st <= 0)
            st = channel;
    }
    tune_radio(p_crad, st);
}

static int get_radio_station(crad_t *p_crad) {
    return QNF_GetCh();
}

static char *get_radio_name(crad_t *p_crad) {
    return "QN8005";
//    static char name[256];
//    ioctl(p_crad->device_file,HIDIOCGNAME(256),name);
//    return name;
}

void set_radio_volume(crad_t *p_crad,int volume) {
#if 0
    int config = get_radio_register(p_crad,SYSCONFIG2);
    config = (config & ~SC2_VOLUME) | (volume & 0xf);
    set_radio_register(p_crad,SYSCONFIG2,config);
#endif
}

void dump_radio_registers(crad_t *p_crad) {
    int i;
    for (i=0;i<0x4f;i++) {
        printf("%s (%2d): 0x%04hx\n","register", i, QND_ReadReg(i));
    }
}



char *get_radio_stations(crad_t *p_crad) {
    // A static structure to hold all possible available stations.  A
    // worst-case scenario of all stations available shows that 2980
    // bytes will be required for this structure.
    static char radio_stations_xml[4096];
    char *xml_offset = radio_stations_xml;
    int channel_count, current_channel;

    bzero(radio_stations_xml, sizeof(radio_stations_xml));


    channel_count = chCount;
    for(current_channel=0; current_channel<channel_count; current_channel++) {
        xml_offset += snprintf(xml_offset, 
                sizeof(radio_stations_xml)-(xml_offset-radio_stations_xml),
                "    <station freq=\"%3.2f\"/>\n", chList[current_channel]/100.0);
    }


    return radio_stations_xml;
}


int crad_refresh_station_list(crad_t *p_crad) {
    int current_station = get_radio_station(p_crad);
    int mute_status = QND_ReadReg(REG_PD2);
//    QND_Init();
//    QND_SetSysMode(QND_MODE_FM|QND_MODE_RX);
//    QND_SetCountry(COUNTRY_USA);
    QND_RXSeekCHAll(QND_CH_START, QND_CH_STOP, QND_CH_STEP, 0, 1);
    tune_radio(p_crad, current_station);
    QND_WriteReg(REG_PD2, mute_status);
    return CRAD_OK;
}





int crad_set_power(crad_t *p_crad, int power) {

    if(!p_crad)
        return CRAD_INVALID_PARAM;

    if(power && !p_crad->playback_thread_running) {
//        fprintf(stderr, "Someone called crad_set_power(1)\n");

        // Set the recording device to Line1.
        select_input(INPUT_LINE1);


        // Turn the recording volume up.
        system("amixer set ADC 100%");


        // Create the thread object that'll be used to shovel data to the
        // sound card.
        fprintf(stderr, "Creating new playback thread\n");
        p_crad->playback_thread_running = 1;
        if(pthread_create(&p_crad->playback_thread, 
                          NULL, playback_thread, p_crad)) {
            perror("Unable to create playback thread");
            p_crad->playback_thread_running = 0;
            return CRAD_FAIL;
        }
        // Let pthreads know it can destroy the thread when it exits.
        pthread_detach(p_crad->playback_thread);

    }
    else if(!power && p_crad->playback_thread_running) {
        int country = qnd_Country;
        fprintf(stderr, "Someone called crad_set_power(0)\n");
        // Set the recording device back to Mic.
        select_input(INPUT_MIC);

        // Stop the playback thread.
        p_crad->playback_thread_running = 0;

        // Stop the RDS stream, if it's active.
        crad_set_rds(p_crad, 0);

        //// Init the radio so that it's ready for the next time around.
        //QND_Init();
        //QND_SetSysMode(QND_MODE_FM|QND_MODE_RX);
        //QND_SetCountry(country);
        //crad_refresh_station_list(p_crad);
    }
    else {
        fprintf(stderr, "Didn't know what to do.  power: %d  running: %d\n",
                    power, p_crad->playback_thread_running);
    }

    return CRAD_OK;
}


int crad_set_rds(crad_t *p_crad, int rds) {
    
    // If the user wants to turn on RDS, and we don't already have a thread
    // running, detach one and send it on its way.
    if(rds && !p_crad->rds_thread_running) {

        p_crad->rds_thread_running = 1;
        if(pthread_mutex_init(&p_crad->rds_mutex, NULL)) {
            perror("Unable to create RDS lock");
            return CRAD_FAIL;
        }

        // Create the thread object that'll be used to read RDS data.
        fprintf(stderr, "Creating new RDS thread\n");
        if(pthread_create(&p_crad->rds_thread, NULL, rds_reader, p_crad)) {
            perror("Unable to create RDS thread");
            p_crad->rds_thread_running = 0;
            pthread_mutex_destroy(&p_crad->rds_mutex);
            return CRAD_FAIL;
        }
        // Let pthreads know it can destroy the thread when it exits.
        pthread_detach(p_crad->rds_thread);
    }

    else if(!rds && p_crad->rds_thread_running) {
        p_crad->rds_thread_running = 0;
        pthread_mutex_destroy(&p_crad->rds_mutex);
    }



    return CRAD_OK;
}

int crad_set_country(crad_t *p_crad, int country) {
    QND_SetCountry(country);
    return CRAD_OK;
}


int crad_get_locked(crad_t *p_crad) {
    return p_crad->lock_locked;
}

int crad_set_locked(crad_t *p_crad, int locked) {
    p_crad->lock_locked = locked;
    return CRAD_OK;
}

int crad_get_key(crad_t *p_crad) {
    return p_crad->lock_key;
}

int crad_set_key(crad_t *p_crad, int key) {
    p_crad->lock_key = key;
    return CRAD_OK;
}

