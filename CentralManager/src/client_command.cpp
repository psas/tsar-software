
// convert recieved string to json doc to command struct
int link_logger::
make_command_data(std::string ) const {

    if(output_string == NULL)
        return 0;

    using namespace rapidjson;

    Document document;
    document.Parse(output_string);

    // get string
    if(document.HasMember("string") && document["string"].IsString())
        strncpy(new_command.string, document["string"].GetString(), 
                strlen(document["string"].GetString()));
    else
        return -1;

    // get int
    if(document["number"].IsNumber())
        new_command.number = document["number"].GetInt();
    else
        return -2;

    return 1;
}


/****************************************************************************/
