
// This is a DCC Accessory Decoder to drive 8 Pulsed Turnouts
// Based on the NMRA Pulsed 8 stationary decoder


#include "defines.h"


#include <NmraDcc.h>
#include "PinPulser.h"

/*
 * Rosscoe Train functions and variables
 */

#include "variables.h"

#include "functions.h"

/*
 * RT end
 */


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
  uint16_t onMs              = Dcc.getCV(CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME) * 10;
  uint8_t  activeOutputState = Dcc.getCV(CV_ACCESSORY_DECODER_ACTIVE_STATE);
#else
  uint16_t onMs[NUM_TURNOUTS] = {};
  uint8_t activeOutputState[NUM_TURNOUTS] = {};
// read the CV's for each address
  for(uint8_t i = 0; i < NUM_TURNOUTS; i++)
  {
    onMs[i] = Dcc.getCV( 33 + ( i * 2 ) ) * 10;
    activeOutputState[i]  = Dcc.getCV( 34 + ( i * 2 ) );
  }
#endif

#ifdef DEBUG_MSG
  Serial.print("initPinPulser: DCC Turnout Base Address: "); Serial.print(BaseTurnoutAddress, DEC);
  Serial.print(" CDU Recharge: "); Serial.print(cduRechargeMs);
#ifdef SINGLE_PULSE
  Serial.print(" Active Pulse: "); Serial.print(onMs);  
  Serial.print("ms Active Output State: "); Serial.println(activeOutputState ? "HIGH" : "LOW" );
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
  pinPulser.init(*onMs, cduRechargeMs, *activeOutputState);
#endif
}

void setup()
{
  Serial.begin(115200);
  uint8_t maxWaitLoops = 255;
  while(!Serial && maxWaitLoops--)
    delay(20);
    
  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion. 
#ifdef digitalPinToInterrupt
  Dcc.pin(DCC_PIN, 0);
#else
  Dcc.pin(0, DCC_PIN, 1);
#endif
  
  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, DCC_DECODER_VERSION_NUM, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );

#ifdef DEBUG_MSG
//  Serial.print("\nNMRA DCC 8-Turnout Accessory Decoder. Ver: "); Serial.println(DCC_DECODER_VERSION_NUM,DEC);
  Serial.print("Rosscoe Train DCC 8 Turnout Accessory Decoder. Ver: "); Serial.println(DCC_DECODER_VERSION_NUM,DEC);
#endif  

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
  Serial.println("Resetting CVs to Factory Defaults");
  notifyCVResetFactoryDefault(); 
#endif

  if( FactoryDefaultCVIndex == 0)	// Not forcing a reset CV Reset to Factory Defaults so initPinPulser
	  initPinPulser();  
}

void loop()
{
  static int learningbuttonOldval = 0,learningbuttonVal = 0;

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

  if (learningbuttonOldval != learningbuttonVal) {
    learningMode = learningbuttonVal;
    if (learningMode == HIGH) showAcknowledge(3);
   }
  learningbuttonOldval = learningbuttonVal;
#endif


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

}

void notifyCVChange(uint16_t CV, uint8_t Value)
{
#ifdef DEBUG_MSG
  Serial.print("notifyCVChange: CV: ") ;
  Serial.print(CV,DEC) ;
  Serial.print(" Value: ") ;
  Serial.println(Value, DEC) ;
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
  Serial.println("notifyCVAck") ;
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


  
