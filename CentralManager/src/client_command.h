#ifndef _CLIENT_COMMAND_H_
#define _CLIENT_COMMAND_H_

#include <cstdint>
#define STRING_LENGTH 128

struct client_command {
    char string[STRING_LENGTH];
    uint32_t number;
};

#endif
