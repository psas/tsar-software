# Central Manager
Software Controller for TSAR in C++. Designed for a Raspberry Pi3 with RT Preempt patch.


## Description
Sequencer 2 threads
+ High thread checks if CM needs to go into an emergency state, highest priority thread.
+ The main thread contains the control theory and process commands from clients,  medium priority thread.

Hardware Controller thread
+ Reads sensor values.
+ Controls hardware.
+ Second highest priority thread.

LinkLogger thread
+ Uses [RAPIDJSON](https://github.com/Tencent/rapidjson).
+ Converts all sensor data into JSON strings for saving and for the server to send.
+ Converts receive JSON strings from the server into a struct for the sequencer to process.
+ Only thread that can talk to the server.
+ Second lowest priority thread.

Server thread
+ Asynchronous TCP server.
+ Sends JSON strings given by LinkLogger to clients.
+ Receives JSON strings from clients to give to the LinkLogger.
+ Lowest prioirity thread.


## Progress
- [ ] Skeletal Protoype
  - [ ] Central Manager (Class)
    - [x] Make Threads
    - [ ] Thread Priority
  - [x] Sequencer
    - [x] Sequence
    - [x] Emergency State Check
  - [ ] Hardware Controller
    - [x] Thread Driver
    - [x] Test RNG data
    - [ ] I2C
    - [x] GPIO pins
    - [ ] UART 
  - [x] LinkLogger
    - [x] Make Server Thread
    - [x] Send data buffer (Queue)
    - [x] Send data to JSON
    - [x] JSON to commands
  - [x] Server
    - [x] General TCP Server
    - [x] Send/Recv string buffers (Queues)


## Testing
- [ ] Skeletal Protoype
  - [ ] Sequencer
  - [x] Hardware Controller
  - [x] LinkLogger
  - [x] Server
