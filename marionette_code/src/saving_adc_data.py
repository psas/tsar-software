import serial
import time
import csv
import marionette_lib
# Connect to the Device
m = marionette_lib.Marionette("/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if00")
ser = serial.Serial('/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if02')
ser.flushInput() # reset input buffer
m.adc.config(0,100) # set the sample rate at 1000 samples per second for channel 0
#m.adc.config(1,1000) # set the sample rate at 1000 samples per second for channel 1
m.adc.start(0) # start sampling channel 0
#m.adc.start(1) # start sampling channel 1
while True:
    try:
	# read a line from the serial port usb-APDM_Marionette_520022004115830393238353-if02
        ser_bytes = ser.readline()
	# open test_data.csv file
        with open("test_data.csv","a") as f:
	    # Write to data.csv file
            writer = csv.writer(f)
	    # write the serial line to data.csv
            writer.writerow([ser_bytes])
# exit the loop by typing "Ctrl C"
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
# Stop sampling data
	m.adc.stop(0)
#	m.adc.stop(1)
        break
