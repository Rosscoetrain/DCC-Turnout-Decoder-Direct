/*
 * variables.h
 */

/*
 *  © 2023 Ross Scanlon
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this code.  If not, see <https://www.gnu.org/licenses/>.
*/



#ifndef VARIABLES_H
#define VARIABLES_H

struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};


/*
 * to use individual CV's for each address output requires two CV's
 * one for the pulse time
 * one for the output state
 * 
 */


/*
 *           address
 *           1    2    3    4    5    6    7    8
 * CV     
 *  pulse   33   35   37   39   41   43   45   47
 *  state   34   36   38   40   42   44   46   48
 */


/*
 * These are stored in an array of CV pairs.
 * Default array is as below
 * 
 */



CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
  {CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME, 30},   // x 10mS for the CDU recharge delay time
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},


#ifdef SINGLE_PULSE
  {CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME, 50},   // x 10mS for the output pulse duration
  {CV_ACCESSORY_DECODER_ACTIVE_STATE,    ACTIVE_OUTPUT_STATE},
#else
  {33, 10},
  {34, ACTIVE_OUTPUT_STATE},
  {35, 10},
  {36, ACTIVE_OUTPUT_STATE},
  {37, 10},
  {38, ACTIVE_OUTPUT_STATE},
  {39, 10},
  {40, ACTIVE_OUTPUT_STATE},
  {41, 10},
  {42, ACTIVE_OUTPUT_STATE},
  {43, 10},
  {44, ACTIVE_OUTPUT_STATE},
  {45, 10},
  {46, ACTIVE_OUTPUT_STATE},
  {47, 10},
  {48, ACTIVE_OUTPUT_STATE},
#endif
};



uint8_t FactoryDefaultCVIndex = 0;

// This is the Arduino Pin Mapping to Turnout Addresses with 2 pins per turnout 
// A1 is missing in the sequence as it is used for the DCC ACK
// The Pins are defined in Pairs T=Thrown, C=Closed (Digitrax Notation)  
//   base address 1C 1T 2C 2T 3C 3T  4C 4T  5C  5T  6C  6T  7C  7T  8C  8T
#ifndef SMT_BOARD
//byte outputs[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19};
//     pins         D3 D4 D5 D6 D7 D8 D9 D10 D11 D12 D13  A0  A2  A3  A4  A5  
byte outputs[] = { 4, 3, 6, 5, 8, 7, 10, 9, 12, 11, 14, 13, 17, 16, 19, 18};
//   pins         D4 D3 D6 D5 D8 D7 D10 D9 D12 D11  A0 D13  A3  A2  A5  A4
#else
byte outputs[] = { 4, 3, 6, 5, 8, 7, 10, 9, 11, 12, 14, 13, 17, 16, 19, 18};
//   pins         D4 D3 D6 D5 D8 D7 D10 D9 D11 D12  A0 D13  A3  A2  A5  A4
#endif

NmraDcc  Dcc ;
DCC_MSG  Packet ;
PinPulser pinPulser;

uint16_t BaseTurnoutAddress;


/*
 * Rosscoe Train functions and variables
 */
// for address learning mode
int LEARNINGBUTTON = A6;    // pin A6
#define LEDCONTROL LED_BUILTIN
int learningMode = LOW;

// buffer to hold serial commands
String readString;


#ifdef SINGLE_PULSE
  uint16_t onMs;
  uint8_t  activeOutputState;
#else
  static uint16_t onMs[NUM_TURNOUTS] = {};
  static uint8_t activeOutputState[NUM_TURNOUTS] = {};
#endif

char* version;
uint8_t versionBuffer[3];


#endif
