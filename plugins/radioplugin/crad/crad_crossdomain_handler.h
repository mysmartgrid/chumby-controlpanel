/*
 * crad_crossdomain_handler.h
 *
 * Aaron "Caustik" Robinson
 * (c) Copyright Chumby Industries, 2007
 * All rights reserved
 *
 * This header declares the cross domain handler for Chumby Radio.
 */

#ifndef CRAD_CROSSDOMAIN_HANDLER_H
#define CRAD_CROSSDOMAIN_HANDLER_H

#include <chumby_httpd/chumby_http_server.h>

/*! @brief Chumby Radio Cross Domain Handler */
class ChumbRadioCrossDomainHandler : public chumby::HTTPContentHandler 
{
    public:

        chumby::HTTPResponse * handleRequest(const chumby::HTTPRequest & request);
};

#endif

