# Actuator Controller Firmware
### Simplified State Diagram
![Peeet](resources/TSAR_Simple_State.png "Title")
The state configuration above allows the controller to be powered on without risk to technicians working on the test stand setup.  A basic breakdown of each of the states is as follows:

### Safety
#### Valve Configuration: VV1, VV2 On; Else Off
This is the initial state when the device is powered up.  In this state all of the solenoids are in their nominal positions and the statemachine can not move until it is explicitly sent a Force Disarm command. This state encomasses several sub-states where technicians will be physically present. It is safe to move out of this safe when the site is clear and there are no humans within 1000 ft of the test stand.

### Pre-Fire
#### Valve Configuration: PV2,PV3, MV2* On; Else Off
In this state the machine is waiting for the system to pressurize and for the fuel lines to cool.

### Ignition
#### Valve Configuration PV2, PV3, IV1, IV2, MV1* On; Else Off
In this fuel is introduced to the chamber and the igniter is lit.

### Burn Initiated
#### Valve Configuration: PV2,PV3, IV1, IV2, MV1, MV2 On; Else Off
Things are on fire.

### Burn Termination 1 & 2
#### Valve Configuration 1: PV2,PV3, MV1, MV2* On; Else Off
#### Valve Configuration 2: PV1, PV2, PV3, VV1, VV2, MV1* On; Else Off
Progressively returning to a safe configuration.

### Purge
#### Valve Configuration 2: PV1, VV1, VV2 On; Else Off
In this state the system is dumping whatever residual fuels are still in the fuel lines.  A minimum amount of time (TBD) should be passed in this state.

### Post
#### Valve Configuration: VV1, VV2 On; Else Off
Post fire, at this stage it is safe to approach the test stand.  All valves in nominal position, no means to actuate solenoids.
