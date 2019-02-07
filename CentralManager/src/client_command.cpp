#include "client_command.h"

int client_command::
make_command_data(std::string & input) {
    using namespace rapidjson;

    rapidjson::Document document;
    document.Parse(input.c_str());

    // get int
    if(document["number"].IsNumber())
        number = document["number"].GetInt();

    return 1;
}


