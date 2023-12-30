#include "PinPulser.h"

#define PIN_PULSER_SLOT_EMPTY 255

#ifdef SINGLE_PULSE
void PinPulser::init(uint16_t onMs, uint16_t cduRechargeMs, uint8_t activeOutputState)
#else
void PinPulser::init(uint16_t &onMs, uint16_t cduRechargeMs, uint8_t &activeOutputState)
#endif
{
#ifdef SINGLE_PULSE
  this->onMs = onMs;
  this->activeOutputState = activeOutputState;
#else
  this->onMs = onMs;
  this->activeOutputState = activeOutputState;
#endif

  this->cduRechargeMs = cduRechargeMs;
  state = PP_IDLE;
  targetMs = 0;
  memset(pinQueue, PIN_PULSER_SLOT_EMPTY, PIN_PULSER_MAX_PINS + 1);
}

uint8_t PinPulser::addPin(uint8_t Pin)
{
//  Serial.print(" PinPulser::addPin: "); Serial.print(Pin,DEC);
  
  for(uint8_t i = 0; i < PIN_PULSER_MAX_PINS; i++)
  {
    if(pinQueue[i] == Pin)
    {
//      Serial.print(" Already in Index: "); Serial.println(i,DEC);
      return i;
    }

    else if(pinQueue[i] == PIN_PULSER_SLOT_EMPTY)
    {
//      Serial.print(" pinQueue Index: "); Serial.println(i,DEC);
      pinQueue[i] = Pin;
      process();
      return i;
    }
  }  

//  Serial.println();
  return PIN_PULSER_SLOT_EMPTY;
}

PP_State PinPulser::process(void)
{
  unsigned long now;
  
  switch(state)
  {
  case PP_IDLE:
    if(pinQueue[0] != PIN_PULSER_SLOT_EMPTY)
    {
//      Serial.print(" PinPulser::process: PP_IDLE: Pin: "); Serial.println(pinQueue[0],DEC);

#ifdef SINGLE_PULSE
      digitalWrite(pinQueue[0], activeOutputState);
      targetMs = millis() + onMs;
#else
      digitalWrite(pinQueue[0], activeOutputState[pinQueue[0]/2]);
      targetMs = millis() + onMs[pinQueue[0]/2];
#endif
      state = PP_OUTPUT_ON_DELAY;
    }
    break;
      
  case PP_OUTPUT_ON_DELAY:
    now = millis();
    if(now >= targetMs)
    {
//      Serial.print(" PinPulser::process: PP_OUTPUT_ON_DELAY: Done Deactivate Pin: "); Serial.println(pinQueue[0],DEC);

#ifdef SINGLE_PULSE
      digitalWrite(pinQueue[0], !activeOutputState);
#else
      digitalWrite(pinQueue[0], !activeOutputState[pinQueue[0]/2]);
#endif
      targetMs = now + cduRechargeMs;
      memmove(pinQueue, pinQueue + 1, PIN_PULSER_MAX_PINS);
      state = PP_CDU_RECHARGE_DELAY;
    }
    break;

  case PP_CDU_RECHARGE_DELAY:
    now = millis();
    if(now >= targetMs)
    {
      if(pinQueue[0] != PIN_PULSER_SLOT_EMPTY)
      {
//        Serial.print(" PinPulser::process: PIN_PULSER_SLOT_EMPTY: Done Deactivate Pin: "); Serial.println(pinQueue[0],DEC);
        
        digitalWrite(pinQueue[0], activeOutputState);
        targetMs = now + onMs;
        state = PP_OUTPUT_ON_DELAY;
      }
      else
      {
//        Serial.println(" PinPulser::process: PP_CDU_RECHARGE_DELAY - Now PP_IDLE");
        state = PP_IDLE;
      }
    }
    break;
  }
  return state;
}



  
