# Safety Bar Valve Indicator Code


## Valve Indicator

Purpose:
This system will collect valve position data using hall effect sensors mounted on nine solenoid valves and will report the current commanded state of the valves. 
The processor will compare the expected valve states with the correct reported valve states, 
if the states do not match the processor will output an indication over the LED arrays.
The LED arrays will display specific binary codes to indicate which valve does not match. 
If multiple valves do not match then the LED arrays will display all binary codes one after another based upon priority information.

Inputs:
- POE+ 24V Power
- UART Data
- Nine Hall Effect Sensors
- ST-Link V2 Emulator

Processor:
- STM32F042K

Outputs:
- Three LED Arrays
- One Buzzer
- SD Card Data Backup

### Drivers In Use

- CMSIS
- STM32F0xx HAL

### Safety Bar Flowchart:
![flowchart](https://github.com/psas/tsar-software/blob/master/images/flowchart_software_valve_ind.png?raw=true)

### Safety Bar State Diagram:
![state](https://github.com/psas/tsar-software/blob/master/images/state_diagram_software_valve_ind.png?raw=true)


