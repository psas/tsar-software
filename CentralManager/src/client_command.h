#ifndef _CLIENT_COMMAND_H_
#define _CLIENT_COMMAND_H_

#include <string>

#include "../thirdparty/rapidjson/document.h"       // rapidjson


/* This a POD (Plain Old Data) for commands given by the client.
 * It tehcically is not a POD, but the function only converts a 
 * JSON string into the internal dat, so for the most part it is.
 */
struct client_command {
    int make_command_data(std::string & output);

    std::string seq_command;
    // TODO: add more options
};

#endif
