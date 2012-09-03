/*
 * chumbyradio_internal.h
 *
 * Aaron "Caustik" Robinson
 * (c) Copyright Chumby Industries, 2007
 * All rights reserved
 *
 * This header defines internal data for accessing the Chumby Radio.
 */

#ifndef CHUMBYRADIO_INTERNAL_H
#define CHUMBYRADIO_INTERNAL_H

#define HIDDEV_COUNT    16

#define USB_VENDOR_SILABS ((__s16)0x10c4)
#define USB_PRODUCT_AN264 ((__s16)0x818a)

// radio registers
#define DEVICEID    0
#define CHIPID      1
#define POWERCONFIG 2
#define CHANNEL     3
#define SYSCONFIG1  4
#define SYSCONFIG2  5
#define SYSCONFIG3  6
#define TEST1       7
#define TEST2       8
#define BOOTCONFIG  9
#define STATUSRSSI  10
#define READCHAN    11
#define RDSA        12
#define RDSB        13
#define RDSC        14
#define RDSD        15

// DEVICEID masks
#define DI_PARTNUM  0xf000
#define DI_VENDOR   0x0fff

// CHIPID masks
#define CI_REV      0xfc00
#define CI_DEV      0x0200
#define CI_FIRMWARE 0x01ff

// POWERCONFIG masks
#define PC_MUTE_OFF 0x4000
#define PC_MONO     0x2000
#define PC_SEEKUP   0x0200
#define PC_SEEK     0x0100
#define PC_DISABLE  0x0040
#define PC_ENABLE   0x0001

// CHANNEL masks
#define CH_TUNE     0x8000
#define CH_CHANNEL  0x03ff

// SYSCONFIG1 masks
#define SC1_DEEMPHASIS  0x0800
#define SC1_RDS     0x1000

// SYSCONFIG2 masks
#define SC2_SEEKTH  0xff00
#define SC2_BAND    0x0080
#define SC2_SPACE   0x0030
#define SC2_VOLUME  0x000f

#define SC2_SPACE_200KHZ    0x0000
#define SC2_SPACE_100KHZ    0x0010
#define SC2_SPACE_50KHZ     0x0020

// STATUSRSSI masks
#define SR_RDSREADY 0x8000
#define SR_TUNED    0x4000
#define SR_SEEKFAIL 0x2000
#define SR_STEREO   0x0100
#define SR_STRENGTH 0x00ff

// READCHAN masks
#define RC_CHANNEL  0x03ff

#define COMMAND     19

#define LED_COMMAND 0x35

#define LED_NO_CHANGE       0x00
#define LED_ALL_COLOR       0x01
#define LED_BLINK_GREEN     0x02
#define LED_BLINK_RED       0x04
#define LED_BLINK_ORANGE    0x10
#define LED_SOLID_GREEN     0x20
#define LED_SOLID_RED       0x40
#define LED_SOLID_ORANGE    0x80

#endif

