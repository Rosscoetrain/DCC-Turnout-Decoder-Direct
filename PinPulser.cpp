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

// Based on the NMRA Pulsed 8 stationary decoder

#include "PinPulser.h"

// define empty pin slot value

#define PIN_PULSER_SLOT_EMPTY 255

#ifdef SINGLE_PULSE
void PinPulser::init(uint16_t onMs, uint16_t cduRechargeMs, uint8_t activeOutputState)
#else
//void PinPulser::init(uint16_t &onMs, uint16_t cduRechargeMs, uint8_t &activeOutputState)
void PinPulser::init(uint16_t onMs_[], uint16_t cduRechargeMs, uint8_t activeOutputState_[], byte outputs_[])
#endif
{
#ifdef SINGLE_PULSE
  this->onMs = onMs;
  this->activeOutputState = activeOutputState;
#else
  this->onMs = onMs_;
  this->activeOutputState = activeOutputState_;
  this->outputs = outputs_;

/*
#ifdef DEBUG_MSG
  for(uint8_t i=0; i < 8; i++)
   {
    Serial.print(F("index : "));Serial.print(i);Serial.print(F(" onMs : "));Serial.println(onMs[i]);
    Serial.print(F("index : "));Serial.print(i);Serial.print(F(" activeOutputState : "));Serial.println(activeOutputState[i]);
   }
#endif
*/

#endif

  this->cduRechargeMs = cduRechargeMs;
  state = PP_IDLE;
  targetMs = 0;
  memset(pinQueue, PIN_PULSER_SLOT_EMPTY, PIN_PULSER_MAX_PINS + 1);
}

uint8_t PinPulser::addPin(uint8_t Pin)
{
#ifdef DEBUG_MSG
  Serial.print(" PinPulser::addPin: "); Serial.print(Pin,DEC);
#endif
  for(uint8_t i = 0; i < PIN_PULSER_MAX_PINS; i++)
  {
    if(pinQueue[i] == Pin)
    {
#ifdef DEBUG_MSG
      Serial.print(F(" Already in Index: ")); Serial.println(i,DEC);
#endif
      return i;
    }

    else if(pinQueue[i] == PIN_PULSER_SLOT_EMPTY)
    {
#ifdef DEBUG_MSG
      Serial.print(F(" pinQueue Index: ")); Serial.println(i,DEC);
#endif
      pinQueue[i] = Pin;
      process();
      return i;
    }
  }  

#ifdef DEBUG_MSG
  Serial.println();
#endif
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


#ifdef DEBUG_MSG
      Serial.print(F(" PinPulser::process: PP_IDLE: Pin: ")); Serial.println(pinQueue[0],DEC);
#endif

#ifdef SINGLE_PULSE
      digitalWrite(pinQueue[0], activeOutputState);
      targetMs = millis() + onMs;
#else

#ifdef DEBUG_MSG
      Serial.print(F(" PinPulser::process: PP_IDLE:  onMs: ")); Serial.println(getOnMs(pinQueue[0]),DEC);
      Serial.print(F(" activeOutputState : "));Serial.println(getActiveOutputState(pinQueue[0]));
#endif

      digitalWrite(pinQueue[0], getActiveOutputState(pinQueue[0]));
      targetMs = millis() + getOnMs(pinQueue[0]);

#ifdef DEBUG_MSG
      Serial.print(F(" millis : "));Serial.print(millis());Serial.print(F(" targetMs : "));Serial.println(targetMs);
#endif

#endif

      state = PP_OUTPUT_ON_DELAY;
    }
    break;
      
  case PP_OUTPUT_ON_DELAY:
    now = millis();
    if(now >= targetMs)
    {

#ifdef DEBUG_MSG
      Serial.print(F(" now : "));Serial.print(now);Serial.print(F(" targetMs : "));Serial.println(targetMs);
      Serial.print(F(" PinPulser::process: PP_OUTPUT_ON_DELAY: Done Deactivate Pin: ")); Serial.println(pinQueue[0],DEC);
#endif

#ifdef SINGLE_PULSE
      digitalWrite(pinQueue[0], !activeOutputState);
#else
      digitalWrite(pinQueue[0], !getActiveOutputState(pinQueue[0]));
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

#ifdef DEBUG_MSG
        Serial.print(F(" PinPulser::process: PIN_PULSER_SLOT_EMPTY: Done Deactivate Pin: ")); Serial.println(pinQueue[0],DEC);
#endif

#ifdef SINGLE_PULSE
        digitalWrite(pinQueue[0], activeOutputState);
        targetMs = now + onMs;
#else
        digitalWrite(pinQueue[0], getActiveOutputState(pinQueue[0]));
        targetMs = now + getOnMs(pinQueue[0]);
#endif

        state = PP_OUTPUT_ON_DELAY;
      }
      else
      {

#ifdef DEBUG_MSG
        Serial.println(F(" PinPulser::process: PP_CDU_RECHARGE_DELAY - Now PP_IDLE"));
#endif

        state = PP_IDLE;
      }
    }
    break;
  }
  return state;
}


#ifndef SINGLE_PULSE
void PinPulser::printArrays()
 {
#ifdef ENABLE_SERIAL
  for(uint8_t i = 0; i < NUM_TURNOUTS; i++)
   {
    Serial.print(F(" output : "));Serial.print(i+1);Serial.print(F(" onMs : "));Serial.print(onMs[i]);Serial.print(F(" activeOutputState : "));Serial.println(activeOutputState[i]);
   }
#endif
 }


uint16_t PinPulser::getOnMs(uint8_t pin)
 {
  int wantedpos = -1;
  for (int i = 0; i < NUM_TURNOUTS * 2; i++)
   {
    if (pin == outputs[i])
     {
      wantedpos = i;
      break;
     }
   }
  if (wantedpos != -1)
    {
      return onMs[wantedpos/2];
    }
   return 10;
 }

uint8_t PinPulser::getActiveOutputState(uint8_t pin)
 {
  int wantedpos = -1;
  for (int i = 0; i < NUM_TURNOUTS * 2; i++)
   {
    if (pin == outputs[i])
     {
      wantedpos = i;
      break;
     }
   }
  if (wantedpos != -1)
    {
      return activeOutputState[wantedpos/2];
    }
   return 1;
 }

#endif

  
