##################################################################################
# Marionette_Tester
# Prints All the data packets that are requested to the marionette
##################################################################################
import serial, sys, time, readline, re
import serial.tools.list_ports
ports = list(serial.tools.list_ports.comports())

def data_pack_to_AC(tty):
          # Removing the comma from the data packet and replacing it with a space.
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
                
          # IF the length of the data is bigger than 0 then me have data to parse.  
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
            #if 'COM11' in p.device: #for windows 
            if "Marionette - Control" in p.description:
                marionette = p.device
                tty = serial.Serial(port= marionette,timeout=3)
                
                return tty 
      except (OSError,serial.serialutil.SerialException) as e:
          print(f" Problem stablishing serial connection with Marionette: {e}.")

def main():
      # Open the PORT 
      tty = open_port_marionette()  
      # SEtup and Initialization of the Marionette  
      tty.write("\r\n".encode('utf-8'))
      tty.write("+noecho\r\n".encode('utf-8'))
      tty.write("+noprompt\r\n".encode('utf-8'))
      tty.write("\r\n".encode('utf-8'))
      
      # Set Marionette to 16 samples per second
      tty.write('adc.config(0,16)\r\n'.encode('utf-8'))
      tty.write('adc.config(1,16)\r\n'.encode('utf-8'))
      
      #  Infinite While loop in charge of saving the data packets into a FIFO QUEUE
      try:
          while True:
                data_pack = data_pack_to_AC(tty)
                sensor_values = str(data_pack)
                #Print all the data packets in one line 
                print(sensor_values, end = '\033[K\r')
      except KeyboardInterrupt:
            tty.close()
            sys.exit(0)
            
if __name__ == '__main__': 
    main()


