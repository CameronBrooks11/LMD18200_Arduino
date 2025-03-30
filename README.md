# LMD18200 Arduino Library

This Arduino library provides an easy-to-use interface for controlling the Texas Instruments LMD18200 motor driver IC. The LMD18200 is a 3A, 55V H-bridge designed for driving DC and stepper motors, and it supports features such as current sensing, thermal shutdown, and configurable PWM control. This library supports both sign/magnitude control and active braking modes.

## Features

- **Speed and Direction Control:** Set motor speed with a signed value (positive for one direction, negative for the opposite).
- **Braking:** Activate an active braking mode.
- **Current Sensing:** Read the current drawn by the motor via an external resistor.
- **Thermal Monitoring:** Check the thermal warning flag status.
- **Easy Integration:** A simple API wrapped in a C++ class.

## Installation

In the future the goal is to submit to be added to the official Arduino library index...but until then:

1. Download or clone the repository.
2. Copy the `LMD18200_Arduino` folder into your Arduino libraries directory.
3. Restart the Arduino IDE.
4. Open an example sketch from the **File > Examples** menu to get started.

## References

- Texas Instruments LMD18200 Datasheet (Rev. F) found [here](https://www.ti.com/lit/ds/symlink/lmd18200.pdf)

## Author

Cameron K. Brooks – [cbrook49@uwo.ca](mailto:cbrook49@uwo.ca)
