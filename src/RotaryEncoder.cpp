//!
//! @file RotaryEncoder.cpp
//! @author Markus Nickels
//! @version 1.0.0
//! @copyright GNU General Public License V3
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

#include <RotaryEncoder.h>

//!
//! Transistion table
//!
//! Defines the status transition from one status to the other
//! column is status, row is input state
//!

const int transition[4][7] = {
    // 0   1   2   3   4   5   6
    //   (-- Fwd --) (-- Rev --)
    {  0,  1,  2, -1,  4,  5, -2},  // !a && !b
    {  1,  1,  2,  3,  4,  6,  6},  //  a && !b
    {  4,  1,  3,  3,  4,  5,  6},  // !a &&  b
    {  0,  2,  2,  3,  5,  5,  6}}; //  a &&  b

//!
//! Class constructor
//!

RotaryEncoder::RotaryEncoder(uint8_t aPhaseAPin,
                             uint8_t aPhaseBPin,
                             int8_t aSwitchPin) {
    phaseAPin = aPhaseAPin;
    phaseBPin = aPhaseBPin;
    switchPin = aSwitchPin;
    
    lowerLimit = 0;
    upperLimit = 10;
    stepWidth = 1;
    count = 0;
    
    encoderStatus = switchStatus = 0;
    timeout = timePressed = 0;
    
    lastResult = None;
}

//!
//! Start working and configure IO
//!

void RotaryEncoder::begin(void){
    pinMode(phaseAPin, INPUT);
    pinMode(phaseBPin, INPUT);

    if (switchPin >= 0) {
        pinMode(switchPin, INPUT);
    }
    
    encoderStatus = switchStatus = 0;
}

//!
//! end working
//!

void RotaryEncoder::end(void){    
    // currently nothing to do
}

//!
//! set timeout for the "long pressed" status
//!

void RotaryEncoder::setSwitchTimeout(unsigned long aTimeout) {
    timeout = aTimeout;
}

//!
//! set range of counter
//!

void RotaryEncoder::setRange(int aLowerLimit,
                             int aUpperLimit,
                             unsigned int aStepWidth){
    lowerLimit = aLowerLimit;
    upperLimit = aUpperLimit;
    stepWidth = aStepWidth;
    
    if (getCount() < lowerLimit) {
        setCount(lowerLimit);
    }

    if (getCount() > upperLimit) {
        setCount(upperLimit);
    }
}

//!
//! set counter value
//!

void RotaryEncoder::setCount(int aCount){
    if (aCount >= lowerLimit && aCount <= upperLimit) {
        count = aCount;
    }
}

//!
//! get counter value
//!

int RotaryEncoder::getCount(void){
    return count;
}

//!
//! get encoder status
//!

RotaryEncoder::encoderStatus_t RotaryEncoder::getStatus(void){
    int a, b, s = LOW;
    encoderStatus_t result = None;
    
    a = digitalRead(phaseAPin);
    b = digitalRead(phaseBPin);
    
    if (switchPin >= 0) {
        s = digitalRead(switchPin);
    }
    
    // make status transition
    if (encoderStatus < 0) {
        encoderStatus = 0;
    }

    if (a == LOW && b == LOW) {
        encoderStatus = transition[0][encoderStatus];
    }
    
    if (a == HIGH && b == LOW) {
        encoderStatus = transition[1][encoderStatus];
    }
    
    if (a == LOW && b == HIGH) {
        encoderStatus = transition[2][encoderStatus];
    }
    
    if (a == HIGH && b == HIGH) {
        encoderStatus = transition[3][encoderStatus];
    }
    
    // check rotation
    switch (encoderStatus) {
        case -1:
            setCount(getCount()+stepWidth);
            result = Forward;
            break;

        case -2:
            setCount(getCount()-stepWidth);
            result = Reverse;
            break;
    }
    
    // Check switch
    if (s != switchStatus) {
        if (s == HIGH) {
            result |= ButtonPressed;
            timePressed = millis();
        }
        else {
            result |= ButtonReleased;
            timePressed = 0;
        }
        
        switchStatus = s;
    }
    else {
        if (s == HIGH) {
            // Check for long press
            if (timePressed &&
                timeout &&
                millis() - timePressed > timeout) {
                result |= ButtonLongPressed;
                timePressed = 0;
            }
        }
    }
    
    // in case of state transition wait a little bit
    // this may be tuned depending on device
    if (result != lastResult) {
        delay(10);
    }
    
    lastResult = result;
    
    return result;
}

//!
//! wait until a state change happens
//!

RotaryEncoder::encoderStatus_t RotaryEncoder::waitForStatus(void){
    encoderStatus_t s;
    
    while ((s = getStatus()) == None) {
        // necessary on some platforms to perform background tasks
        yield();
    }

    return s;
}

//!
//! operators for datatype encoderStatus_t
//!

constexpr RotaryEncoder::encoderStatus_t operator |
    (RotaryEncoder::encoderStatus_t X, RotaryEncoder::encoderStatus_t Y) {
    return static_cast<RotaryEncoder::encoderStatus_t>
    (static_cast<unsigned int>(X) | static_cast<unsigned int>(Y));
}

RotaryEncoder::encoderStatus_t& operator |=
    (RotaryEncoder::encoderStatus_t& X, RotaryEncoder::encoderStatus_t Y) {
    X = X | Y;
    return X;
}





