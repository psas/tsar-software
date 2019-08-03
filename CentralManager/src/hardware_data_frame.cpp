#include "hardware_data_frame.h"

// this is overload to ignore the time field, since every object will have a different time value
bool operator == (const hardware_data_frame & A, const hardware_data_frame & B) {
    if(memcmp(&A.i2c_data, &B.i2c_data, sizeof(i2c_data_frame)) != 0)
        return 0;
    if(memcmp(&A.gpio_data, &B.gpio_data, sizeof(gpio_data_frame)) != 0)
        return 0;
    if(A.AC_connected != B.AC_connected)
        return 0;
    if(memcmp(&A.AC_data, &B.AC_data, sizeof(AC_data_frame)) != 0)
        return 0;
    return 1;
}


// this is overload to ignore the time field, since every object will have a different time value
bool operator != (const hardware_data_frame & A, const hardware_data_frame & B) {
    if(memcmp(&A.i2c_data, &B.i2c_data, sizeof(i2c_data_frame)) == 0)
        return 0;
    if(memcmp(&A.gpio_data, &B.gpio_data, sizeof(gpio_data_frame)) == 0)
        return 0;
    if(A.AC_connected == B.AC_connected)
        return 0;
    if(memcmp(&A.AC_data, &B.AC_data, sizeof(AC_data_frame)) == 0)
        return 0;
    return 1;
}


// Uses RapidJSON to converts hardware_data_frame struct to a vector. 
void hardware_data_frame::
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


/* Uses RapidJSON to converts hardware_data_frame struct to a vector. 
 * Only adds data that has changed to reduce data sent to clients
 */
void hardware_data_frame::
make_JSON_diff(std::string & output, const hardware_data_frame & other) {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    //writer.SetMaxDecimalPlaces(6);

    writer.StartObject();

    writer.Key("what");
    writer.String("hdw_change");

    // time
    writer.Key("time");
    writer.String(time.c_str());

    // i2c 
    if(i2c_data.sensor_1_connected != other.i2c_data.sensor_1_connected) {
        writer.Key("sensor_1_connected");
        writer.Bool(i2c_data.sensor_1_connected);
    }
    if(i2c_data.temp_1 != other.i2c_data.temp_1) {
        writer.Key("temp_1");
        writer.Double(i2c_data.temp_1);
    }
    if(i2c_data.pres_1 != other.i2c_data.pres_1) {
        writer.Key("pres_1");
        writer.Double(i2c_data.pres_1);
    }
    if(i2c_data.sensor_2_connected != other.i2c_data.sensor_2_connected) {
        writer.Key("sensor_2_connected");
        writer.Bool(i2c_data.sensor_2_connected);
    }
    if(i2c_data.temp_2 != other.i2c_data.temp_2) {
        writer.Key("temp_2");       
        writer.Double(i2c_data.temp_2);
    }
    if(i2c_data.pres_2 != other.i2c_data.pres_2) {
        writer.Key("pres_2");       
        writer.Double(i2c_data.pres_2);
    }

    // gpio
    if(gpio_data.light_1_status != other.gpio_data.light_1_status){
        writer.Key("light_1_status"); 
        writer.Bool(gpio_data.light_1_status);
    }
    if(gpio_data.light_2_status != other.gpio_data.light_2_status){
        writer.Key("light_2_status"); 
        writer.Bool(gpio_data.light_2_status);
    }

    // uart
    if(AC_connected != other.AC_connected) {
        writer.Key("AC_connected"); 
        writer.Bool(AC_connected);
    }
    if(AC_data.next_failure_mode != other.AC_data.next_failure_mode) {
        writer.Key("AC_next_failure_mode"); 
        writer.Int((int)AC_data.next_failure_mode);
    }
    if(AC_data.failure_mode != other.AC_data.failure_mode) {
        writer.Key("AC_failure_mode"); 
        writer.Int((int)AC_data.failure_mode);
    }
    if(AC_data.failure_cause != other.AC_data.failure_cause) {
        writer.Key("AC_failure_cause"); 
        writer.Int((int)AC_data.failure_cause);
    }
    if(AC_data.consecutive_checksum_errors != other.AC_data.consecutive_checksum_errors) {
        writer.Key("AC_consecutive_checksum_errors"); 
        writer.Int((int)AC_data.consecutive_checksum_errors);
    }
    if(AC_data.lifetime_checksum_errors != other.AC_data.lifetime_checksum_errors) {
        writer.Key("AC_lifetime_checksum_errors"); 
        writer.Int((int)AC_data.lifetime_checksum_errors);
    }
    if(AC_data.sensor1 != other.AC_data.sensor1) {
        writer.Key("AC_sensor1"); 
        writer.Int((int)AC_data.sensor1);
    }
    if(AC_data.sensor2 != other.AC_data.sensor2) {
        writer.Key("AC_sensor2"); 
        writer.Int((int)AC_data.sensor2);
    }
    if(AC_data.sensor3 != other.AC_data.sensor3) {
        writer.Key("AC_sensor3"); 
        writer.Int((int)AC_data.sensor3);
    }
    if(AC_data.sensor4 != other.AC_data.sensor4) {
        writer.Key("AC_sensor4"); 
        writer.Int((int)AC_data.sensor4);
    }
    if(AC_data.sensor5 != other.AC_data.sensor5) {
        writer.Key("AC_sensor5"); 
        writer.Int((int)AC_data.sensor5);
    }

    writer.EndObject();
    output = s.GetString();
    return;
}


