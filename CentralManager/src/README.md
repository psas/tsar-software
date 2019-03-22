# Class and Struct Descriptions

## Main Class Descriptions
These are the main class for Central Manager.
All of the main classes have their own thread.

Governor
+ Starts system
+ Creates and controlls all threads

Sequencer
+ Sequences through a list of step for a liquid engine test fire.
+ Checks if the system needs to go into an emergency state.
+ Processes commands from clients (start, emergency stop, halt, continue, end, etc).

Hardware Controller
+ Hardware interface for Central Manager
+ Reads data form I2C sensor.
+ Controls GPIO conections.
+ Read hearbeats from Actuator Controller system using UART.
+ Sends commands and hearbeats to Actuator Controller system using UART.

Link Logger
+ Converts all Sequencer and Hardware status/data into JSON strings for saving and for the server to send.
+ Converts receive JSON strings from the Server into a struct for the Sequencer to process.
+ Save all data sent

Server
+ Asynchronous TCP server.
+ Sends any strings given by Link Logger to clients.
+ Receives any strings from clients to give to the Link Logger.

## POD (plain old data) Structs Descriptions
These structs are mostly just a POD struct, but they all have functions to convert to or from strings.
All POD Classes use [RAPIDJSON](https://github.com/Tencent/rapidjson) for struct to string conversions.
These struct are uses by the main classes to send data to one another.

Sequence Status
+ Hold a snap shot of the sequencer satuts to send to cleints

Hardware Data Frame
+ Hold a snap shot of i2c status/data
+ Hold a snap shot of gpio status/data
+ Hold a snap shot of AC status/data
+ Sequencer uses this data for sequencing
+ A copy is sent to clients

Client Command
+ A command for the sequncer to do (start, emergency stop, halt, continue, end, etc).

## Data Structures Class(es) Decriptions
Fixed Queue (a c++ template class)
+ This is queue that has a fixed size to greaty reduce dynamic memory allocation and destruction.
+ If the queue becomes full, any new data will override oldest queued data.
+ Both Link Logger and Server have a Fixed Queue for data needing to be sent.


# Progress
- [ ] Skeletal Protoype
  - [x] Governor
    - [x] Makes all threads needed
    - [x] RT Thread priority
    - [x] Handles threads stopping early
  - [ ] Sequencer
    - [x] Working test sequence
    - [x] Emergency state check
    - [ ] Client command reader
  - [ ] Hardware Controller
    - [x] I2C example reader
    - [x] GPIO pins control
    - [x] GPIO control function for Sequencer to call
    - [x] UART protocal to AC (with a heartbeat timeout)
    - [ ] Control function for sending new UART message to AC
  - [x] LinkLogger
    - [x] Send/Recv Queues
    - [x] Outgoing data to JSON
    - [x] Incoming JSON to Commands data
  - [ ] Server
    - [x] General TCP Server
    - [x] Send/Recv Queues
    - [x] Test read only client
    - [ ] Test Asynchronous read/write client
