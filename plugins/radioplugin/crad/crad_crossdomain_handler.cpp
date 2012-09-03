/*
    crad_crossdomain_handler.c
    Aaron "Caustik" Robinson, Chumby Industries

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

#include <string.h>
#include "crad_crossdomain_handler.h"

chumby::HTTPResponse * ChumbRadioCrossDomainHandler::handleRequest(const chumby::HTTPRequest & request)
{
    std::string uri = request.getRequestURI();

    static const char *crossDomainURI = "/crossdomain.xml";

    if(uri == crossDomainURI)
    {
        static const char *crossDomainXML = 
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE cross-domain-policy SYSTEM \"http://www.macromedia.com/xml/dtds/cross-domain-policy.dtd\">\n"
            "<cross-domain-policy>\n"
            "  <allow-access-from domain=\"*\"/>\n"
            "</cross-domain-policy>\n";

        chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);

        response->setMimeType("text/xml");
        response->addContent(crossDomainXML, strlen(crossDomainXML));

        return response;
    }

    return NULL;
}

