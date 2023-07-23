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




## Current State of the Project




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
- 

If you used any third-party assets that require attribution, list the creators with links to their primary web presence in this section.
- 
