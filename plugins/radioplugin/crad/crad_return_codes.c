/*
 * crad_return_codes.c
 *
 * Aaron "Caustik" Robinson
 * (c) Copyright Chumby Industries, 2007
 * All rights reserved
 *
 * This module implements the CPI return code lookup table, which is provided
 * for convienence during debugging.
 */

#include "crad_interface.h"

char *CRAD_RETURN_CODE_LOOKUP[0x07] =
{
    "CRAD_OK",
    "CRAD_FAIL",
    "CRAD_NOTIMPL",
    "CRAD_INVALID_PARAM",
    "CRAD_OUT_OF_MEMORY",
    "CRAD_ACCESS_DENIED",
    "CRAD_INVALID_CALL"
};
