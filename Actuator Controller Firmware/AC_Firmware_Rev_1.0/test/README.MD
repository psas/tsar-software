# Required Tools
- PC
- STM32CubeProgrammer
- STM32CubeIDE 1.0.1 or later
- NUCLEO-G071RB Evaluation Board
- USB Micro-B Cable
- Logic Analyzer
- Serial Emulator
- USB to Serial Converter
# Testing
## Installation/Build
- Verify JP2 jumper is connected to STLK
- Connect USB cable to PC and NUCLEO board
- Start STM32CubeProgrammer
- Click Connect and Verify Connectivity
- Disconnect and close STM32CubeProgrammer
- Start STM32CubeIDE 1.0.1
- Change Workspace to Appropriate Directory
- Launch .cproject file from TSAR\actuator-controller
- Right click project Actuator-Controller
    - Click Clean Project
- Right click project Actuator-Controller
    - Click Build Project
- Right Click project Actuator-Controller
    - Drill into Debug As
    - Click 2 STM32 Cortex-M
- Verify in STM32CubeIDE console view that target build and upload was successful
## State Machine
- Disconnect USB micro-b cable
- Change JP2 Jumper from STLK to Vin
- Connect USB to Serial Converter to UART pins
- Connect Logic Analyzer to GPIO Outputs
- Provide Power (3-5V) to Nucleo Board via discrete pins
- Verify Safety state valve configuration on logic analyzer
- Verify safety state log messages on serial emulator
- Provide arm command from serial emulator
- Verify state machine processes each state as follows
- Verify Valve Configuration with Logic Analyzer
- Verify State Log Messages with Serial Emulator
- Verify Logs and Valve configurations Match
## Messages
- From messages received in previous steps
- Verify raw messages match expected values
## Arm/Disarm
- Repeat setup steps for State Machine
- Send ACK message via serial emulator
- Verify that machine does not change states
    - Verify GPIO’s with Logic Analyzer
    - Verify Logs with serial emulator
- Send pseudorandom bit streams to statemachine
- Verify that machine does not change states
- Arm functionality tested in State Machine section
## Failure
- Repeat setup steps for state machine
- Provide a “HIGH” signal to one of the GPIO inputs
- Send ARM command to start machine
- Verify Logs Show Failure state with Serial Emulator
- Verify that GPIO outputs return to safe state with logic analyzer
- Wait 2 minutes
- Verify that state has not changed
- Send ARM command
- Verify that state does not change
- Send set of pseudorandom bit streams
- Verify state does not change
- Send ACK command
- Verify state changes to Safety
