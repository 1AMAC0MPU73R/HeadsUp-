//*****************************************************************************
//
// Title        : MP3stick - MP3 player
// Authors      : Michael Wolf
// File Name    : 'playlist.h'
// Date         : February 12, 2006
// Version      : 1.10
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
//
//*****************************************************************************

#ifndef PLAYLIST_H_
#define PLAYLIST_H_


#include <stddef.h>			// Used to get NULL keyword


extern FAT_FILE playlist_file;
extern FAT_FILE mp3_file;


uint8_t get_title_info ( uint8_t playlist,				// playlist to read
						 uint16_t * entries,			// number of entries in playlist
						 uint32_t * playlist_length,	// total time length in seconds
						 uint16_t title,				// title to read
						 uint16_t * title_length,		// title length in seconds
						 char * title_name,				// title name
						 char * artist_name,			// artist name
						 char *playlist_name );			// playlist name


#define get_playlist_info(playlist,entries,playlist_length,playlist_name) \
get_title_info(playlist,entries,playlist_length,0,NULL,NULL,NULL,playlist_name);


#endif /* PLAYLIST_H_ */