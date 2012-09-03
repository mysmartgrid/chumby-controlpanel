// Portions of this code come from the GNU Radio project, and therefore are
// GPLed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crad_interface.h"
#include "qndriver.h"

static char *group_codes[16][2] = {
    {
        "Basic tuning and switching information",
        "Basic tuning and switching information",
    },
    {
        "Program Item Number and slow labelign codes",
        "Program Item Number",
    },
    {
        "RadioText",
        "RadioText",
    },
    {
        "Application Identification for ODA",
        "ODA",
    },
    {
        "Clock-time and data",
        "ODA",
    },
    {
        "Transparent Data Channels (32) or ODA",
        "Transparent Data Channels (32) or ODA",
    },
    {
        "In-house applications",
        "In-house applications",
    },
    {
        "Radio paging or ODA",
        "ODA",
    },
    {
        "Traffic Message Channel or ODA",
        "ODA",
    },
    {
        "Emergency Warning System or ODA",
        "ODA",
    },
    {
        "Program Name Type",
        "ODA",
    },
    {
        "ODA",
        "ODA",
    },
    {
        "ODA",
        "ODA",
    },
    {
        "Enhanced radio paging or ODA",
        "ODA",
    },
    {
        "Enhanced Other Networks",
        "Enhanced Other Networks",
    },
    {
        "Defined in RDBS",
        "Fast switching information",
    },
};

static char *pty_codes[] = {
    "Unknown",
    "News",
    "Information",
    "Sports",
    "Talk",
    "Rock",
    "Classic Rock",
    "Adult Hits",
    "Soft rock",
    "Top 40",
    "Country",
    "Oldies",
    "Soft",
    "Nostalgia",
    "Jazz",
    "Classical",
    "Rhythm and Blues",
    "Soft Rhythm and Blues",
    "Foreign Language",
    "Religious Music",
    "Religious Talk",
    "Personality",
    "Public",
    "College",
    "Unassigned",
    "Unassigned",
    "Unassigned",
    "Unassigned",
    "Unassigned",
    "Weather",
    "Emergency Test",
    "Emergency",
};


static char *pty_codes_europe[] = {
	"None",
	"News",
	"Current Affairs",
	"Information",
	"Sport",
	"Education",
	"Drama",
	"Culture",
	"Science",
	"Varied",
	"Pop Music",
	"Rock Music",
	"Easy Listening Music",
	"Light classical",
	"Serious classical",
	"Other Music",
	"Weather",
	"Finance",
	"Children’s programs",
	"Social Affairs",
	"Religion",
	"Phone In",
	"Travel & Touring",
	"Leisure & Hobby",
	"Jazz Music",
	"Country Music",
	"National Music",
	"Oldies Music",
	"Folk Music",
	"Documentary",
	"Alarm Test",
	"Alarm - Alarm !",
};







char transform_char(char z) {
    int row, col;
    static char transform_table[16][8]  = 
                        {{' ', '0', '@', 'P', ' ', 'p', 'a', 'a'},
                        {'!', '1', 'A', 'Q', 'a', 'q', 'a', 'a'},
                        {'"', '2', 'B', 'R', 'b', 'r', 'e', 'e'},
                        {'#', '3', 'C', 'S', 'c', 's', 'e', 'e'},
                        {'?', '4', 'D', 'T', 'd', 't', 'i', 'i'},
                        {'%', '5', 'E', 'U', 'e', 'u', 'i', 'i'},
                        {'&', '6', 'F', 'V', 'f', 'v', 'o', 'o'},
                        {'\'', '7', 'G', 'W', 'g', 'w', 'o', 'o'},
                        {'(', '8', 'H', 'X', 'h', 'x', 'u', 'u'},
                        {')', '9', 'I', 'Y', 'i', 'y', 'u', 'u'},
                        {'*', ':', 'J', 'Z', 'j', 'z', 'N', 'n'},
                        {'+', ';', 'K', '[', 'k', '{', 'c', 'c'},
                        {',', '<', 'L', '\\', 'l', '|', '\0', '-'},
                        {'\0', '=', 'M', ']', 'm', '}', 'b', '{'},
                        {'.', '>', 'N', '-', 'n', '-', 'i', '}'},
                        {'/', '?', 'O', '-', 'o', ' ', 'I', 'i'}};

    z &= 0x0000007fL;
    row = (z & 0x0F);
    col = ((z >> 4) & 0x0F);
    col -= 2; //FIXME
    if(col<0)
        col=0;
    return transform_table[row][col];
}



void do_type0(struct rds_data *rds_data, unsigned int *group, int is_version_A) {
    unsigned char alternative_frequency_code_1;
    unsigned char alternative_frequency_code_2;
//    char flagstring[8]="0000000";
    
    unsigned char decoder_control_bit = (group[1]>>2) & 0x01;
    unsigned char segment_address = group[1] & 0x03;
    rds_data->traffic_program     = (group[1]>>10) & 0x01;
    rds_data->program_type        = (group[1]>>5) & 0x0F;
	if(qnd_Country == COUNTRY_EUROPE)
		rds_data->program_type_code   = pty_codes_europe[rds_data->program_type];
	else
		rds_data->program_type_code   = pty_codes[rds_data->program_type];
    rds_data->traffic_announcement= (group[1]>>4) & 0x01;
    rds_data->music_speech        = (group[1]>>3) & 0x01;
    rds_data->program_service_name[segment_address*2]=transform_char((group[3]>>8)&0xff);
    rds_data->program_service_name[segment_address*2+1]=transform_char(group[3]&0xff);
    switch (segment_address) {
        case 0:
            rds_data->mono_stereo=decoder_control_bit;
        break;
        case 1:
            rds_data->artificial_head=decoder_control_bit;
        break;
        case 2:
            rds_data->compressed=decoder_control_bit;
        break;
        case 3:
            rds_data->static_pty=decoder_control_bit;
        break;
        default:
        break;
    }
    if (is_version_A) {
        alternative_frequency_code_1=(group[2]>>8)&0xff;
        alternative_frequency_code_2=group[2]&0xff;
        //TODO decode AFs       
    }
    else {
    }
    /*
    flagstring[0]=traffic_program?'1':'0';    
    flagstring[1]=traffic_announcement?'1':'0';
    flagstring[2]=music_speech?'1':'0';
    flagstring[3]=mono_stereo?'1':'0';
    flagstring[4]=artificial_head?'1':'0';
    flagstring[5]=compressed?'1':'0';
    flagstring[6]=static_pty?'1':'0';
    */
}



void do_radiotext(struct rds_data *rds_data, unsigned int *group, int is_version_A) {
    unsigned char new_radiotext_AB_flag=(group[1]>>4) & 0x01;
    unsigned char text_offset = (group[1] & 0x0f)*2;
    int end_of_block = 0;

    // If the ab flag flips, clean out the radiotext fields.
    if (rds_data->radiotext_AB_flag != new_radiotext_AB_flag) {
        int i;


        // Now clear out the block.
        for(i= 0; i < 64; i++)
            rds_data->radiotext[0][i] = rds_data->radiotext[1][i] = ' ';

        rds_data->radiotext[0][64]  = rds_data->radiotext[1][64] = '\0';
        rds_data->radiotext_AB_flag = new_radiotext_AB_flag;
    }

    if (is_version_A) {
        char new_chars[4];
        text_offset *= 2;

        new_chars[0] = transform_char((group[2]>>8)&0xff);
        new_chars[1] = transform_char(group[2]&0xff);
        new_chars[2] = transform_char((group[3]>>8)&0xff);     
        new_chars[3] = transform_char(group[3]&0xff);      

        // If any of the new characters are a NULL, we consider it the end
        // of the block, and will copy the resulting string to the old
        // radiotext field.
        end_of_block = (!new_chars[0]) || (!new_chars[1])
                    || (!new_chars[2]) || (!new_chars[3]);

        memcpy(&rds_data->radiotext[0][text_offset],
                new_chars, sizeof(new_chars));

    }
    else {
        char new_chars[2];

        new_chars[0] = transform_char((group[3]>>8)&0xff);
        new_chars[1] = transform_char(group[3]&0xff);      

        end_of_block = (!new_chars[0]) || (!new_chars[1]);

        memcpy(&rds_data->radiotext[0][text_offset],
                new_chars, sizeof(new_chars));
    }

    if(end_of_block) {
        memcpy(rds_data->radiotext_filled[0], rds_data->radiotext[0],
               sizeof(rds_data->radiotext_filled[0]));
        memcpy(rds_data->radiotext_filled[1], rds_data->radiotext[1],
               sizeof(rds_data->radiotext_filled[1]));
    }
}



void do_timedate(struct rds_data *rds_data, unsigned int *group, int is_version_A) {

    // Version B of this group is actually ODA.  Ignore it.
    if(!is_version_A) {
        return;
    }

    rds_data->julian_date = ((group[1] & 0x0003) << 15) | ((group[2] & 0xfffe)>>1);
    rds_data->hour_code   = ((group[2] & 0x0001) << 4)  | ((group[3] & 0xf000)>>12);
    rds_data->minute      = ((group[3] >> 6) & 0x3f);
    rds_data->localtime_hours   = (group[3] & 0x1f) / 2;
    rds_data->localtime_minutes =  (group[3] & 1) ? 30 : 0;
    if(group[2] & 0x20)
        rds_data->localtime_hours = -rds_data->localtime_hours;

}


void do_program_item_number(struct rds_data *rds_data, unsigned int *group, int is_version_A) {
    if(is_version_A) {
        rds_data->radio_paging_codes = group[1]&0x1f;
        rds_data->linking_actuator   = !!(group[2]&0x8000);
        rds_data->variant_code       = ((group[2]&0x6000)>>12);

        if(0 == rds_data->variant_code) {
            rds_data->paging = (group[2]&0x0f00)>>8;
            rds_data->extended_country_code = group[2]&0x00ff;
        }
        else if(1 == rds_data->variant_code)
            rds_data->tmc = group[2]&0x0fff;
        else if(2 == rds_data->variant_code)
            rds_data->paging_identification = group[2]&0x0fff;
        else if(3 == rds_data->variant_code)
            rds_data->language = group[2]&0x0fff;

        rds_data->start_day       = (group[3]&0xf800)>>11;
        rds_data->start_hour      = (group[3]&0x07c0)>>6;
        rds_data->start_minute    = (group[3]&0x003f)>>0;

    }
    else {
        rds_data->program_item_number = group[3];
    }

}


void do_traffic(struct rds_data *rds_data, unsigned int *group, int is_version_A) {
    if(!is_version_A) {
        // Ignoring ODA group type 8B
        return;
    }
}



// Note that we're using the North American system here, which only works
// for callsigns that start with a 'W' or 'K'.
// Codes within the range of 0x1000 - 0x994F follow this rule.
// Codes between 0x9950 and 0x99B9 are three letters, and come from a LUT.
// Codes between 0xA100 and 0xA9FF are... strange.  I don't know that
// they're ever used.
// Codes between 0xC000 and 0xCFFF are Canadian, but their mapping is
// undefined.
// Codes between 0xF000 and 0xFFFF are Mexican, but their mapping is
// undefined.
// Codes between 0xB000 and 0xBFFF are national broadcasting systems.  They
// are unused.
// Codes between 0xD000 and 0xEFFF are also national broadcasting systems,
// but they are undefined.
static char *callsign_lut[] = {
    "KEX", "KFH", "KFI", "KGA", "KGO", "KGU", "KGW", "KGY", "KID",
    "KIT", "KJR", "KLO", "KLZ", "KMA", "KMJ", "KNX", "KOA", NULL,
    NULL,  NULL,  "KQV", "KSL", "KUJ", "KVI", "KWG", NULL,
    NULL, "KYW",  NULL,  "WBZ", "WDZ", "WEW", NULL,  "WGL",
    "WGN", "WGR", NULL,  "WHA", "WHB", "WHK", "WHO", NULL,
    "WIP", "WJR", "WKY", "WLS", "WLW", NULL,  NULL,  "WOC",
    NULL,  "WOL", "WOR", NULL,  NULL,  NULL, "WWJ",  "WWL",
    NULL,  NULL,  NULL,  NULL,  NULL,  NULL, "KDB",  "KGB",
    "KOY", "KPQ", "KSD", "KUT", "KXL", "KXO", NULL,  "WBT",
    "WGH", "WGY", "WHP", "WIL", "WMC", "WMT", "WOI", "WOW",
    "WRR", "WSB", "WSM", "KBW", "KCY", "KDF", NULL,  NULL,
    "KHQ", "KOB", NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
    NULL,  "WIS", "WJW", "WJZ", NULL,  NULL,  NULL,  "WRC",
};
void decode_callsign(unsigned int *group, char *callsign) {
    int pi_code = group[0];

    bzero(callsign, 5);

    // For simple callsigns, compute the value.
    if(pi_code >= 0x1000 && pi_code <= 0x994f) {
        if(pi_code >= 21672) {
            pi_code -= 21672;
            callsign[0] = 'W';
        }
        else if(pi_code >= 4096) {
            pi_code -= 4096;
            callsign[0] = 'K';
        }

        callsign[1] = (pi_code / 676);
        pi_code -= callsign[1]*676;

        callsign[2] = (pi_code / 26);
        pi_code -= callsign[2]*26;

        callsign[3] = pi_code;

        callsign[1] += 'A';
        callsign[2] += 'A';
        callsign[3] += 'A';
    }
    
    
    // For three-letter callsigns, use the LUT.
    else if(pi_code >= 0x9950 && pi_code <= 0x99B9) {
        if(callsign_lut[pi_code-0x9950])
            memcpy(callsign, callsign_lut[pi_code-0x9950], 4);
    }

    // Other codes are for fancy things like locating similar stations when
    // the current station goes out of range.  We don't want to support
    // these.
    // There are also Canadian and Mexican codes in the spec, but they are
    // nothing but a bunch of question marks, so I'm not quite sure what to
    // do about them besides ignore them all.
    else {
        ;
    }


    return;
}






// RDS comes through as four groups of 16 bits:
// [PI code]  [Group type | Version | Traffic | PT | Data] [Data] [Data]
// The bits are stored (according to the spec) as:
// [PPPPPPPPPPPPPPPP] [GGGGVTPPPPPDDDDD] [DDDDDDDDDDDDDDDD] [DDDDDDDDDDDDDDDD]
void crad_decode_rds(struct rds_data *rds_data, char *data) {
    int word0 = (data[1]   ) | (data[0]<<8);
    int word1 = (data[3]   ) | (data[2]<<8);
    int word2 = (data[5]   ) | (data[4]<<8);
    int word3 = (data[7]   ) | (data[6]<<8);


    int pi_code         = word0;
    int type_code       = (word1>>12) & 0xf;
    int version         = !!(word1&(1<<11));
    int traffic_program = !!(word1&(1<<10));
    int pt_code         = ((word1&(0x1f<<5))>>5)&0x1f;
    int groups[4];

    // Convert the words to an array of groups, which is what we pass to
    // functions.
    groups[0] = word0;
    groups[1] = word1;
    groups[2] = word2;
    groups[3] = word3;


    // Decode the callsign, according to D.6 of RDS.
    decode_callsign(groups, rds_data->callsign);


    switch(rds_data, type_code) {

        case 0:
            do_type0(rds_data, groups, !version);
            break;

        case 1:
            do_program_item_number(rds_data, groups, !version);
            break;

        case 2:
            do_radiotext(rds_data, groups, !version);
            break;

        case 4:
            do_timedate(rds_data, groups, !version);
            break;

        case 8:
#warning Implement traffic
            do_traffic(rds_data, groups, !version);
            break;

        // Ignore pure-ODA messages.
        case 3:
        case 11:
        case 12:
            break;

        case 14:
#warning Implement other-stations
            break;

        default:
//            fprintf(stderr, "Unhandled code: %d\n", type_code);
            break;
    }

/*
    
    // Clear cursor to top of screen.
    fprintf(stderr, "%c[H", 0x1b);


    fprintf(stderr, "---\n");
    fprintf(stderr, "RDS data: %02x%02x %02x%02x %02x%02x %02x%02x\n",
            data[0], data[1],
            data[2], data[3],
            data[4], data[5],
            data[6], data[7]);
    fprintf(stderr, "Listening to %s\n", rds_data->callsign);
    fprintf(stderr, "==>%s<== ", rds_data->program_service_name);
    fprintf(stderr, "- %2s - %2s ", (rds_data->traffic_program?"TP":"  "), (rds_data->traffic_announcement?"TA":"  "));
    fprintf(stderr, "- %6s - %6s ", (rds_data->music_speech?"Music":"Speech"), (rds_data->mono_stereo?"MONO":"STEREO")); 
    fprintf(stderr, "- %15s\n", pty_codes[rds_data->program_type]);
    fprintf(stderr, "Radiotext[0]: %64s\n", rds_data->radiotext[0]);
    fprintf(stderr, "Radiotext[1]: %64s\n", rds_data->radiotext[1]);
   // fprintf(stderr, "UTC Julian date: %d  Hour code: %d  Minute: %d  Localtime: %d:%02d  \n", julian_date, hour_code, minute, localtime_hours, localtime_minutes);
  //  fprintf(stderr, "Paging code: %d  Linking actuator: %d  Paging: %d  Country: %d  TMC: %d  Paging Identification: %d  Language: %d  Day: %d  Hour: %d  Minute: %d  Program item: %d\n", radio_paging_codes, linking_actuator, paging, extended_country_code, tmc, paging_identification, language, start_day, start_hour, start_minute, program_item_number);
*/


}


