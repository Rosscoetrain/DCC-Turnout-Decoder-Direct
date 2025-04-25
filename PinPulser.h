/*
 *
 * @file PinPulser.h
 * 
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

#ifndef PINPUSHER_H
#define PINPUSHER_H

#include <Arduino.h>
#include "defines.h"

// this is the maximum number of output pins

#define PIN_PULSER_MAX_PINS    16

enum PP_State
{  
  PP_IDLE = 0,
  PP_OUTPUT_ON_DELAY,
  PP_CDU_RECHARGE_DELAY,
};

class PinPulser
{
  private:
    uint16_t      cduRechargeMs;
    PP_State      state = PP_IDLE;
    unsigned long targetMs = 0;
    uint8_t       pinQueue[PIN_PULSER_MAX_PINS + 1];


#ifdef SINGLE_PULSE
    uint16_t      onMs;
    uint8_t       activeOutputState = HIGH;
#else
    uint16_t *onMs;
    uint8_t *activeOutputState;
    byte *outputs;
#endif

  public:
#ifdef SINGLE_PULSE
    void init(uint16_t onMs, uint16_t cduRechargeMs, uint8_t activeOutputState);
#else
//    void init(uint16_t &onMs, uint16_t cduRechargeMs, uint8_t &activeOutputState);
    void init(uint16_t onMs_[], uint16_t cduRechargeMs, uint8_t activeOutputState_[], byte outputs_[]);
#endif
    uint8_t addPin(uint8_t pin);
    PP_State process(void);
#ifndef SINGLE_PULSE
    void printArrays();

  private:
    uint16_t getOnMs(uint8_t pin);
    uint8_t getActiveOutputState(uint8_t pin);
#endif
};


#endif

  
