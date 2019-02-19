#include "sensor_data_frame.h"

// this is overload to ignore the time field, since every object will have a different time value
bool operator == (const sensor_data_frame & A, const sensor_data_frame & B) {
    if(A.temp_1 != B.temp_1)
        return 0;
    if(A.pres_1 != B.pres_1)
        return 0;
    if(A.temp_2 != B.temp_2)
        return 0;
    if(A.pres_2 != B.pres_2)
        return 0;
    if(A.light_status != B.light_status)
        return 0;
    return 1;
}


// this is overload to ignore the time field, since every object will have a different time value
bool operator != (const sensor_data_frame & A, const sensor_data_frame & B) {
    if(A.temp_1 == B.temp_1)
        return 0;
    if(A.pres_1 == B.pres_1)
        return 0;
    if(A.temp_2 == B.temp_2)
        return 0;
    if(A.pres_2 == B.pres_2)
        return 0;
    if(A.light_status == B.light_status)
        return 0;
    return 1;
}


// Uses RapidJSON to converts sensor_data_frame struct to a vector. 
void sensor_data_frame::
make_JSON(std::string & output) {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    //writer.SetMaxDecimalPlaces(6);

    writer.StartObject();

    // time
    writer.Key("time");
    writer.String(time.c_str());

    // i2c 
    writer.Key("temp_1");
    writer.Double(temp_1);
    writer.Key("pres_1");   
    writer.Double(pres_1);
    writer.Key("temp_2");       
    writer.Double(temp_2);
    writer.Key("pres_2");       
    writer.Double(pres_2);

    // gpio
    writer.Key("light_status"); 
    writer.Bool(light_status);

    writer.EndObject();
    output = s.GetString();
    return;
}


/* Uses RapidJSON to converts sensor_data_frame struct to a vector. 
 * Only adds data that has changed to reduce data sent to clients
 */
void sensor_data_frame::
make_JSON_diff(std::string & output, const sensor_data_frame & other) {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    //writer.SetMaxDecimalPlaces(6);

    writer.StartObject();

    // time
    writer.Key("time");
    writer.String(time.c_str());

    // i2c 
    if(temp_1 != other.temp_1) {
        writer.Key("temp_1");
        writer.Double(temp_1);
    }
    if(pres_1 != other.pres_1) {
        writer.Key("pres_1");
        writer.Double(pres_1);
    }
    if(temp_2 != other.temp_2) {
        writer.Key("temp_2");       
        writer.Double(temp_2);
    }
    if(pres_2 != other.pres_2) {
        writer.Key("pres_2");       
        writer.Double(pres_2);
    }

    // gpio
    if(light_status != other.light_status){
        writer.Key("light_status"); 
        writer.Bool(light_status);
    }

    writer.EndObject();
    output = s.GetString();
    return;
}


