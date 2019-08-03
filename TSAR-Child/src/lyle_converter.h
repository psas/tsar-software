#ifndef TSAR_LYLE_CONVERTER
#define TSAR_LYLE_CONVERTER

#include "state.h"
#include "tsar_lowrate_daq.h"

class LyleConverter {
  private:
    lowrate_daq board;

    float binary_to_pressure(const int);
    float binary_to_temperature(const int);
  public:
    LyleConverter();

    const int load_state(const State&);
};

#endif
