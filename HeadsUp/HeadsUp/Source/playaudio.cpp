#include <avr/io.h>
#include <avr/eeprom.h>
#include "spi.h"
#include "fat.h"
#include "multimediacard.h"
#include "vs1011eController.h"
#include "id3.h"
#include "playlist.h"
#include "playaudio.h"


char *audiodata_pntr;	// points to audio data in buffer

volatile uint16_t actual_playlist = 0;	// actual playlist to choose titles
volatile uint16_t actual_title = 1;	// actual title to play

uint16_t playlist_entries = 0;	// number of titles in actual playlist
uint32_t playlist_length = 0;	// total length in seconds of playlist
char playlist_name[60];	// actual playlist name
uint16_t number_of_playlists = 0;	// number of playlists in root directory

uint16_t title_time = 0;	// actual title length in seconds
char titlename[60];		// actual title name
char artistname[60];		// actual artist name

volatile uint8_t is_playing = 0;		// to indicate playing state
uint16_t last_decode_time = 0;				// to check if time display update is necessary

// preset EEPROM variables
#define EEPROM __attribute__ ((section (".eeprom")))
EEPROM uint8_t ee_volume = 0;	// default full volume
EEPROM int8_t ee_balance = 100;	// default balance center
EEPROM uint8_t ee_bassboost = 0;	// default no bass boost
//EEPROM uint8_t ee_randommode = 0;	// default random mode


void play_audio_through_vs1011(){

	spi_init();
	VS1011setDataDirection();
	InitVS1011();
	vs1011_hardreset();
	//vs1011_setvolume (eeprom_read_byte(&ee_volume), (int8_t)(eeprom_read_byte((uint8_t*)&ee_balance)));
	//vs1011_setbassboost (eeprom_read_byte(&ee_bassboost));
	sinewave_beep_test(4);

	if (init_fat() == S_OK) {
		number_of_playlists = dir_list("m3u", &playlist_file);
		get_playlist_info(actual_playlist, &playlist_entries, &playlist_length, playlist_name);
		sinewave_beep_test(2);
	} else {
		number_of_playlists = 0;
		// ERROR: NO songs found
	}
	play_title();
	sinewave_beep_test(2);
}

//*****************************************************************************
// Function: pump_audio
// Parameters: none.
// Returns: none.
//
// Description: Fill audio data FIFO of VS1011
//*****************************************************************************
void pump_audio(){

    int16_t read_data;

    while( bit_is_set(VS_DREQ_INP, VS_DREQ) ) {

        if (audiodata_pntr == (sector_buffer + 512))	// Buffer empty
        {
            audiodata_pntr = sector_buffer;				// Data pointer at begin of sector buffer

            read_data = fat_read(sector_buffer, 512, &mp3_file);
      
            // If we reach the end of file play next title
            if (read_data == 0) {
                //next_title (1);	// Play next title
            }
        }
        vs1011_send32(audiodata_pntr);	// Send data on SPI bus
        audiodata_pntr += 32;			// Advance data pointer
    }
}

//*****************************************************************************
// Function: play_title
// Parameters: none.
// Returns: Error status
//          0 = Error
//          1 = No error
//
// Description: get title infos and start audio stream
//*****************************************************************************
uint8_t play_title(){

    vs1011_softreset();   // reset VS1011

    vs1011_setvolume (eeprom_read_byte (&ee_volume),
		      eeprom_read_byte ((uint8_t *)&ee_balance));
    vs1011_setbassboost (eeprom_read_byte (&ee_bassboost));

    is_playing = 0;		// prevent playing in case of error

    // get actual title information from play list
    if (get_title_info (actual_playlist,
						&playlist_entries,
						&playlist_length,
						actual_title,
						&title_time,
						titlename,
						artistname,
						playlist_name) == 1 ) {

        // read ID3 tag informations
        get_id3_tag(titlename, artistname, playlist_name, &title_time);

        // read first sector
        if (fat_read (sector_buffer, 512, &mp3_file) > 0) {
      
            //show_titleinfo ();	// show title infos on LCD 
            
            audiodata_pntr = sector_buffer;	// init pointer to audio data
            pump_audio ();	// pump initial audio data to VS1011
      
            is_playing = 1;	// indicate playing state
      
            last_decode_time = 0;
      
            return 1;		// return no error
        }
    }

    return 0;			// return error

}