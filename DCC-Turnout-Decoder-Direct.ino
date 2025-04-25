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

// This is a DCC Accessory Decoder to drive 8 Pulsed Turnouts
// Based on the NMRA Pulsed 8 stationary decoder


#include "defines.h"


#include <NmraDcc.h>
#include "PinPulser.h"

/*
 * Rosscoe Train functions and variables
 */

#include "version.h"

#include "variables.h"

#include "functions.h"


/*
 * RT end
 */

/*
// This function is called whenever a normal DCC Turnout Packet is received
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{
#ifdef  NOTIFY_TURNOUT_MSG
  Serial.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  Serial.print(Addr,DEC) ;
  Serial.print(" Direction: ");
  Serial.print(Direction ? "Thrown" : "Closed") ;
  Serial.print(" Output: ");
  Serial.print(OutputPower ? "On" : "Off") ;
#endif

// check to see if in learning mode and update address

#ifdef LEARNING
  if (learningMode == HIGH) {

//    int H = (Addr - 1) / 64;
//    int L = Addr - (H * 64);
    byte L = (Addr + 3) / 4;
    byte H = (Addr + 3) / 1024;

#ifdef DEBUG_MSG
    Serial.println("");
    Serial.print(F("Value = ")); Serial.println(Addr,DEC);
    Serial.print(F(" H = ")); Serial.println(H,DEC);
    Serial.print(F(" L = ")); Serial.println(L,DEC);
#endif
                  
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);

   }
  else
#endif

   {
    if(( Addr >= BaseTurnoutAddress ) && ( Addr < (BaseTurnoutAddress + NUM_TURNOUTS )) && OutputPower )
     {
      uint16_t pinIndex = ( (Addr - BaseTurnoutAddress) << 1 ) + Direction ;
      pinPulser.addPin(outputs[pinIndex]);
#ifdef  NOTIFY_TURNOUT_MSG
      Serial.print(" Pin Index: ");
      Serial.print(pinIndex,DEC);
      Serial.print(" Pin: ");
      Serial.print(outputs[pinIndex],DEC);
#endif
     }
   }
#ifdef  NOTIFY_TURNOUT_MSG
  Serial.println();
#endif
}

void initPinPulser(void)
{
//  BaseTurnoutAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 64) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;
  BaseTurnoutAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;

  uint16_t cduRechargeMs     = Dcc.getCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME) * 10;

#ifdef SINGLE_PULSE
  onMs              = Dcc.getCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME) * 10;
  activeOutputState = Dcc.getCV(CV_ACCESSORY_DECODER_ACTIVE_STATE);
#else
//  uint16_t onMs[NUM_TURNOUTS] = {};
//  uint8_t activeOutputState[NUM_TURNOUTS] = {};
// read the CV's for each address
  for(uint8_t i = 0; i < NUM_TURNOUTS; i++)
  {
    onMs[i] = Dcc.getCV( 33 + ( i * 2 ) ) * 10;
    activeOutputState[i]  = Dcc.getCV( 34 + ( i * 2 ) );
#ifdef DEBUG_MSG
    Serial.print(F(" i : "));Serial.print(i);
    Serial.print(F(" onMs : "));Serial.print(onMs[i]);
    Serial.print(F(" activeOutputState : "));Serial.println(activeOutputState[i]);
#endif

  }
#endif

//#ifdef DEBUG_MSG
  Serial.print(F("initPinPulser: DCC Turnout Base Address: ")); Serial.print(BaseTurnoutAddress, DEC);
  Serial.print(F(" CDU Recharge: ")); Serial.println(cduRechargeMs);
#ifdef SINGLE_PULSE
  Serial.print(F(" Active Pulse: ")); Serial.print(onMs);  
  Serial.print(F("ms Active Output State: ")); Serial.println(activeOutputState ? "HIGH" : "LOW" );
#endif
//#endif  

  // Step through all the Turnout Driver pins setting them to OUTPUT and NOT Active State
  for(uint8_t i = 0; i < (NUM_TURNOUTS * 2); i++)
  {
#ifdef SINGLE_PULSE
  	digitalWrite(outputs[i], !activeOutputState); // Set the Output Inactive before the direction so the 
#else
    digitalWrite(outputs[i], !activeOutputState[i / 2]); // Set the Output Inactive before the direction so the 
#endif
  	pinMode( outputs[i], OUTPUT );                // Pin doesn't momentarily pulse the wrong state
	}

  // Init the PinPulser with the new settings 
#ifdef SINGLE_PULSE
  pinPulser.init(onMs, cduRechargeMs, activeOutputState);
#else
  pinPulser.init(onMs, cduRechargeMs, activeOutputState, outputs);

  pinPulser.printArrays();

#endif
}
*/

void setup()
{
#ifdef ENABLE_SERIAL
  
  Serial.begin(115200);
  uint8_t maxWaitLoops = 255;
  while(!Serial && maxWaitLoops--)
    delay(20);
#endif
    
  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion. 

  setVersion();

#ifdef digitalPinToInterrupt
  Dcc.pin(DCC_PIN, 0);
#else
  Dcc.pin(0, DCC_PIN, 1);
#endif

#ifdef ENABLE_DCC_ACK
  pinMode(ENABLE_DCC_ACK, OUTPUT);
#endif

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, DCC_DECODER_VERSION_NUM, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );

#ifdef ENABLE_SERIAL

//#ifdef DEBUG_MSG
//  Serial.print("\nNMRA DCC 8-Turnout Accessory Decoder. Ver: "); Serial.println(DCC_DECODER_VERSION_NUM,DEC);
#ifdef KATO_SMT_BOARD
  Serial.print("Rosscoe Train DCC 8 Turnout Accessory Decoder Kato. ");
#else
  Serial.print("Rosscoe Train DCC 8 Turnout Accessory Decoder. ");
#endif

  Serial.print(F("Version: "));
  Serial.print(versionBuffer[0]);
  Serial.print(F("."));
  Serial.print(versionBuffer[1]);
  Serial.print(F("."));
  Serial.println(versionBuffer[2]);

  
  Serial.println();

#endif

//#endif

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
#ifdef ENABLE_SERIAL
  Serial.println("Resetting CVs to Factory Defaults");
#endif
  notifyCVResetFactoryDefault(); 
#endif

  if( FactoryDefaultCVIndex == 0)	// Not forcing a reset CV Reset to Factory Defaults so initPinPulser
	  initPinPulser();  
}

void loop()
{
#ifdef LEARNING
  static int learningbuttonOldval = 0,learningbuttonVal = 0;
#endif

  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();
  
  pinPulser.process();
  
  if( FactoryDefaultCVIndex && Dcc.isSetCVReady())
   {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array
    uint16_t cv = FactoryDefaultCVs[FactoryDefaultCVIndex].CV;
    uint8_t val = FactoryDefaultCVs[FactoryDefaultCVIndex].Value;
#ifdef DEBUG_MSG
    Serial.print("loop: Write Default CV: "); Serial.print(cv,DEC); Serial.print(" Value: "); Serial.println(val,DEC);
#endif     
    Dcc.setCV( cv, val );
    
    if( FactoryDefaultCVIndex == 0)	// Is this the last Default CV to set? if so re-initPinPulser
	    initPinPulser();
   }


/*
 * added RT
 */

  ////////////////////////////////////////////////////////////////
  // check if the learning button has been enabled
  //
  // This is the jumper on A6 marked Learn on the PCB.
  // jumper must be in place to enter learning mode
  // When in learning mode send a close or throw command from the command station
  // and the address will be programmed into the decoder
  //
  ////////////////////////////////////////////////////////////////

#ifdef LEARNING

  learningbuttonVal = dr(LEARNINGBUTTON);

  if (learningbuttonOldval != learningbuttonVal)
   {
    learningMode = learningbuttonVal;
#ifndef ARDUINO_ARCH_ESP32
    if (learningMode == HIGH) showAcknowledge(3);
#endif

   }
  learningbuttonOldval = learningbuttonVal;
#endif

#ifdef ENABLE_SERIAL
    // see if there are serial commands
  readString="";              //empty for next input

  while (Serial.available())
   {
    char c = Serial.read();     //gets one byte from serial buffer
    readString += c;            //makes the string readString
    delay(10);                   //slow looping to allow buffer to fill with next character
   }

  // act on serial commands

  if (readString.length() >0)
   {
    doSerialCommand(readString);
   } 
#endif

}



  
