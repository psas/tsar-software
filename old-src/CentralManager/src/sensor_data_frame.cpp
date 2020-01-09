#include "sensor_data_frame.h"


// Uses RapidJSON to converts hardware_data_frame struct to a vector. 
void sensor_data_frame::
make_JSON(std::string & output) {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    //writer.SetMaxDecimalPlaces(6);

    writer.StartObject();

    writer.Key("what");
    writer.String("hdw_full");

    // time
    writer.Key("time");
    writer.String(time.c_str());

    // i2c 
    writer.Key("sensor_1_connected");
    writer.Bool(i2c_data.sensor_1_connected);
    writer.Key("temp_1");
    writer.Double(i2c_data.temp_1);
    writer.Key("pres_1");   
    writer.Double(i2c_data.pres_1);
    writer.Key("sensor_2_connected");
    writer.Bool(i2c_data.sensor_2_connected);
    writer.Key("temp_2");       
    writer.Double(i2c_data.temp_2);
    writer.Key("pres_2");       
    writer.Double(i2c_data.pres_2);

    // gpio
    writer.Key("light_1_status"); 
    writer.Bool(gpio_data.light_1_status);
    writer.Key("light_2_status"); 
    writer.Bool(gpio_data.light_2_status);

    // uart
    writer.Key("AC_connected"); 
    writer.Bool(AC_connected);
    writer.Key("AC_next_failure_mode"); 
    writer.Int((int)AC_data.next_failure_mode);
    writer.Key("AC_failure_mode"); 
    writer.Int((int)AC_data.failure_mode);
    writer.Key("AC_failure_cause"); 
    writer.Int((int)AC_data.failure_cause);
    writer.Key("AC_consecutive_checksum_errors"); 
    writer.Int((int)AC_data.consecutive_checksum_errors);
    writer.Key("AC_lifetime_checksum_errors"); 
    writer.Int((int)AC_data.lifetime_checksum_errors);
    writer.Key("AC_sensor1"); 
    writer.Int((int)AC_data.sensor1);
    writer.Key("AC_sensor2"); 
    writer.Int((int)AC_data.sensor2);
    writer.Key("AC_sensor3"); 
    writer.Int((int)AC_data.sensor3);
    writer.Key("AC_sensor4"); 
    writer.Int((int)AC_data.sensor4);
    writer.Key("AC_sensor5"); 
    writer.Int((int)AC_data.sensor5);

    writer.EndObject();
    output = s.GetString();
    return;
}
