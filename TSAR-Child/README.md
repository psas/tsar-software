# TSAR-Child
TSAR-Child is a two thread program design for a BeagleBone Black. The main thead is the central manager thread. It is the main working thread that handles the state machine, hardware control, and saving data locally on the BeagleBone. The other thread is the user interface thread. It uses ncruses to display data and take in the user input.

## File Structure
- **main-propellent-value.ino** - Arduino code to cotrol a the main values.
- **data** - Directory to hold output of TSAR-Child.
- **docs** - Directory for documentation.
- **makefile** - Makefile for tsar-child
- **src** - Directory for source code.
    - **central_manager.cpp/.h** - Main work thread. Includes state machine and hardware control.
    - **command_control.cpp/.h** - User interface (ncrueses) thread.
    - **GPIO** - GPIO library [repo](https://github.com/mkaczanowski/BeagleBoneBlack-GPIO)
    - **lyle_converter.cpp/.h** - ??
    - **main.cpp** - Main function.
    - **mcp234x.cpp/.h** - Driver for MCP232x.
    - **tsar-lowrate_daq.cpp/.h** - Driver for Lowrate Data Aquistion board.
    - **state.h** - Shared data between threads.
- **utils** - Directory for useful script for development.

## Use
- `make clean tsar-child`
- `./tsar-child`
