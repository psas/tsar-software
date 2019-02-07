# Central Manager
Software Controller for TSAR in C++14. Designed for a Raspberry Pi3 with RT Preempt patch.


## Description
Governor
+ Starts system
+ Creates and controlls all threads

Sequencer
+ Contains the control theory.
+ Checks if the system needs to go into an emergency state.
+ Process commands from clients.

Hardware Controller
+ Reads sensor values.
+ Controls hardware.

Link Logger
+ Uses [RAPIDJSON](https://github.com/Tencent/rapidjson).
+ Converts all recieved data into JSON strings for saving and for the server to send.
+ Converts receive JSON strings from the server into a struct for the sequencer to process.

Server
+ Asynchronous TCP server.
+ Sends any strings given by LinkLogger to clients.
+ Receives any strings from clients to give to the LinkLogger.


## Progress
- [ ] Skeletal Protoype
  - [x] Governor
    - [x] Makes all threads needed
    - [x] Handles threads stopping early
  - [ ] Sequencer
    - [ ] Working test sequence
    - [ ] Emergency state check
  - [ ] Hardware Controller
    - [x] I2C
    - [x] GPIO pins
    - [ ] UART 
  - [x] LinkLogger
    - [x] Send/Recv Queue
    - [x] Send data to JSON
    - [x] Sequencer status to JSON
    - [x] JSON to commands
  - [x] Server
    - [x] General TCP Server
    - [x] Send/Recv Queues


## Testing
- [ ] Skeletal Protoype
  - [ ] Sequencer
  - [ ] Hardware Controller
  - [ ] LinkLogger
  - [ ] Server
