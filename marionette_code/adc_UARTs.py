import serial
import time
import marionette_lib
# Connect to the Device
m = marionette_lib.Marionette("/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if00")
ser = serial.Serial('/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if02')
ser.flushInput() # reset input buffer
m.adc.config(0,1000) # set the sample rate at 1000 samples per second for channel 0
m.adc.config(1,1000) # set the sample rate at 1000 samples per second for channel 1
m.adc.start(0) # start sampling channel 0
m.adc.start(1) # start sampling channel 1
# define ports
UART6 = serial.Serial("/dev/ttyO5", baudrate=115200, timeout=3.0) 
# set in an infinit loop 
while True:    
    try:
        # read a line from the serial port usb-APDM_Marionette_520022004115830393238353-if02
        ser_bytes = ser.readline()
        # send the data through UART6_rxd
        UART6.write(str.encode(ser_bytes))
        # receive the data from UART6_txd and output it to the user 
        print (UART6.readline()) 
        time.sleep(1)
# Exiting by typing Ctrl C
    except:
        print("Keyboard Interrupt")
        m.adc.stop(0) # start sampling channel 0
        m.adc.stop(1) # start sampling channel 1
        print("Exiting")
        break