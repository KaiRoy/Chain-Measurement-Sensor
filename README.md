# Chain-Measurement-Sensor
The focus of this project was to create a Sensor that measures the wear of a chain for our Senior design course. The project partner for this project was the Hyster-Yale Group.

<!-- Insert Image Here -->

<!-- LINK SUMMARY VIDEO SOMEWHERE IN THE README!! -->

## Table of Contents

- [Description](#description)
- [Design](#design)
- [Current State](#current-state-of-the-project)
- [Post Project Notes](#post-project-notes)
- [Installation](#installation) <!-- Should I have Installation and Usage above or below the Design, Current State, Changes, etc? -->
- [Usage](#usage)
- [Credits](#credits)
- [License](#license)

## Description

The goal of this project is to create a chain elongation monitor for the Hyster-Yale Group that will automatically detect a worn lift chain and notify an operator to replace said chain. Chains, especially lifting chains, need to be frequently inspected to ensure they meet specifications. A 3% increase in a chain’s length can reduce its lifting capacity by 15%. This can become a dangerous hazard if out-of-spec chains are not caught. 

Currently, this inspection is either done by hand or by expensive third party devices, thus the creation of a cheap electronic alternative is very desirable. We plan to use three pairs of IR LEDs and IR receivers that will act as our sensors. These pairs will be situated in such a way that the IR “beam” will break when a chain link passes the sensor and reconnect in the gap between chain links. One pair will be used to measure the speed, while the other two will measure the “stretch” of the chain by recording the time difference between activations and calculating a length measurement.

In addition to the sensors, the device will consist of a power circuit to convert a Forklift’s 24V power into 5Vs, a CAN communication circuit, and a microcontroller to calculate the elongation based on the sensors and send those measurements via the CAN Circuit to the Hyster Vehicle. We chose an ATTiny814 for our microcontroller as it is a relatively small and cheap microcontroller and is capable of performing necessary functions, such as basic calculations and time measurement. 

Our project partner has specified that the device needs to meet four requirements:
 - The device needs to measure chain “stretch” (wear) within ±5% of the actual measurement.
 - The device, specifically the electronics, need to cost less than $15 or less at a production quantity of 15,000/yr
 - The device need to be robust enough to survive abrasive (high chain wear) applications
 - The device needs to communicate using the CAN communication protocol

At the end of our project, we were able to meet 3 of the 4 project partner requirements with the accuracy requirement being the one that was not met. This is mostly due to not having enough time to finish experimenting and debugging the code. With better time management and planning, we would have been able to finish the development of our code and could have met the final requirement.


## Designs

For the design portion of this project, the system's design was split into eight blocks with each person handling the design for two blocks. Adam handled the design of the Power Filter and the Power Regulator Blocks. Tianhao designed the Microcontroller and CAN Communication Circuits. The sensor and PCB blocks were handled by Kai. The code and the enclosure blocks were designed by Corbin. 

![System Block Diagram](Assets/ECE%2044x%20Block%20Diagram.png)

<!-- Power Blocks -->
The power converter block is based on an application schematic from the MC34063A-E datasheet to be used as a step-down converter. This block accomplishes the goal of reducing voltage and increasing current from 25V to 5V and current up to 500mA. The goal is to make the voltage and current levels safe for our microcontroller, CAN controller, and sensors to be used properly without overheating. The block contains the MC34063A/E chip itself, IN5819 Zener diode, three resistors (two are external to control the voltage output), three capacitors, and one inductor.

The power filter block is based on the LC filter schematic from [Digikey’s online LC filter calculator](https://www.digikey.com/en/resources/conversion-calculators/conversion-calculator-low-pass-and-high-pass-filter) to be used as a first order LC filter. This block accomplishes the goal of reducing frequency down to a fixed value of about less than 16kHz. This is to ensure that our high noise level of frequency is reduced so that our microcontroller and sensors can operate effectively. The block contains one inductor and one capacitor.

<!-- CAN Controller-->
The can block is built with one of the most practical stand-alone CAN controllers, MCP 2515 as the core. This module achieves the goal of communicating with Hyster Forklift’s electronics via 5 V digital serial signal. After achieving this goal, we can connect our device to the forklift with only two wires, so that the forklift can receive any information accurately. In this block, in addition to MCP 2515, we will also use TJA 1051 as the transceiver of the block to transmit or receive information transmitted through the CAN bus, an 8M Hz crystal oscillator as a bit rate regulator, an LED as an indicator, and some resistors and capacitors to stabilize the voltage and current of the part. In addition, we will also add two terminal block connectors to allow the output of the signal to adapt to more kinds of wires.

<!-- Micrcontroller -->
The Microcontroller block to be used in this project is the ATtiny 814. The module will be responsible for collecting and analyzing signals from the sensor, as well as controlling the operation of the signal transmitter. With the ATtiny 814, the project aims to achieve certain targets, such as measuring chain "stretch" within +/- 5% of the actual measurement, costing less than $10 at a production quantity of 15,000/yr, and outputting an analog (5V) or digital signal (CAN or other serial protocol signal).

<!-- Sensors -->
The Sensor block is implemented using three IR Breakbeam sensors. This document provides an overview of the overall block function including interface properties and a schematic, verification for the design in the form of a step-by-step testing process, and support for the validity of the design in the form of outside research and numerical justification addressing individual properties.

One sensor is placed in the ‘middle’ middle of the device and will be for measuring the speed of the Chain, while two sensors will be placed on the outer edges of the device and will be used for measuring/detecting the elongation of the chain.

The measurement of speed can be done by measuring the time difference between sensor activation as a chain link crosses the two central sensors and calculating the speed with that delta and the chain link length. For the elongation measurement/monitoring, the outer two sensors are placed with a specific distance between them, X amount of chain links. If positioned correctly, the two sensors will activate simultaneously with a chain that is not elongated and activates with some time delta with a chain that is elongated. This time delta coupled with the calculated speed can provide an estimate for the elongation measurement.

<!-- PCB -->
The Final PCB contains most of the circuits for all of the electronics blocks of the system. These circuits include the Power Filter, Power Converter, Microcontroller, and CAN Controller Blocks. This means that all of the outward-facing interfaces for these blocks, such as the sensor, power, and CAN connectors, are the interfaces for this block. 

<!-- Enclosure -->
The enclosure for this project will be a durable Pelican Case, created and distributed by the Pelican company. This ensures that our enclosure will be consistent, professionally made, and strong enough to protect our fragile components from the dangerous environment they will have to be in. Additionally, a piece of acrylic will be laser cut to allow for the mounting of the PCB and the guiding of wires through the walls of the enclosure, which will have holes cut into it.


## Current State of the Project

By the end of the project, we had a mostly working system. 


## Post Project Notes




## Installation

What EDA libraries and files are needed for this project?
- Various Component Libraries



<!--
## Usage

Provide instructions and examples for use. Include screenshots as needed.

To add a screenshot, create an `assets/images` folder in your repository and upload your screenshot to it. Then, using the relative filepath, add it to your README using the following syntax:

    ```md
    ![alt text](assets/images/screenshot.png)
    ```

## Features

If your project has a lot of features, list them here.

## Tests

-->

## Credits

List your collaborators, if any, with links to their GitHub profiles.
- Kai Roy
- 

If you used any third-party assets that require attribution, list the creators with links to their primary web presence in this section.
- 
