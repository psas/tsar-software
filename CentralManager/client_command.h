#ifndef _CLIENT_COMMAND_H_
#define _CLIENT_COMMAND_H_

#define STRING_LENGTH 128

struct client_command {
    char string[STRING_LENGTH];
    int number;
};

#endif
