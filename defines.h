/*
 * defines.h
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


#ifndef DEFINES_H
#define DEFINES_H

// You can print every DCC packet by un-commenting the line below
//#define NOTIFY_DCC_MSG

// You can print every notifyDccAccTurnoutOutput call-back by un-commenting the line below
#define NOTIFY_TURNOUT_MSG

// You can also print other Debug Messages uncommenting the line below
//#define DEBUG_MSG

// Un-Comment the line below to include learning funciton
//#define LEARNING

// Un-Comment the line below to force CVs to be written to the Factory Default values
// defined in the FactoryDefaultCVs below on Start-Up
// THIS NEEDS to be un-commented and uploaded once to setup the eeprom
// after uploading comment out the line and upload again for normal operation
//#define FORCE_RESET_FACTORY_DEFAULT_CV

// Un-Comment the line below to Enable DCC ACK for Service Mode Programming Read CV Capablilty 
//#define ENABLE_DCC_ACK  15  // This is A1 on the Iowa Scaled Engineering ARD-DCCSHIELD DCC Shield

// Un-Comment the line below if this firemware is being used on the RT_Pulse_8_HP_SMT board.
//#define SMT_BOARD

// Un-Comment the line below to use a single output pulse time.
// The pulse time will be the same for all addresses
//
//#define SINGLE_PULSE


// Define the Arduino input Pin number for the DCC Signal 
#define DCC_PIN     2

#define NUM_TURNOUTS 8                // Set Number of Turnouts (Pairs of Pins)
#define ACTIVE_OUTPUT_STATE HIGH      // Set the ACTIVE State of the output to Drive the Turnout motor electronics HIGH or LOW 

#define DEFAULT_PULSE 10              // the default pulse ms/10

#define DEFAULT_RECHARGE 30           // the default CDU recharge time ms/10

#define DCC_DECODER_VERSION_NUM 10    // Set the Decoder Version - Used by JMRI to Identify the decoder


// To set the Turnout Addresses for this board you need to change the CV values for CV1 (CV_ACCESSORY_DECODER_ADDRESS_LSB) and 
// CV9 (CV_ACCESSORY_DECODER_ADDRESS_MSB) in the FactoryDefaultCVs structure below. The Turnout Addresses are defined as: 
// Base Turnout Address is: ((((CV9 * 64) + CV1) - 1) * 4) + 1 
// With NUM_TURNOUTS 8 (above) a CV1 = 1 and CV9 = 0, the Turnout Addresses will be 1..8, for CV1 = 2 the Turnout Address is 5..12


#define CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME 2  // CV for the Output Pulse ON ms
#define CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME 3  // CV for the delay in ms to allow a CDU to recharge
#define CV_ACCESSORY_DECODER_ACTIVE_STATE      4  // CV to define the ON Output State 

#endif

  
  
