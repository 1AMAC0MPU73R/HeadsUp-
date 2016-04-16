//*****************************************************************************
//
// Title        : MP3stick - MP3 player
// Authors      : Michael Wolf
// File Name    : 'id3.h'
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
#ifndef ID3_H
#define ID3_H

void get_id3_tag (char *, char *, char *, uint16_t *);

#endif
