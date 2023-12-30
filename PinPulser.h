#include <Arduino.h>

// if only using single on pulse value leave following uncommented
//#define SINGLE_PULSE

#define NUM_TURNOUTS 8

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
#ifdef SINGLE_PULSE
    uint16_t      onMs;
    uint8_t       activeOutputState = HIGH;
#else
    uint16_t *    onMs;
    uint8_t *     activeOutputState;
#endif
    uint16_t      cduRechargeMs;
    PP_State      state = PP_IDLE;
    unsigned long targetMs = 0;
    uint8_t       pinQueue[PIN_PULSER_MAX_PINS + 1];

  public:
#ifdef SINGLE_PULSE
    void init(uint16_t onMs, uint16_t cduRechargeMs, uint8_t activeOutputState);
#else
    void init(uint16_t &onMs, uint16_t cduRechargeMs, uint8_t &activeOutputState);
#endif
    uint8_t addPin(uint8_t pin);
    PP_State process(void);
};

  
