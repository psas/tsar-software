#########################################################################################
# Capstone 2020 
# by Josh Pradera 
# Marionette samples per second tester
#
# This script was developed to confirm samples per second for the tars.pack\r\n command
##########################################################################################
import serial,time
import serial.tools.list_ports
ports = list(serial.tools.list_ports.comports())

def data_pack_to_AC(tty):
          char_remove = ";"
          try:

                tty.write('tsar.pack\r\n'.encode('utf-8'))

          except (OSError,serial.serialutil.SerialException) as e:
            print(f" There was a problem sending command to Marionette: {e}")
          # read lines until we get an 'end' marker or we timeout
          try:
            data_pack = tty.readline().decode('utf-8').rstrip('\r\n')
          except (OSError,serial.serialutil.SerialException) as e:
                    print(f" There was a problem reading from Marionette: {e}")
                
            
          while len(data_pack) > 0:
            if data_pack.lower().startswith("u16:"):
              for character in char_remove:
                  data_pack = data_pack.replace(character, ' ')
                  
              packtoAC = data_pack[4:]
              tty.flushInput() 
              return packtoAC
            elif data_pack.lower().startswith("end:"):
              break
            else:
                try:
                    data_pack = tty.readline().decode().rstrip('\r\n')
                except (OSError,serial.serialutil.SerialException) as e:
                    print(f" There was a problem reading from Marionette: {e}")    
      
def open_port_marionette():
      try:
          marionette = ""
               
          for p in ports:
            if 'COM11' in p.device: #for windows 
            #if "Marionette - Control" in p.description:
                marionette = p.device
                tty = serial.Serial(port= marionette,timeout=3)
                
                return tty 
      except (OSError,serial.serialutil.SerialException) as e:
          print(f" Problem stablishing serial connection with Marionette: {e}.")

def init_marionette(tty):    
      tty.write("\r\n".encode('utf-8'))
      tty.write("+noecho\r\n".encode('utf-8'))
      tty.write("+noprompt\r\n".encode('utf-8'))
      tty.write("\r\n".encode('utf-8'))

      tty.write('adc.config(0,16)\r\n'.encode('utf-8'))
      tty.write('adc.config(1,16)\r\n'.encode('utf-8'))

tty = open_port_marionette()
count = 0      
while True:
    start =  time.time()
    tracker0 = time.time()
    # Set to count all the samples that are printed in a second
    d = tracker0-start
    while tracker0-start <=1.0:
        packtoAC = data_pack_to_AC(tty)
        print(packtoAC, end= '\r', flush=True)
        count +=1
        tracker0 = time.time()
    
    print(f"Samples in a second: {count}\033[K\n")  
    count = 0 
