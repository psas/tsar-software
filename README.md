# TSAR Manager

A manager for the test stand.

### Usage:

`$` `./tsar-man [OPTIONS]`
* `-h/--help`	Stop it, get some help
* `-u/--utility-mode`	Enables utility mode allowing for individual vent toggling instead of running the regular test stand sequence

##### In-Program commands:

* `>` `help:` Gets a list of useable commands
* `>` `status:` Gets the status of the state-machine and all of its current properties

###### Regular Runtime:

* `> ` `prefill:` **[Default State]** Attempts to go to state `LOX_PREFILL`
* `> ` `fill:` Attempts to go to state `LOX_FILL`
* `> ` `standby:` Attempts to go to state `SS0`
* `> ` `pressurize:` Attempts to go to state `PRESSURIZE`
* `> ` `ignite:` Attempts to go to state `IGNITE`
* `> ` `oxidize:` Attempts to go to state `O_START`
* `> ` `fuel:` Attempts to go to state `F_START`
* `> ` `fire:` Attempts to go to state `FIRE`
* `> ` `purge:` Attempts to go to state `PURGE`
* `> ` `blowdown:` Attempts to go to state `LOX_BLOWDOWN`

###### Utility-Mode Runtime:

All of the valve-short-names _(SOV1, SOV2, etc.)_, are commands for toggling the respective valve.

  **Ex:**
  * `> ` `sov1`

  Will toggle the Fuel Pressure Valve

  _(Note: valve-short-names as commands are not case sensitive.)_

### Valves / Igniters:
* `SOV1:` Fuel Pressure
* `SOV2:` LOX Pressure
* `SOV3:` Purge N2
* `SOV4:` Solenoid Vent Valve 1
* `SOV5:` Fuel Main
* `SOV6:` LOX Main
* `SOV7:` LOX Fill
* `SOV8:` Solenoid Vent Valve 2
* `IG1:` Igniter 1
* `IG2:` Igniter 2
