/*
 * variables.h
 */


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


#ifdef SINGLE_PLUSE
  {CV_ACCESSORY_DECODER_OUTPUT_PULSE_TIME, 50},   // x 10mS for the output pulse duration
  {CV_ACCESSORY_DECODER_ACTIVE_STATE,    ACTIVE_OUTPUT_STATE},
#else
  {33, 50},
  {34, ACTIVE_OUTPUT_STATE},
  {35, 50},
  {36, ACTIVE_OUTPUT_STATE},
  {37, 50},
  {38, ACTIVE_OUTPUT_STATE},
  {39, 50},
  {40, ACTIVE_OUTPUT_STATE},
  {41, 50},
  {42, ACTIVE_OUTPUT_STATE},
  {43, 50},
  {44, ACTIVE_OUTPUT_STATE},
  {45, 50},
  {46, ACTIVE_OUTPUT_STATE},
  {47, 50},
  {48, ACTIVE_OUTPUT_STATE},
#endif
};



uint8_t FactoryDefaultCVIndex = 0;

// This is the Arduino Pin Mapping to Turnout Addresses with 2 pins per turnout 
// A1 is missing in the sequence as it is used for the DCC ACK
// The Pins are defined in Pairs T=Thrown, C=Closed (Digitrax Notation)  
//   base address 1C 1T 2C 2T 3C 3T 4C 4T 5C  5T  6C  6T  7C  7T  8C  8T
#ifndef SMT_BOARD
//byte outputs[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19};
//     pins         D3 D4 D5 D6 D7 D8 D9 D10 D11 D12 D13  A0  A2  A3  A4  A5  
byte outputs[] = { 4, 3, 6, 5, 8, 7, 10, 9, 12, 11, 14, 13, 17, 16, 19, 18};
//   pins         D4 D3 D6 D5 D8 D7 D10 D9 D12 D11  A0 D13  A3  A2  A5  A4
#else
byte outputs[] = { 4, 3, 6, 5, 8, 7, 10, 9, 11, 12, 14, 13, 17, 16, 19, 18};
//   pins         D4 D3 D6 D5 D8 D7 D10 D9 D11 D12  A0 D13  A3  A2  A5  A4
#endif
