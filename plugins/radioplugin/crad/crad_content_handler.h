/*
 * crad_content_handler.h
 *
 * Aaron "Caustik" Robinson
 * (c) Copyright Chumby Industries, 2007
 * All rights reserved
 *
 * This header declares the content handler for Chumby Radio.
 */

#ifndef CRAD_CONTENT_HANDLER_H
#define CRAD_CONTENT_HANDLER_H

#include <chumby_httpd/chumby_http_server.h>

/*! @brief Chumby Radio Content Handler */
class ChumbRadioContentHandler : public chumby::HTTPContentHandler 
{
    public:

        chumby::HTTPResponse * handleRequest(const chumby::HTTPRequest & request);
};

#endif

