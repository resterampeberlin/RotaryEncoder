//!
//! @file RotaryEncoder.h
//! @author Markus Nickels
//! @version 1.0.0
//! @copyright GNU General Public License V3
//!
//! Example for the RotaryEncoder class
//!
//! This example shows status and value of the associated rotary encoder
//!

// This file is part of the library "RotaryEncoder".
//
// RotaryEncoder is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RotaryEncoder is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RotaryEncoder.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __ROTARYENCODER_H__
#define __ROTARYENCODER_H__

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

//!
//! Main class for rotary encoder library
//!
//! Defines status, control and behaviour of a rotary encoder
//!
//! @todo write an interrupt driven version of this class
//!

class RotaryEncoder {
public:
    
    //!
    //! Definition of various encoder states
    //!

    typedef enum {
        None               = 0,              //!< No movement
        Forward            = 1,              //!< Forward movement
        Reverse            = 2,              //!< Reverse movement
        ButtonPressed      = 4,              //!< Switch pressed
        ButtonLongPressed  = 8,              //!< Switch pressed after timeout
        ButtonReleased     = 16              //!< Switch released
    } encoderStatus_t;
    
    //!
    //! Class constructor
    //!
    //! Initialises data
    //!
    //! @param aPhaseAPin pin number of phase A connection
    //! @param aPhaseBPin pin number of phase B connection
    //! @param aSwitchPin pin number of switch connector, "-1" disables switch
    //!
    
    RotaryEncoder(uint8_t aPhaseAPin,
                  uint8_t aPhaseBPin,
                  int8_t aSwitchPin = -1);
    
    //!
    //! Start working and configure IO
    //!
    
    void begin(void);

    //!
    //! end working
    //!

    void end(void);

    //!
    //! set timeout for the RotaryEncoder::ButtonLongPressed status
    //!
    //! Sets the time in milliseconds, after which the pressed
    //! switch is considered as RotaryEncoder::ButtonLongPressed.
    //! "0" disables timeout.
    //!
    //! @param aTimeout time in milliseconds
    //! @sa getStatus(), waitForStatus(), ButtonLongPressed
    //!
    
    void setSwitchTimeout(unsigned long aTimeout);
    
    //!
    //! set range of counter
    //!
    //! Sets range and stepwidth of internal counter.
    //! With each turn the counter isincreases or decreases by aStepWidth
    //!
    //! @param aLowerLimit, aUpperLimit Upper and lower limit of counter
    //! @param aStepWidth step width of counter
    //! @sa setCount(), getCount()
    //!

    void setRange(int aLowerLimit,
                  int aUpperLimit,
                  unsigned int aStepWidth = 1);
    
    //!
    //! set counter value
    //!
    //! initialises counter with a certain value
    //!
    //! @param aCount new counter value, must be with upper and lower limit
    //! @sa getCount(), setRange();
    //!
    
    void setCount(int aCount);

    //!
    //! get counter value
    //!
    //! returns current counter value
    //!
    //! @return current counter value
    //! @sa setCount(), setRange();
    //!
    
    int getCount(void);

    //!
    //! get encoder status
    //!
    //! returns after a full cycle forward or backward, if knob is pressed the
    //! switch status
    //! if nothing happens "none" is returned.
    //! This routine has to be called in short cycles to detect encoder
    //! movement.
    //! If movement is detected, the counter will be increased/decreased
    //!
    //! @return current encoder status
    //!

    encoderStatus_t getStatus(void);

    //!
    //! wait until a state change happens
    //!
    //! waits either until a movement is found or switch is pressed or released.
    //! If movement is detected, the counter will be increased/decreased
    //!
    //! @return current encoder status
    //! @sa setSwitchTimeout()
    //!

    encoderStatus_t waitForStatus(void);
    
protected:
    uint8_t phaseAPin;                  //!< pin assigned to phase A
    uint8_t phaseBPin;                  //!< pin assigned to phase B
    int8_t  switchPin;                  //!< pin assigned to switch
    
    int lowerLimit;                     //!< lower limit of counter
    int upperLimit;                     //!< upper limit if count
    unsigned int stepWidth;             //!< step width of counter
    int count;                          //!< current counter
    
    int encoderStatus;                  //!< current status of encoder
    int switchStatus;                   //!< current status of switch
    unsigned long timeout;              //!< timeout switch
    unsigned long timePressed;          //!< time when switch was pressed
    
    encoderStatus_t lastResult;         //!< remember result of getStatus()
};

//!
//! @name operators for datatype encoderStatus_t
//!
//!@{

constexpr RotaryEncoder::encoderStatus_t operator |
    (RotaryEncoder::encoderStatus_t X, RotaryEncoder::encoderStatus_t Y);

RotaryEncoder::encoderStatus_t& operator |=
    (RotaryEncoder::encoderStatus_t& X, RotaryEncoder::encoderStatus_t Y);

//!@}

#endif

//!
//! @mainpage Store and show sensor data recored from a remote sensor
//!
//! @tableofcontents
//!
//! @section s1 Purpose
//!
//! This arduino library
//!
//! @section s2 Compatibility
//!
//! This library has been tested with
//! - ESP8266
//! - Arduino Uno
//! - Adafruit rotary encoder breakout
//!
//! @section s3 Software Installation
//!
//! @section pinout Hardware Installation / Pinout
//!
//!  | Encoder Pin | Direction | Meaning        | MCU Pin                       |
//!  | ------------| --------- | -------------- | ----------------------------- |
//!  | A           | Out       | Phase A output | As defined in RotaryEncoder() |
//!  | B           |           | Phase B output | As defined in RotaryEncoder() |
//!  | C           | Out       | Common         | VCC/GND                       |
//!  | SWITCH      | Out       | Switch Output  | As defined in RotaryEncoder() |
//!
//!
//! @section vh Version History
//!
//! - Version 1.0.0
//!
//! @section kb Known bugs
//!
//! - xxx
//!
//! @section cr Credits
//!
//! xxx
//!

