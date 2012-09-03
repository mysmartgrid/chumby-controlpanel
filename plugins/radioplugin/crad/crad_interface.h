/*
 * crad_interface.h
 *
 * Aaron "Caustik" Robinson
 * (c) Copyright Chumby Industries, 2007
 * All rights reserved
 *
 * This API defines an interface for accessing the Chumby Radio.
 */

#ifndef CRAD_INTERFACE_H
#define CRAD_INTERFACE_H

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \name forward declarations */
/*! \{ */
struct _crad_info_t;
struct _crad_t;
/*! \} */

/*!

 Create an instance of the Chumby Radio Interface.

  @param p_crad_info (INP) - Information used to initialize Chumby Radio instance
  @param pp_crad (OUT) - Pointer to Chumby Radio instance
  @return CRAD_OK for success, otherwise CRAD_ error code

*/

extern int crad_create(struct _crad_info_t *p_crad_info, struct _crad_t **pp_crad);

/*!

 Closes an instance of the Chumby Radio Interface.

  @param p_crad (INP) - Chumby Radio instance
  @return CRAD_OK for success, otherwise CRAD_ error code

 */

extern int crad_close(struct _crad_t *p_crad);

/*!

 Refresh an instance of the Chumby Radio Interface. Should be called periodically
 in order to refresh internal device handles.

  @param p_crad (INP) - Chumby Radio instance
  @param device_path (INP) - Path to hid devices (e.g. CRAD_DEFAULT_DEVICE_PATH)
  @return CRAD_OK for success, otherwise CRAD_ error code

 */

extern int crad_refresh(struct _crad_t *p_crad, char *device_path);

/*!

 Retrieve status of Chumby Radio.

  @param p_crad (INP) - Chumby Radio instance
  @param xml_str (OUT) - XML status string
  @param max_size (INP) - Max size, in bytes, of xml_str (including null terminator)
  @return CRAD_OK for success, otherwise CRAD_ error code

*/

extern int crad_get_status_xml(struct _crad_t *p_crad, char *xml_str, int max_size);

/*!

 Tune Chumby Radio to the specified station.

  @param p_crad (INP) - Chumby Radio instance
  @param station (INP) - Station to tune radio to, e.g. 105.30 or 89.50
  @return CRAD_OK for success, otherwise CRAD_ error code

*/

extern int crad_tune_radio(struct _crad_t *p_crad, double station);

/*!

 Seek Chumby Radio, either up or down.

  @param p_crad (INP) - Chumby Radio instance
  @param direction (INP) - Direction to seek (CRAD_SEEK_DIR_*)
  @param strength (INP) - Seek strength (e.g. CRAD_DEFAULT_SEEK_STRENGTH)
  @return CRAD_OK for success, otherwise CRAD_ error code

*/

extern int crad_seek_radio(struct _crad_t *p_crad, int direction, int strength);

/*!

 Tune Chumby volume to the specified station.

  @param p_crad (INP) - Chumby Radio instance
  @param volume (INP) - Volume to set radio to (range 0-15)
  @return CRAD_OK for success, otherwise CRAD_ error code

*/

extern int crad_set_radio_volume(struct _crad_t *p_crad, int volume);


/*!

 Set the unit to behave as if it were in the specified country.

  @param p_crad (INP) - Chumby Radio instance
  @param country (INP) - Which country to set the unit to.
  @return CRAD_OK for success, otherwise CRAD_ error code

*/
extern int crad_set_country(struct _crad_t *p_crad, int country);


/*!

 Set Chumby LED to the specified value.

  @param p_crad (INP) - Chumby Radio instance
  @param led_value (INP) - Value to set LED to (range 0-7)
  @return CRAD_OK for success, otherwise CRAD_ error code

*/
extern int crad_set_radio_led(struct _crad_t *p_crad, int led_value);



/*!

 Causes the radio to refresh its internal list of tunable stations.

  @param p_crad (INP) - Chumby Radio instance
  @return CRAD_OK for success, otherwise CRAD_ error code

*/
extern int crad_refresh_station_list(struct _crad_t *p_crad);



/*!

 Power up (or down) the radio, setting up playback paths.

  @param p_crad (INP) - Chumby Radio instance
  @param power (INP) - 1 to power up, 0 to power down
  @return CRAD_OK for success, otherwise CRAD_ error code

*/
extern int crad_set_power(struct _crad_t *p_crad, int power);


/*!

 Set up the RDS monitoring thread, if requested.

  @param p_crad (INP) - Chumby Radio instance
  @param rds (INP) - 1 to enable RDS, 0 to disable
  @return CRAD_OK for success, otherwise CRAD_ error code

*/
extern int crad_set_rds(struct _crad_t *p_crad, int rds);


/*!

  Defines a new API key.

  @param p_crad (INP) - Chumby Radio instance
  @param key (INP) - The decimal key to set as the API lock.
  @return CRAD_OK for success, otherwise CRAD_ error code

*/


extern int crad_set_key(struct _crad_t *p_crad, int key);


/*!

  Returns current API key.

  @param p_crad (INP) - Chumby Radio instance
  @return Current API key

*/
extern int crad_get_key(struct _crad_t *p_crad);


/*!

  Locks or unlocks the radio.

  @param p_crad (INP) - Chumby Radio instance
  @param locked (INP) - true or false to lock or unlock the radio.
  @return CRAD_OK for success, otherwise CRAD_ error code

*/


extern int crad_set_locked(struct _crad_t *p_crad, int locked);


/*!

  Determines whether the radio is locked or not.

  @param p_crad (INP) - Chumby Radio instance
  @return true if the radio is locked, or false if not.

*/


extern int crad_get_locked(struct _crad_t *p_crad);


/*! 

  @brief Chumby Radio instance

  This structure represents an instance of the Chumby Radio Interface.

*/

struct rds_data {
    char name[5];
    char radiotext[2][65];
    char radiotext_filled[2][65];
    unsigned char radiotext_AB_flag;
    char traffic_program;
    char traffic_announcement;
    char music_speech;
    char mono_stereo;
    char artificial_head;
    char compressed;
    char static_pty;
    unsigned char program_type;
    unsigned char pi_country_identification;
    unsigned char pi_area_coverage;
    unsigned char pi_program_reference_number;
    char program_service_name[9];
    char *program_type_code;
    int radio_paging_codes;
    int linking_actuator;
    int variant_code;
    int paging;
    int extended_country_code;
    int tmc;
    int paging_identification;
    int language;
    int start_day;
    int start_hour;
    int start_minute;
    int program_item_number;
    int julian_date;
    int hour_code;
    int minute;
    int localtime_hours;
    int localtime_minutes;
    char callsign[5];
};


typedef struct _crad_t
{
    /*! initialization flag */
    int                 is_initialized;
    /*! device file handle */
    int                 device_file;
    int                 lock_key;
    int                 lock_locked;
    double              frequency;

    pthread_t           playback_thread;
    int                 playback_thread_running;

    pthread_t           rds_thread;
    pthread_mutex_t     rds_mutex;
    int                 rds_thread_running;
    struct rds_data     rds_data;
}
crad_t;

/*! 

  @brief Chumby Radio information

  This structure should be passed to crad_create() in order to initialize
  a Chumby Radio Interface instance. 

*/

typedef struct _crad_info_t
{
    int dummy; /*!< temporary placeholder */
}
crad_info_t;

/*! \name Chumby Radio default device path */
/*! \{ */
#define CRAD_DEFAULT_DEVICE_PATH "/dev"
/*! \} */

/*! \name Chumby Radio seek directions */
/*! \{ */
#define CRAD_SEEK_DIR_UP            0x0001
#define CRAD_SEEK_DIR_DOWN          0x0000
/*! \} */

/*! \name Chumby Radio default seek strength */
/*! \{ */
#define CRAD_DEFAULT_SEEK_STRENGTH  0x00 /*!< Default seek strength 0.0 */
/*! \} */

/*! \name Chumby Radio return codes */
/*! \{ */
#define CRAD_OK                     0x0000  /*!< Success! */
#define CRAD_FAIL                   0x0001  /*!< Generic failure */
#define CRAD_NOTIMPL                0x0002  /*!< Functionality not implemented */
#define CRAD_INVALID_PARAM          0x0003  /*!< Invalid parameter */
#define CRAD_OUT_OF_MEMORY          0x0004  /*!< Out of memory */
#define CRAD_ACCESS_DENIED          0x0005  /*!< Access denied */
#define CRAD_INVALID_CALL           0x0006  /*!< Invalid call */
/*! \} */

/*! \name Chumby Radio return code lookup table, for convienence */
/*! \{ */
extern char *CRAD_RETURN_CODE_LOOKUP[0x07];
/*! \} */

/*! \name Chumby Radio return code helper functions */
/*! \{ */
/*! Detect success error code */
#define CRAD_SUCCESS(x) (x == CRAD_OK)
/*! Detect failure error code */
#define CRAD_FAILED(x) (x != CRAD_OK)
/*! \} */

char *country_to_iso(int band);
extern int get_country_start(int country);
extern int get_country_stop(int country);
extern int get_country_step(int country);

#ifdef __cplusplus
}
#endif

#endif

