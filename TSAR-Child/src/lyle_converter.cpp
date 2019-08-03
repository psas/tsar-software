#include "lyle_converter.h"

//
// LyleConverter implementation
//
LyleConverter::LyleConverter() : board("/dev/i2c-1", 0x68, 0x69, 0x6a, 0x6b){}

LyleConverter::load_state(const State& state){
  int* data;
  board.read_adcs(data);

  state.state_mutex.lock(); // Lock the mutex

  state.pressure_1 = binary_to_pressure(data[0]);
  state.pressure_2 = binary_to_pressure(data[4]);
  state.pressure_3 = binary_to_pressure(data[8]);
  state.pressure_4 = binary_to_pressure(data[12]);

  state.temp_1 = binary_to_temperature(data[1]);
  state.temp_2 = binary_to_temperature(data[5]);
  state.temp_3 = binary_to_temperature(data[9]);
  state.temp_4 = binary_to_temperature(data[13]);

  state.state_mutex.unlock(); // Release the mutex

  delete [] data;
}
