# Actuator Controller Firmware
### State Diagram
![Peeet](resources/TSAR_Simple_State.png "Title")
The state configuration above allows the controller to be powered on without risk to technicians working on the test stand setup.  A basic breakdown of each of the states is as follows:

### Safety
#### Valve Configuration: SOV4, SOV8 On; Else Off
This is the initial state when the device is powered up.  In this state all of the solenoids are in their nominal positions and the statemachine can not move until it is explicitly sent an ARM command. It is possible that innocents will be present in this state.

### Valve Check
#### Valve Configuration: Cycle all Valves
In this state the valves will all be cycled to ensure that they are working prior to introducing fuel to the system.

### Fuel Fill
#### Valve Configuration: SOV4, SOV8 ON; Else Off
Introduction of the fuel to the Test Stand.

### LOX Pre-Fill
#### Valve Configuration: SOV2, SOV6 ON; Else Off
**It is safe to move out of this state if and only if the site is clear and there are no humans (or other animals) within 1000 ft of the test stand.**

### LOX Chill
#### Valve Configuration: SOV6, SOV7, SOV8 ON; Else Off
LOX just needs to chill out for a little while.

### LOX Fill
#### Valve Configuration: SOV6, SOV7, SOV8 ON; Else Off
__TBD__

### READY!
#### Valve Configuration: SOV4, SOV8 ON; Else Off
Take time to make sure we really want to do this.  Ensure system and people are safe.

### Pressure
#### Valve Configuration: SOV1, SOV2 ON; Else Off
Turn LOX and fuel pressure valves on.  System will build pressure and is extremely dangerous.

### Ignition
#### Valve Configuration SOV1, SOV2 On; Else Off
Start the igniters in preparation for fuel and lox mixture.

### LOX Start
#### Valve Configuration SOV1, SOV2, SOV6 On; Else Off
Open main LOX valve to start oxidizer flow to combustion chamber.

### Fuel Start
#### Valve Configuration SOV1, SOV2, SOV5, SOV6 On; Else Off
Open main fuel valve to start fuel flow to combustion chamber.  Things should start burning.

### Firing
#### Valve Configuration: SOV1, SOV2, SOV5, SOV6 On; Else Off
Things are on fire.

### Purge
#### Valve Configuration 2: SOV3 On; Else Off
Open LOX and fuel pressure lines; Open N2 purge valve, force oxidizer from system.

### LOX Boiloff
#### Valve Configuration SOV2, SOV6 On; Else Off
If you ignore the problem long enough it will go away.

### Shutdown
#### Valve Configuration: SOV3 On; Else Off
Re-Open N2 Purge valve.

### Fuel Drain
#### Valve Configuration: SOV4, SOV8 On; Else Off
Post fire, at this stage it is safe to approach the test stand.  All valves in nominal position, no means to actuate solenoids. Remove any unburned fuel.

### Failure
#### Valve Configuration: SOV3 On; Else Off
Any detected failure or ESTOP command will send to this state.  Cannot leave until an ACK is recieved from test controller.

### Test
#### Valve Configuration:  ANY
From the test state an engineer should be able to enter any other state for testing purposes, this should be disabled for the release build.
