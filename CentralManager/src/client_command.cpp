#include "client_command.h"

int client_command::
make_command_data(std::string & input) {

    using namespace rapidjson;

    Document document;
    document.Parse(input);

    // get int
    if(document["number"].IsNumber())
        number = document["number"].GetInt();

    return 1;
}


