#include <iostream>

/*
 * This file contatins all sequencer related test main()
 */

#if defined(SYSTEM_TEST)

#include "../central_manager.h"

int
main() {
    std::cout << "Starting System Test" << std::endl;

    central_manager CM;
    CM.start_system();

    return 0;
}

#elif defined(SEQUENCER_TEST)

int
main() {
    std::cout << "Starting Sequnecer Test" << std::endl;

    sequencer seq;

    return 0;
}


#endif
