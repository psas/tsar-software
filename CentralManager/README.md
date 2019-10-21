# Central Manager
Software Controller for TSAR in C++14. Designed for a Raspberry Pi3 with RT Preempt patch.


## High Level Description
The Central Manager can be broken into 3 parts: the Sequencer, the Hardware Controller, & the Link Logger.

Sequencer
+ Sequences through a list of step for a liquid engine test fire.
+ Processes commands from clients (start, emergency stop, halt, continue, etc).

Hardware Controller
+ Hardware interface for Central Manager
+ Reads data sensor.
+ Sends commands to Actuator Controller system.

Link Logger
+ TCP server (clients are monitoring laptops).
+ Send system status updates and hardware data to clients.
+ Gives any recieved commands from clients to Sequencer for processing.

