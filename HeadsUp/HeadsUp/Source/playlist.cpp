//*****************************************************************************
//
// Title        : MP3stick - MP3 player
// Authors      : Michael Wolf
// File Name    : 'playlist.h'
// Date         : February 12, 2006
// Version      : 1.11
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
// 1.00     12/02/2006  Michael Wolf  Initial Release
// 1.10     22/10/2006  Michael Wolf  +Added direct M3U playlist support
// 1.11     05/11/2006  Michael Wolf  *change code to support new FAT functions
//
//*****************************************************************************
#include <stdint.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fat.h"
#include "playlist.h"

char line_buffer[MAX_PATH];   // buffer which hold one line read from playlist
FAT_FILE playlist_file;
FAT_FILE mp3_file;

//*****************************************************************************
// Function: title_in_playlist
// Parameters: Number of playlist to search
//             Number of entries in playlist
//             Total playlist length
//             Title number to read
//             Title length
//             Title name
//             Artist name
//             Playlist name
// Returns: 1 - Playlist found
//          0 - Playlist not found
//
// Description: Reads first block of playlist and returns number of entries,
//              total length and playlist name
//*****************************************************************************
uint8_t get_title_info (uint8_t playlist,					// playlist to read
						uint16_t * entries,					// number of entries in playlist
						uint32_t * playlist_length,			// total time length in seconds
   						uint16_t title,						// title to read
						uint16_t * title_length,			// title length in seconds
						char * title_name,					// title name
						char * artist_name,					// artist name
						char *playlist_name)				// playlist name
{
    char *ptr_sector;
    char *ptr_line;
    uint16_t n;
    uint16_t bytes_read;
    uint16_t local_entries = 0;
    
    *title_length = 0;
   
    /* Open specific playlist */
    if( fat_open (playlist, NULL, "m3u", &playlist_file) == S_OK ) {
        // Read first sector of playlist
        bytes_read = fat_read(sector_buffer, 512, &playlist_file);      
        ptr_sector = sector_buffer;
        
        /* Browse complete playlist */
        while(bytes_read > 0) {
        
            ptr_line = line_buffer;
            n = sizeof(line_buffer);
            
            /* Read on line with specific number of chars or until EOF */
            while( (--n > 0) && (bytes_read > 0 ) ) {

                /* Check if line length crosses sector boundary */
                if( ptr_sector == (sector_buffer + bytes_read) ) { // buffer empty
                    
                    /* Read new sector of playlist */
                    ptr_sector = sector_buffer;
                    bytes_read = fat_read(sector_buffer, 512, &playlist_file);
                }
                
                /* Store char by char in buffer */
                *ptr_line++ = *ptr_sector;
                
                /* Stop if we reach the end of line */
                if( *ptr_sector++ == '\n' ) {
                    *(ptr_line-2) = 0;			// Null terminate string
                    break;
                }
            }      
                        
            /* Check for extended title information */
            if(strncmp("#EXTINF", line_buffer, 7) == 0) {
                
                char *endptr;
                
                /* Check if we scanning the list or playing a specific title */
                if(title != 0) {
                    /* Read title length */
                    *title_length = (uint16_t)strtol( &line_buffer[8], &endptr, 10 );
                    endptr++;	// skip ',' after title length
                    /* Search title and artist name delimiter */
                    ptr_line = strstr( endptr, " - ");                    
                    /* Null terminate title name */
                    *ptr_line = 0;
                    /* Copy title name */
                    strncpy(artist_name, endptr, 60);
                    /* Copy artist name */
                    strncpy(title_name, ptr_line+3, 60);
                } else {
                    /* Count total play time on list scan only */
                    *playlist_length += strtol( &line_buffer[8], (char **)NULL, 10);
                }
            } else
				/* Check if line is a file path name, if so, count as one entry */
				if(isalnum(line_buffer[0]) && (bytes_read > 0)) local_entries += 1;            
            
				if( (title != 0) && (title == local_entries) ) {

					char *p;
					char *tok;

						/* Parse complete line for path and file name */
						for( tok = strtok_r(line_buffer,"\\",&p); tok; tok = strtok_r(NULL,"\\",&p) ){

							/* Skip drive letter */
							if(*(tok+1) != ':') {

								if(fat_open (0, tok, NULL, &mp3_file) != S_OK) {
									partition.current_directory = partition.root_directory;
									return 0;
								}

								/* Check if token is not an MP3 file name */
								if( strncasecmp( tok+(strlen(tok)-4), ".mp3",4) != 0) {
									/* Change current directory */
									partition.current_directory = mp3_file.first_cluster;

								}	// End if check for MP3 file name

							}	// End if skip drive letter
                    
						 }	// End for loop strtok
                
						/*  Always start look-up from root directory */
						partition.current_directory = partition.root_directory;

						return 1;   // file successful read

				} // end if title and entry match

            
		}  // end while num_bytes_read > 0
        
        if(title == 0) {
            /* Update entry count if we counted all entries in playlist */
            *entries = local_entries;
            /* First 32 characters of file name (without extension) are taken as playlist name */
            memset(playlist_name,0,32); // clear old playlist name
//            strncpy (playlist_name, playlist_file->filename, (strlen(playlist_file->filename)-4)%32);

            return S_OK;   // Play list successful read
        }
    }

    return E_ERROR;			// File not found or error

}

