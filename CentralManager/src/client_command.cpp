#include "client_command.h"


int client_command::
make_command_data(std::string & input) {
    using namespace rapidjson;

    rapidjson::Document document;
    document.Parse(input.c_str());

    // get int
    if(document["seq_command"].IsString())
        seq_command = document["seq_command"].GetString();

    return 1;
}


