//*****************************************************************************
//
// Title        : MP3stick - MP3 player
// Authors      : Michael Wolf
// File Name    : 'id3.c'
// Date         : November 5, 2006
// Version      : 1.00
// Target MCU   : Atmel AVR ATmega128/1281
// Editor Tabs  : 2
//
// NOTE: The authors in no way will be responsible for damages that you
//       coul'd be using this code.
//       Use this code at your own risk.
//
//       This code is distributed under the GNU Public License
//       which can be found at http://www.gnu.org/licenses/gpl.txt
//
// Change Log
//
// Version  When        Who           What
// -------  ----        ---           ----
// 1.00     05/11/2006  Michael Wolf  Initial Release
//
//*****************************************************************************
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "id3.h"
#include "fat.h"
#include "multimediacard.h"

extern FAT_FILE mp3_file;

void get_id3_tag (char * title_name, char * artist_name, char * playlist_name, uint16_t * title_length){

    char id3tag[128];

    // set file pointer to start of ID3v1 tag
    fat_seek (&mp3_file, 128, SEEK_END);

    // check if ID3v1 tag exists
    if ((fat_read (id3tag, 128, &mp3_file) == 128) &&
        (strncmp (id3tag, "TAG", 3) == 0)) {

        strncpy (title_name, id3tag + 3, 30);
        strncpy (artist_name, id3tag + 33, 30);
        strncpy (playlist_name, id3tag + 63, 30);
    }

    // set file pointer back to start of file
    fat_seek (&mp3_file, 0, SEEK_START);

    // check if ID3v2 tag header exists
    if ((fat_read (id3tag, 10, &mp3_file) == 10) && // header successful read
        (strncmp (id3tag, "ID3", 3) == 0) && // id tag exists
        (id3tag[3] >= 3)) { // tag version 2.3.x or higher

        // get ID3v2 tag size
        uint32_t id3v2_tag_size = (id3tag[9] & 0x7f) +
            ((id3tag[8] & 0x7f) << 7) +
            ((id3tag[7] & 0x7f) << 14) + ((id3tag[6] & 0x7f) << 21);
      
        uint32_t tag_header_size = 0;
      
        // check if extended header exists
        if ((id3tag[5] & 0x40)) {
            // read extended header if present
            if (fat_read (id3tag, 6, &mp3_file) == 6) {
                // get extended header size
                tag_header_size = (id3tag[3] & 0x7f) +
                    ((id3tag[2] & 0x7f) << 7) +
                    ((id3tag[1] & 0x7f) << 14) + ((id3tag[0] & 0x7f) << 21);
                // skip extended header            
                fat_seek (&mp3_file, tag_header_size - 6, SEEK_CUR);
            }
        }
      
        while (id3v2_tag_size) {
            // read frame header
            if (fat_read (id3tag, 10, &mp3_file) == 10) {
      
                // check if frame ID exists
                if ((id3tag[0] == 0) && (id3tag[1] == 0) &&
                    (id3tag[2] == 0) && (id3tag[3] == 0)) {
                    break;
                }
            
                tag_header_size = (id3tag[7] & 0x7f) +
                    ((id3tag[6] & 0x7f) << 7) +
                    ((id3tag[5] & 0x7f) << 14) + ((id3tag[4] & 0x7f) << 21);
            
                if (strncmp (id3tag, "TPE1", 4) == 0) {
            
            
                    // read frame content, max 50 characters for artist name
                    if (fat_read (id3tag, tag_header_size % 50, &mp3_file) ==
                        tag_header_size) {
            
                        // copy artist name
                        strncpy (artist_name, &id3tag[1],
                           (tag_header_size - 1) % 50);
                    }
                }
                else if (strncmp (id3tag, "TIT2", 4) == 0) {
            
                    // read frame content, max 50 characters for  title name
                    if (fat_read (id3tag, tag_header_size % 50, &mp3_file) ==
                        tag_header_size) {
            
                        // copy title name
                        strncpy (title_name, &id3tag[1],
                           (tag_header_size - 1) % 50);
                    }
                }
                else if (strncmp (id3tag, "TLEN", 4) == 0) {
            
                    // read frame content, max 128 characters for title length
                    if (fat_read (id3tag, tag_header_size % 128, &mp3_file) ==
                        tag_header_size) {
                        // get title length in full seconds
                        *title_length = (uint16_t)(strtol( &id3tag[1], NULL, 10)/1000);
                    }
                }
                else if (strncmp (id3tag, "TALB", 4) == 0) {
            
                    // read frame content, max 50 characters for album name
                    if (fat_read (id3tag, tag_header_size % 50, &mp3_file) ==
                        tag_header_size) {
                        // copy album name
                        strncpy (playlist_name, &id3tag[1],
                           (tag_header_size - 1) % 50);
                    }
                }                  
                // skip frame if not required
                else
                    fat_seek (&mp3_file, tag_header_size, SEEK_CUR);
            }
      
            id3v2_tag_size -= 10;
        }			// end while id3v2_tag_size
    } // end if ID3v2 tag found

    // set file pointer to start of file
    fat_seek (&mp3_file, 0, SEEK_START);
}
