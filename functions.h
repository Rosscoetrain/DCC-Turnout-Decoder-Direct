/*
 * @file functions.h
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



/*
 * a function to read an analogue pin and return a boolean value depending on reading.
 * works the same as doing a digital read on a digital pin
 * > 512 = TRUE, <= 512 = FALSE
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H


bool dr (int pin)
 {
  int val = analogRead(pin);
  return ( val > 512 ) ;
 }


/*
 * setup the version number
 */


void setVersion() {
  const String versionString = VERSION;
  char versionArray[versionString.length() + 1];
  versionString.toCharArray(versionArray, versionString.length() + 1);
  version = strtok(versionArray, "."); // Split version on .
  versionBuffer[0] = atoi(version);  // Major first
  version = strtok(NULL, ".");
  versionBuffer[1] = atoi(version);  // Minor next
  version = strtok(NULL, ".");
  versionBuffer[2] = atoi(version);  // Patch last
}


/**
 * this is just a function to show via the onboard PCB led, the state of the decoder
 */

#ifndef ARDUINO_ARCH_ESP32
void showAcknowledge(int nb) {
    for (int i=0;i<nb;i++) {
    digitalWrite(LEDCONTROL, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(LEDCONTROL, LOW);    // turn the LED off by making the voltage LOW
    delay(100);               // wait for a second
  }
}
#endif

void showUserCVs()
 {
  for(uint8_t i = 0; i < NUM_TURNOUTS; i++)
   {
    Serial.print(F("CV"));
    Serial.print(33 + (i * 2));
    Serial.print(F(" = "));
    Serial.print(Dcc.getCV(33 + (i * 2)));
    Serial.print("    ");
    Serial.print(F("CV"));
    Serial.print(34 + (i * 2));
    Serial.print(F(" = "));
    Serial.println(Dcc.getCV(34 + (i * 2)));
  }
 }



void(* resetFunc) (void) = 0;                     // declare reset function at address 0


#ifdef ENABLE_SERIAL

#include "StringSplitter.h"

void doSerialCommand(String readString)
 {
//  byte p = 0;

  readString.trim();

#ifdef ARDUINO_AVR_ATmega4809
    Serial3.println(readString);                    // so you can see the captured string
#else
    Serial.println(readString);                    // so you can see the captured string
#endif

if (readString == "<Z>")
   {

#ifdef ARDUINO_AVR_ATmega4809
    Serial3.println(F("Resetting"));
#else
    Serial.println(F("Resetting"));
#endif

    resetFunc();
   }

  if (readString == "<?>")
   {
#ifdef ARDUINO_AVR_ATmega4809
    Serial3.println(F("Help Text"));
    Serial3.println(F("Close a turnout: <C address>"));
    Serial3.println(F("Throw a turnout: <T address>"));
    Serial3.println(F("Set decoder base address: <A address>"));
#ifdef SINGLE_PULSE
    Serial3.println(F("Set decoder output pulse time: <P  mS / 10>"));
    Serial3.println(F("Set deocder active state: <S  0/1>"));
#else
    Serial3.println(F("Set decoder output pulse time: <P output  mS / 10>"));
    Serial3.println(F("Set decoder active state: <S output 0/1>"));
    Serial3.println(F("Where output is 1 - 8 as on the decoder pcb"));
#endif
    Serial3.println(F("Set decoder CDU recharge time: <R  mS / 10>"));

//    Serial3.print(F("Change decoder address LSB: <W ")); Serial.print(CV_ACCESSORY_DECODER_ADDRESS_LSB); Serial.println(F(" address>"));
//    Serial3.print(F("Change decoder address MSB: <W ")); Serial.print(CV_ACCESSORY_DECODER_ADDRESS_MSB); Serial.println(F(" address>"));
//    Serial3.print(F("Set decoder output pulse time: <W ")); Serial.print(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME); Serial.println(F(" mS / 10>"));
//    Serial3.print(F("Set decoder CDU recharge time: <W ")); Serial.print(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME); Serial.println(F(" mS / 10>"));
//    Serial3.print(F("Set deocder active state: <W ")); Serial.print(CV_ACCESSORY_DECODER_ACTIVE_STATE); Serial.println(F(" 0/1>"));
    Serial3.println(F("Reset decoder to factory defaulst: <D>"));
    Serial3.println(F("Show current CVs: <>"));
    Serial3.println(F("Soft Reset: <Z>"));
#else
    Serial.println(F("Help Text"));
    Serial.println(F("Close a turnout: <C address>"));
    Serial.println(F("Throw a turnout: <T address>"));
    Serial.println(F("Set decoder base address: <A address>"));
#ifdef SINGLE_PULSE
    Serial.println(F("Set decoder output pulse time: <P  mS / 10>"));
    Serial.println(F("Set deocder active state: <S  0/1>"));
#else
    Serial.println(F("Set decoder output pulse time: <P output  mS / 10>"));
    Serial.println(F("Set decoder active state: <S output 0/1>"));
    Serial.println(F("Where output is 1 - 8 as on the decoder pcb"));
#endif
    Serial.println(F("Set decoder CDU recharge time: <R  mS / 10>"));
 
//    Serial.print(F("Change decoder address LSB: <W ")); Serial.print(CV_ACCESSORY_DECODER_ADDRESS_LSB); Serial.println(F(" address>"));
//    Serial.print(F("Change decoder address MSB: <W ")); Serial.print(CV_ACCESSORY_DECODER_ADDRESS_MSB); Serial.println(F(" address>"));
//    Serial.print(F("Set decoder output pulse time: <W ")); Serial.print(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME); Serial.println(F(" mS / 10>"));
//    Serial.print(F("Set decoder CDU recharge time: <W ")); Serial.print(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME); Serial.println(F(" mS / 10>"));
//    Serial.print(F("Set deocder active state: <W ")); Serial.print(CV_ACCESSORY_DECODER_ACTIVE_STATE); Serial.println(F(" 0/1>"));

    Serial.println(F("Reset decoder to factory defaulst: <D>"));
    Serial.println(F("Show current CVs: <>"));
    Serial.println(F("Soft Reset: <Z>"));
#endif
   }
  else
   {
    if (readString.startsWith("<>"))
     {
#ifdef ARDUINO_AVR_ATmega4809
      Serial3.println(F("CVs are:"));
      Serial3.print(F("CV"));
      Serial3.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
      Serial3.print(F("CV"));
      Serial3.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));
      Serial3.print(F("CV"));
      Serial3.print(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME));

#ifdef SINGLE_PULSE
      Serial3.print(F("CV"));
      Serial3.print(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME));

      Serial3.print(F("CV"));
      Serial3.print(CV_ACCESSORY_DECODER_ACTIVE_STATE);
      Serial3.print(F(" = "));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_ACTIVE_STATE));
#else
      for(uint8_t i = 0; i < NUM_TURNOUTS; i++)
       {
        Serial3.print(F("CV"));
        Serial3.print(33 + (i * 2));
        Serial3.print(F(" = "));
        Serial3.println(Dcc.getCV(33 + (i * 2)));

        Serial3.print(F("CV"));
        Serial3.print(34 + (i * 2));
        Serial3.print(F(" = "));
        Serial3.println(Dcc.getCV(34 + (i * 2)));
       }
#endif
#else
      Serial.println(F("CVs are:"));
      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));
      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME));

#ifdef SINGLE_PULSE
      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME));

      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_ACTIVE_STATE);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ACTIVE_STATE));
#else
      showUserCVs();

/*
      for(uint8_t i = 0; i < NUM_TURNOUTS; i++)
       {
        Serial.print(F("CV"));
        Serial.print(33 + (i * 2));
        Serial.print(F(" = "));
        Serial.print(Dcc.getCV(33 + (i * 2)));
        Serial.print("    ");
        Serial.print(F("CV"));
        Serial.print(34 + (i * 2));
        Serial.print(F(" = "));
        Serial.println(Dcc.getCV(34 + (i * 2)));
       }
*/
#endif
#endif

     }
    else
     {
      if (readString.startsWith("<"))
       {
//        int pos = 0;
        // this is where commands are completed

        // command to close turnout <C address>

        if (readString.startsWith("<C"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();


          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();
            notifyDccAccTurnoutOutput( addr, 0, 1 );
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <C address>"));
#else
            Serial.println(F("Invalid command: should be <C address>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }


        if (readString.startsWith("<U"))
         {
          showUserCVs();
         }

         // command to throw turnout <T address>

        if (readString.startsWith("<T"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();
            notifyDccAccTurnoutOutput( addr, 1, 1 );
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <T address>"));
#else
            Serial.println(F("Invalid command: should be <T address>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }

        if (readString.startsWith("<A>"))
         {
          Serial.println(BaseTurnoutAddress);
          readString = "";
         }

         // command to set address <A address>
         // address will be adjusted to the correct base turnout address
         // eg if address is 2 this will be corrected to 1 as the address are groups of 8 with an offset of 4
         // ie 1..8, 5..12, ...

        if (readString.startsWith("<A"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            byte L = (addr + 3) / 4;
            byte H = (addr + 3) / 1024;

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.print(F("Value = ")); Serial.println(addr);
            Serial3.print(F(" H = ")); Serial.println(H);
            Serial3.print(F(" L = ")); Serial.println(L);
#else
            Serial.print(F("Value = ")); Serial.println(addr);
            Serial.print(F(" H = ")); Serial.println(H);
            Serial.print(F(" L = ")); Serial.println(L);
#endif
#endif
                  
            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <A address>"));
#else
            Serial.println(F("Invalid command: should be <A address>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }

/*
 * command to set output pulse time.
 * The value here is the number of milliseconds / 10
 * ie 100ms/10 = 10.
 * 
 */

        if (readString.startsWith("<P"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();


#ifdef SINGLE_PULSE
          if ( itemCount == 2)
           {
            int value = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.print(F("Value = ")); Serial.println(value);
#else
            Serial.print(F("Value = ")); Serial.println(value);
#endif
#endif
      
            Dcc.setCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME, value);
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <P ms/10>"));
#else
            Serial.println(F("Invalid command: should be <P ms/10>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }
#else
          if ( itemCount == 3)
           {
            int addr = splitter->getItemAtIndex(1).toInt();
            int value = splitter->getItemAtIndex(2).toInt();

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.print(F("Adress = ")); Serial.println(addr);
            Serial3.print(F("Value = ")); Serial.println(value);
#else
            Serial.print(F("Adress = ")); Serial.println(addr);
            Serial.print(F("Value = ")); Serial.println(value);
#endif
#endif
            if ( addr >= 1 && addr <= 8 )
             {
              Dcc.setCV(33 + (addr - 1) * 2, value);
             }
            else
             {
#ifdef ARDUINO_AVR_ATmega4809
              Serial3.println(F("Invalid output: should be 1 to 8"));
#else
              Serial.println(F("Invalid output: should be 1 to 8"));
#endif
             }
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <P output ms/10>"));
#else
            Serial.println(F("Invalid command: should be <P output ms/10>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }
#endif

/*
 * delay time for the CDU to recharge
 * The value here is the number of milliseconds / 10
 * ie 300ms/10 = 30
 */

        if (readString.startsWith("<R"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.print(F("Value = ")); Serial.println(addr);
#else
            Serial.print(F("Value = ")); Serial.println(addr);
#endif
#endif

            Dcc.setCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME, addr);
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <R ms/10>"));
#else
            Serial.println(F("Invalid command: should be <R ms/10>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }

/*
 * the output state
 * 1 = high for output
 * 0 = low for output
 */

        if (readString.startsWith("<S"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

#ifdef SINGLE_PULSE
          if (itemCount == 2)
           {
            int value = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.print(F("Value = ")); Serial.println(value);
#else
            Serial.print(F("Value = ")); Serial.println(value);
#endif
#endif

            if ( value == 0 || value == 1 )
             {
              Dcc.setCV(CV_ACCESSORY_DECODER_ACTIVE_STATE, value);
             }
            else
             {
#ifdef ARDUINO_AVR_ATmega4809
              Serial3.println(F("Invalid value: state should be 0 or 1"));
#else
              Serial.println(F("Invalid value: state should be 0 or 1"));
#endif
             }
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <S 0> or <S 1>"));
#else
            Serial.println(F("Invalid command: should be <S 0> or <S 1>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }
#else
          if (itemCount == 3)
           {
            int addr = splitter->getItemAtIndex(1).toInt();
            int value = splitter->getItemAtIndex(2).toInt();

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.print(F("Address = ")); Serial.println(addr);
            Serial3.print(F("Value = ")); Serial.println(value);
#else
            Serial.print(F("Address = ")); Serial.println(addr);
            Serial.print(F("Value = ")); Serial.println(value);
#endif
#endif

            Dcc.setCV(34 + (addr - 1) * 2, value);
           }
          else
           {
#ifdef ARDUINO_AVR_ATmega4809
            Serial3.println(F("Invalid command: should be <S address 0> or <S address 1>"));
#else
            Serial.println(F("Invalid command: should be <S address 0> or <S address 1>"));
#endif
           }
          delete splitter;
          splitter = NULL;
         }
#endif



    if (readString.startsWith("<X"))
     {
#ifdef ARDUINO_AVR_ATmega4809
      Serial3.println(F("Serial number is:"));
      Serial3.println(Dcc.getCV(CV_ACCESSORY_DECODER_SERIAL_LSB) + (Dcc.getCV(CV_ACCESSORY_DECODER_SERIAL_MSB) * 256 ));
      Serial3.println("");
#else
      Serial.println(F("Serial number is:"));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_SERIAL_LSB) + (Dcc.getCV(CV_ACCESSORY_DECODER_SERIAL_MSB) * 256 ));
      Serial.println("");
#endif
     }


    if (readString == "<D>")
     {
  #ifdef ARDUINO_AVR_ATmega4809
          Serial3.println(F("Reset factory default CVs"));
  #else
          Serial.println(F("Reset factory default CVs"));
  #endif
  
      notifyCVResetFactoryDefault();
     }

/*              
        if (readString.startsWith("<W"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 3)
           {
            byte addr = splitter->getItemAtIndex(1).toInt();
            int value = splitter->getItemAtIndex(2).toInt();

            switch (addr) {
              case CV_ACCESSORY_DECODER_ADDRESS_LSB:                  // CV1

                    byte L = (value + 3) / 4;
                    byte H = (value + 3) / 1024;

#ifdef DEBUG_MSG
                  Serial.print(F("Value = ")); Serial.println(value);
                  Serial.print(F(" H = ")); Serial.println(H);
                  Serial.print(F(" L = ")); Serial.println(L);
#endif
                  
                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
              break;
              case CV_ACCESSORY_DECODER_ADDRESS_MSB:                  // CV9
                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, value);
              break;

              case 8:
                if (value == 8)
                 {
                 }
              break;
              case CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME:
                if ((value >= 0) && (value <= 255))
                 {
                  Dcc.setCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME, value);
                 }
              break;
              case CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME:
                if ((value >= 0) && (value <= 255))
                 {
                  Dcc.setCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME, value);
                 }
              break;
              case CV_ACCESSORY_DECODER_ACTIVE_STATE:
                if ((value == 0) || (value == 1))
                 {
                  Dcc.setCV(CV_ACCESSORY_DECODER_ACTIVE_STATE, value);
                 }
                else
                 {
                  Serial.println(F("Value must be 0 (LOW) or 1 (HIGH)"));
                 }
              break;
              default:
                 Serial.println(F("Invalid cv number: should be <W cv value> "));
              break;
             }
           }
          else
           {
            Serial.println(F("Invalid command: should be <W cv value>"));
           }
          delete splitter;
          splitter = NULL;
         }
*/

       }
      else
       {
#ifdef ARDUINO_AVR_ATmega4809
        Serial3.println(F("ERROR: Unknown command"));
#else
        Serial.println(F("ERROR: Unknown command"));
#endif
       }
     }
   }
 }

#endif // ENABLE_SERIAL


void initPinPulser(void)
{
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
#ifdef ARDUINO_AVR_ATmega4809
    Serial3.print(F(" i : "));
    Serial3.print(i);
    Serial3.print(F(" onMs : "));
    Serial3.print(onMs[i]);
    Serial3.print(F(" activeOutputState : "));
    Serial3.println(activeOutputState[i]);
#else
    Serial.print(F(" i : "));
    Serial.print(i);
    Serial.print(F(" onMs : "));
    Serial.print(onMs[i]);
    Serial.print(F(" activeOutputState : "));
    Serial.println(activeOutputState[i]);
#endif
#endif

  }
#endif

#ifdef ENABLE_SERIAL
//#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.print(F("initPinPulser: DCC Turnout Base Address: "));
  Serial3.print(BaseTurnoutAddress, DEC);
  Serial3.print(F(" CDU Recharge: "));
  Serial3.println(cduRechargeMs);
#else
  Serial.print(F("initPinPulser: DCC Turnout Base Address: "));
  Serial.print(BaseTurnoutAddress, DEC);
  Serial.print(F(" CDU Recharge: "));
  Serial.println(cduRechargeMs);
#endif
#ifdef SINGLE_PULSE
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.print(F(" Active Pulse: "));
  Serial3.print(onMs);
  Serial3.print(F("ms Active Output State: "));
  Serial3.println(activeOutputState ? "HIGH" : "LOW" );
#else
  Serial.print(F(" Active Pulse: ")); Serial.print(onMs);  
  Serial.print(F("ms Active Output State: ")); Serial.println(activeOutputState ? "HIGH" : "LOW" );
#endif
#endif
#endif

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
 

/*
 *  DCC functions
*/


// This function is called whenever a normal DCC Turnout Packet is received
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
 {
#ifdef  NOTIFY_TURNOUT_MSG
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  Serial3.print(Addr,DEC) ;
  Serial3.print(" Direction: ");
  Serial3.print(Direction ? "Thrown" : "Closed") ;
  Serial3.print(" Output: ");
  Serial3.print(OutputPower ? "On" : "Off") ;
#else
  Serial.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  Serial.print(Addr,DEC) ;
  Serial.print(" Direction: ");
  Serial.print(Direction ? "Thrown" : "Closed") ;
  Serial.print(" Output: ");
  Serial.print(OutputPower ? "On" : "Off") ;
#endif
#endif

// check to see if in learning mode and update address

#ifdef LEARNING
  if (learningMode == HIGH) {

//    int H = (Addr - 1) / 64;
//    int L = Addr - (H * 64);
    byte L = (Addr + 3) / 4;
    byte H = (Addr + 3) / 1024;

#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
    Serial3.println("");
    Serial3.print(F("Value = ")); Serial.println(Addr,DEC);
    Serial3.print(F(" H = ")); Serial.println(H,DEC);
    Serial3.print(F(" L = ")); Serial.println(L,DEC);
#else
    Serial.println("");
    Serial.print(F("Value = ")); Serial.println(Addr,DEC);
    Serial.print(F(" H = ")); Serial.println(H,DEC);
    Serial.print(F(" L = ")); Serial.println(L,DEC);
#endif
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
#ifdef ARDUINO_AVR_ATmega4809
      Serial3.print(" Pin Index: ");
      Serial3.print(pinIndex,DEC);
      Serial3.print(" Pin: ");
      Serial3.print(outputs[pinIndex],DEC);
#else
      Serial.print(" Pin Index: ");
      Serial.print(pinIndex,DEC);
      Serial.print(" Pin: ");
      Serial.print(outputs[pinIndex],DEC);
#endif
#endif
     }
   }
#ifdef  NOTIFY_TURNOUT_MSG
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.println();
#else
  Serial.println();
#endif
#endif
 }


void notifyCVChange(uint16_t CV, uint8_t Value)
 {
#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.print("notifyCVChange: CV: ") ;
  Serial3.print(CV,DEC) ;
  Serial3.print(" Value: ") ;
  Serial3.println(Value, DEC) ;
#else
  Serial.print("notifyCVChange: CV: ") ;
  Serial.print(CV,DEC) ;
  Serial.print(" Value: ") ;
  Serial.println(Value, DEC) ;
#endif  
#endif

  Value = Value;  // Silence Compiler Warnings...

  if((CV == CV_ACCESSORY_DECODER_ADDRESS_MSB) || (CV == CV_ACCESSORY_DECODER_ADDRESS_LSB) ||
		 (CV == CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME) || (CV == CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME) || (CV == CV_ACCESSORY_DECODER_ACTIVE_STATE))
		initPinPulser();	// Some CV we care about changed so re-init the PinPulser with the new CV settings
 }

void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
};

// This function is called by the NmraDcc library when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain on the power supply for 6ms to ACK a CV Read 
#ifdef  ENABLE_DCC_ACK
void notifyCVAck(void)
 {
#ifdef DEBUG_MSG
#ifdef ARDUINO_AVR_ATmega4809
  Serial3.println("notifyCVAck") ;
#else
  Serial.println("notifyCVAck") ;
#endif
#endif
  // Configure the DCC CV Programing ACK pin for an output
  pinMode( ENABLE_DCC_ACK, OUTPUT );

  // Generate the DCC ACK 60mA pulse
  digitalWrite( ENABLE_DCC_ACK, HIGH );
  delay( 10 );  // The DCC Spec says 6ms but 10 makes sure... ;)
  digitalWrite( ENABLE_DCC_ACK, LOW );
 }
#endif

#ifdef  NOTIFY_DCC_MSG
#ifdef ARDUINO_AVR_ATmega4809
void notifyDccMsg( DCC_MSG * Msg)
 {
  Serial3.print("notifyDccMsg: ") ;
  for(uint8_t i = 0; i < Msg->Size; i++)
  {
    Serial3.print(Msg->Data[i], HEX);
    Serial3.write(' ');
  }
  Serial3.println();
 }
#else
void notifyDccMsg( DCC_MSG * Msg)
 {
  Serial.print("notifyDccMsg: ") ;
  for(uint8_t i = 0; i < Msg->Size; i++)
  {
    Serial.print(Msg->Data[i], HEX);
    Serial.write(' ');
  }
  Serial.println();
 }
#endif
#endif
#endif

