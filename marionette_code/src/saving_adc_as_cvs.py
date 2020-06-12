import time
import csv
import marionette_lib
# Connect to the Device
m = marionette_lib.Marionette("/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if00")
# endless loop  
while True:
    try:
	# Open data.csv file
        with open("data.csv","a") as f:
	    # Write to data.csv file
            writer = csv.writer(f)
	    # Convert the analog read of the two channels 0 and 1 to digital and save the digital values .csv file
            writer.writerow(['single 0 :',m.adc.single(0), 	'single 1 :',m.adc.single(1)],)
	time.sleep(.001)		
# "Ctrl C" to Interrupt and exit the loop
    except:
        print("Keyboard Interrupt")
        break

