/*
    crad_content_handler.c
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

#include <strings.h>
#include <stdio.h>
#include "crad_content_handler.h"
#include "crad_interface.h"
#include "qndriver.h"

#include <vector>
#include <string>


crad_t *p_crad;

/*! utility function used to parse a URI's HTTP query string into param/value pairs */
static void parseQueryString(std::string &uri, std::vector<std::string> &param, std::vector<std::string> &value)
{
    std::string::size_type query_offs = uri.find("?", 0);
    std::string::size_type var_beg = query_offs+1;

    /*! step through configuration parameters */
    while(var_beg < uri.length())
    {
        std::string::size_type var_end = uri.find("=", var_beg);

        /*! we need an equal sign, or the query string is invalid */
        if(var_end == -1) { break; }

        std::string::size_type val_end = uri.find("&", var_end+1);

        if(val_end == -1) { val_end = uri.length(); }

        std::string cur_param = uri.substr(var_beg, var_end - var_beg);
        std::string cur_value = uri.substr(var_end+1, val_end-var_end-1);

        param.push_back(cur_param);
        value.push_back(cur_value);

        var_beg = val_end+1;
    }

    return;
}

/*! utility function used to append results to result-list */
static void appendResult(std::string &content, const char *command, int ret)
{
    char *status = "success";

    if(CRAD_FAILED(ret)) { status = "failure"; }

    content += "  <result command=\"";
    content += command;
    content += "\" status=\"";
    content += status;
    content += "\"/>";

    return;
}


static char *generate_country_tag(char *country_str,
								  int country_str_size,
								  int country_id) {
	snprintf(country_str, country_str_size,
			"\t<country iso=\"%s\" start=\"%3.2f\" stop=\"%3.2f\" spacing=\"%d\"/>\n",
			country_to_iso(country_id),
			get_country_start(country_id)/100.0,
			get_country_stop(country_id)/100.0,
			steparray[get_country_step(country_id)]*10
	);
	return country_str;
}


chumby::HTTPResponse * ChumbRadioContentHandler::handleRequest(const chumby::HTTPRequest & request)
{
    std::string uri = request.getRequestURI();
    std::string baseURI = uri;
    std::string::size_type query_offs = uri.find("?", 0);

    static const char *statusURI = "/radio/status.xml";
    static const char *configURI = "/radio/configure";
    static const char *serviceStartURI = "/radio/start";
    static const char *serviceStopURI = "/radio/stop";
    static const char *serviceStatusURI = "/radio/status";
    static const char *countryCodeListURI = "/radio/countries";


    /*! create chumby radio interface instance */
    if(!p_crad) {
        crad_info_t crad_info = { 0 };

        int ret = crad_create(&crad_info, &p_crad);

        if(CRAD_FAILED(ret)) 
        { 
            fprintf(stderr, "Error: crad_create failed (%s)\n", CRAD_RETURN_CODE_LOOKUP[ret]);
            goto cleanup;
        }
    }

    /*! refresh Chumby Radio instance */
    crad_refresh(p_crad, CRAD_DEFAULT_DEVICE_PATH);

    /*! extract base URI (query string removed) */
    if(query_offs != std::string::npos)
    {
        baseURI = uri.substr(0, query_offs);
    }

    if(baseURI == statusURI)
    {
        chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);

        response->addHeader("Cache-Control", "no-cache");
        response->addHeader("Pragma", "no-cache");

        response->setMimeType("text/xml");

        /*! output chumby radio status */
        {
            char buff[16384];

            /*! retrieve status XML */
            int ret = crad_get_status_xml(p_crad, buff, sizeof(buff));

            /*! if we can't prepare even an error XML, we shouldn't even give an OKAY response */
            if(CRAD_FAILED(ret)) { return NULL; }

            std::string content = buff; 
            
            response->addContent(content);
        }

        return response;
    }
    else if(baseURI == configURI)
    {
        chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);

        response->addHeader("Cache-Control", "no-cache");
        response->addHeader("Pragma", "no-cache");

        response->setMimeType("text/xml");

        std::vector<std::string> paramList, valueList;

        /*! parse parameter/value pairs from query string */
        parseQueryString(uri, paramList, valueList);

        double station = 0.0;
        int seek_up = 0, seek_down = 0, seek_strength = CRAD_DEFAULT_SEEK_STRENGTH;
        int power = -1, rescan = -1, rds_enable = -1, country = -1;
        int api_key = 0, lock = -1;

        /*! start with a standard XML header */
        std::string content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; 

        /*! begin configuration results */
        content += "<result-list>\n";

        /*! process parameters */
        {
            int v;

            for(v=0;v<paramList.size();v++)
            {
                std::string &cur_param = paramList[v];
                std::string &cur_value = valueList[v];

                if(cur_param == "station")
                {
                    /*! attempt to parse value */
                    sscanf(cur_value.c_str(), "%lf", &station);
                }
                else if(cur_param == "power")
                {
                    sscanf(cur_value.c_str(), "%u", &power);
                }
                else if(cur_param == "rescan")
                {
                    sscanf(cur_value.c_str(), "%u", &rescan);
                }
                else if(cur_param == "rds")
                {
                    sscanf(cur_value.c_str(), "%u", &rds_enable);
                }
                else if(cur_param == "country")
                {
                    if(!strcasecmp(cur_value.c_str(), "us"))
                        country = COUNTRY_USA;
                    if(!strcasecmp(cur_value.c_str(), "it"))
                        country = COUNTRY_ITALY;
                    if(!strcasecmp(cur_value.c_str(), "jp"))
                        country = COUNTRY_JAPAN;
                    if(!strcasecmp(cur_value.c_str(), "ch"))
                        country = COUNTRY_CHINA;
                    if(!strcasecmp(cur_value.c_str(), "eu"))
                        country = COUNTRY_EUROPE;
                    if(!strcasecmp(cur_value.c_str(), "xx"))
                        country = COUNTRY_UNKNOWN;
                }
                else if(cur_param == "seek_up")
                {
                    //! attempt to parse value 
                    sscanf(cur_value.c_str(), "%u", &seek_up);
                }
                else if(cur_param == "seek_down")
                {
                    //! attempt to parse value
                    sscanf(cur_value.c_str(), "%u", &seek_down);
                }
                else if(cur_param == "seek_strength")
                {
                    //! attempt to parse value
                    sscanf(cur_value.c_str(), "%u", &seek_strength);
                }
                else if(cur_param == "key")
                {
                    sscanf(cur_value.c_str(), "%d", &api_key);
                }
                else if(cur_param == "lock")
                {
                    sscanf(cur_value.c_str(), "%u", &lock);
                }
            }
        }


        /*! If the radio is locked and the key isn't correct, deny access */
        if(crad_get_locked(p_crad) && crad_get_key(p_crad) != api_key)
        {
            appendResult(content, "locked", CRAD_ACCESS_DENIED);
            content += "</result-list>\n";
            response->addContent(content);
            return response;
        }

        if(lock >= 0)
        {
            crad_set_key(p_crad, api_key);
            appendResult(content, "lock", crad_set_locked(p_crad, lock));
        }


        /*! attempt to tune radio, if requested */
        if(station != 0.0)
        {
            int ret = crad_tune_radio(p_crad, station);

            appendResult(content, "station", ret);
        }

        if(power != -1)
        {
            appendResult(content, "power", crad_set_power(p_crad, power));
        }

        /*! If the user requests we enable RDS, fire off an RDS thread. */
        if(rds_enable != -1) {
            appendResult(content, "rds", crad_set_rds(p_crad, rds_enable));
        }

        if(country != -1) {
            appendResult(content, "country", crad_set_country(p_crad, country));
        }

        if(rescan != -1)
        {
            appendResult(content, "rescan", crad_refresh_station_list(p_crad));
        }

        if( (seek_up != 0) || (seek_down != 0) )
        {
            int ret = crad_seek_radio(p_crad, (seek_up != 0) ? CRAD_SEEK_DIR_UP : CRAD_SEEK_DIR_DOWN, seek_strength);

            if(seek_strength == -1)
            {
                seek_strength = CRAD_DEFAULT_SEEK_STRENGTH;
            }
            else
            {
                appendResult(content, "seek_strength", ret);
            }

            if(seek_up != 0)
            {
                appendResult(content, "seek_up", ret);
            }
            else
            {
                appendResult(content, "seek_down", ret);
            }
        }


        content += "</result-list>\n";

        response->addContent(content);

        return response;
    }
    else if( (baseURI == serviceStartURI) || (baseURI == serviceStopURI) || (baseURI == serviceStatusURI) )
    {
        chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);

        std::string content;

        response->addHeader("Cache-Control", "no-cache");
        response->addHeader("Pragma", "no-cache");

        response->setMimeType("text/xml");

        if(baseURI == serviceStartURI)
        {
            FILE *inp_file = popen("service_control chumbradiod start", "r");
            pclose(inp_file);
        }
        else if(baseURI == serviceStopURI)
        {
            FILE *inp_file = popen("service_control chumbradiod stop", "r");
            pclose(inp_file);
        }

        /*! redirect child stdout to HTTP response */
        {
            FILE *inp_file = popen("service_control chumbradiod status", "r");

            if(inp_file == 0) { return NULL; }

            int cur;
            while( (cur = fgetc(inp_file)) != EOF)
            {
                content += (char)cur;
            }

            pclose(inp_file);
        }

        response->addContent(content);

        return response;
    }
	else if(baseURI == countryCodeListURI) {
        chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);

        std::string content;
		char tmp_str[256];


        response->addHeader("Cache-Control", "no-cache");
        response->addHeader("Pragma", "no-cache");

        response->setMimeType("text/xml");

		content += "<countries>\n";
		content += generate_country_tag(tmp_str, sizeof(tmp_str), COUNTRY_CHINA);
		content += generate_country_tag(tmp_str, sizeof(tmp_str), COUNTRY_EUROPE);
		content += generate_country_tag(tmp_str, sizeof(tmp_str), COUNTRY_ITALY);
		content += generate_country_tag(tmp_str, sizeof(tmp_str), COUNTRY_JAPAN);
		content += generate_country_tag(tmp_str, sizeof(tmp_str), COUNTRY_USA);
		content += generate_country_tag(tmp_str, sizeof(tmp_str), COUNTRY_UNKNOWN);
		content += "</countries>\n";

		response->addContent(content);
		return response;
	}

cleanup:

    /*! cleanup Chumby Radio instance */
    /*
    if(p_crad != 0)
    {
        int ret = crad_close(p_crad);

        if(CRAD_FAILED(ret))
        {
            fprintf(stderr, "Warning: crad_close failed (0x%.08X)\n", ret);
            return NULL;
        }
    }
    */

    return NULL;
}

