
//!
//! @file SimpleRotaryEncoder.ino
//! @author Markus Nickels
//! @version 1.0.0
//! @copyright GNU General Public License V3
//!
//! Simple example for class RotaryEncoder
//!
//! This example shows continously encoder status and value
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
//! our encoder
//!

RotaryEncoder encoder(4, 5, 14);

//!
//! Setup routine
//!

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup");

  encoder.begin();

  // configure encoder behauviour
  encoder.setRange(-1000, 1000, 10);
  encoder.setCount(0);
  encoder.setSwitchTimeout(500);

  Serial.println("Setup complete");
}

//!
//! Main routine
//!

void loop() {
  RotaryEncoder::encoderStatus_t s;

  s = encoder.waitForStatus();

  if (s & RotaryEncoder::Forward) {
      Serial.print("Forward");
  }

  if (s & RotaryEncoder::Reverse) {
      Serial.print("Reverse");
  }

  if (s & RotaryEncoder::ButtonPressed) {
      Serial.print("ButtonPressed");
  }

  if (s & RotaryEncoder::ButtonReleased) {
      Serial.print("ButtonReleased");
  }
  
  if (s & RotaryEncoder::ButtonLongPressed) {
      Serial.print("ButtonLongPressed");
  }

  Serial.print("\tValue=");
  Serial.println(encoder.getCount());
}
