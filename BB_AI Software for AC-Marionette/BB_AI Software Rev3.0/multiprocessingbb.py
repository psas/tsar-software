#########################################################################################
# Capstone 2020 
# by Josh Pradera 
# Actuator Controller and Marionette handler script
#
# This script merges the Actuator Controller and Marionette using processes.
# - Data packets containing the sensor values and valve  position are parse in this script
##########################################################################################
import serial, sys, time, readline, re
import concurrent.futures # uses the tHread pool executor 
#from queue import Queue 
from multiprocessing import Queue, Manager
import serial.tools.list_ports
import threading
import multiprocessing 
from ctypes import c_char
ports = list(serial.tools.list_ports.comports())

#############################################################
## Marionette
## This section handles the communication with the marionette
##############################################################
var = 0

#Create a multidimentional list to hold all the sensor values
multi_list =[]

def data_pack_to_AC(tty, q2):
          char_remove = ";"
          signal_to_reset = False
          try:
            # Passing the reset marionette signal between two processes via a queue
            if q2.empty():
                pass
            elif not q2.empty():
                re = q2.get()
                
                if 'RESET_MARIONETTE' in str(re):
                    signal_to_reset = True
                    #if sifnal_to_reset:
                    #print('Signal to reset successfull\n')


            if not signal_to_reset:
            #if not resetting:
                tty.write('tsar.pack\r\n'.encode('utf-8'))
            elif signal_to_reset:
            #elif 'RESET_MARIONETTE' in str(ting):
            #elif resetting:
                print('OK resetting Marionette!') 
                tty.write('reset\r\n'.encode('utf-8'))
                
                tty.write("\r\n".encode('utf-8'))
                tty.write("+noecho\r\n".encode('utf-8'))
                tty.write("+noprompt\r\n".encode('utf-8'))
                tty.write("\r\n".encode('utf-8'))

                tty.write('adc.config(0,16)\r\n'.encode('utf-8'))
                tty.write('adc.config(1,16)\r\n'.encode('utf-8')) 
                #reset is done 
                #resetting = False
                
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
            #if 'COM11' in p.device: #for windows 
            if "Marionette - Control" in p.description:
                marionette = p.device
                tty = serial.Serial(port= marionette,timeout=3)
                
                return tty 
      except (OSError,serial.serialutil.SerialException) as e:
          print(f" Problem stablishing serial connection with Marionette: {e}.")

def init_marionette(q, q2, tty):    
      tty.write("\r\n".encode('utf-8'))
      tty.write("+noecho\r\n".encode('utf-8'))
      tty.write("+noprompt\r\n".encode('utf-8'))
      tty.write("\r\n".encode('utf-8'))

      tty.write('adc.config(0,16)\r\n'.encode('utf-8'))
      tty.write('adc.config(1,16)\r\n'.encode('utf-8'))

      
      while True:
            data_pack = data_pack_to_AC(tty, q2)
            sensor_values = str(data_pack)
            q.put(sensor_values)
            
            #for testing 
            #print(sensor_values)
            #print('Got here!')

######################################################
# AC controller section 
# Consumer Process
###############################################        

def opening_port(q2):
    global serial_port
    try:
        ac =""
        for p in ports:
            #if 'COM9' in p.device: #for windows
            ##if "066BFF343633464257245637" in p.serial_number: #PSAS nucleo 
            if "066BFF343633464257245637" in p.serial_number: #my nucleo 
                ac = p.device
                # Port initialization 
                serial_port = serial.Serial( 
                port= ac,
                baudrate=9600, 
                bytesize= serial.EIGHTBITS, 
                parity= serial.PARITY_NONE, 
                stopbits= serial.STOPBITS_ONE, 
                timeout= 0.5)
        serial_port.flushInput()
        serial_port.flushOutput()

        # tread writer
        writer = multiprocessing.Process(target=writeall, args=(serial_port,q2))
        writer.daemon = True
        writer.start()

        return serial_port

    except (OSError,serial.serialutil.SerialException) as e:
        print(f" Problem stablishing serial connection with Actuator Controller: {e})")
###############################################################################################
# Writer Process that writes anything in stdout
# Daemon status (ends when main finish)
################################################################################################
def writeall(serial_port, q2):
    all_bytes = []
    while True:        
            try:
                bytesToRead = serial_port.inWaiting()
                data = serial_port.read(bytesToRead).decode('utf-8')
                num = int(bytesToRead)
                
                if not data:
                    pass
                else:
                    if data == '\n':
                        data = ''.join(all_bytes)
                        #print(data, flush = True)

                        #for testing
                        print(data, end = '\033[K\n\n', flush=True)
                        all_bytes.clear()
                        #serial_port.flush()
                    else:
                        #append all the bytes that are being received
                        all_bytes.append(str(data))
                

            except (OSError,serial.serialutil.SerialException) as e:
                print(f'Actuator Controller: {e}')

def greeting():          
    print('\n************************************************************************\r')
    print('               PORTLAND STATE AEROSPACE SOCIETY              \r')
    print('************************************************************************\n')
    global sig_to_print
    sig_to_print = True
    
def help_menu():
    
    commands=[[' GENERAL COMMANDS  ',' VALVE CHECK COMMANDS ', ' Valves '],
            [' status: Current state. ',' cyclevalves: Automatic valve check.',' SOV1: Fuel Pressure'],
            [' loxprefill',' cyclevoff: Automatic sequence OFF.',' SOV2: Lox Pressure '],
            [' loxfill', 'Turn ON valve: sov1on ', ' SOV3: Fuel Purge N2'],[' loxchill','Turn OFF Valve: sov1off',' SOV4: Fuel Vent'],
            [' safety','',' SOV5: Fuel Main '],[' pressurize',' ',' SOV6: Lox Main '],[' ignition',' ',' SOV7: Lox Fill'],
            [' off: Terminate AC software.',' ',' SOV8: Lox Vent'], [' valvecheck',' ',' SOV9: Lox Purge'], [' exit: ends the python scri ',' ',' IG:   Igniter']]
            
            
    dash = '-' * 93 
    for i in range(len(commands)):
        if i == 0:
            print(dash+ "\r")
            print('|{:^28s}|{:^40s}|{:^21}|'.format(commands[i][0],commands[i][1],commands[i][2])+ "\r")
            print(dash+ "\r")
        else:
            print('|{:<28s}|{:^40s}|{:<21}|'.format(commands[i][0],commands[i][1], commands[i][2])+ "\r")
        
    print(dash+ "\r")     


    valid_transitions=[[' VALID TRANSITIONS BETWEEN STATES '],['Start Handler -> Valve Check -> OFF'],
                        ['Start Handler -> Lox Pre Fill -> Lox Fill -> Lox Chill -> Safety -> Pressurize -> Ignition'],
                        ['(Auto Start):Ignition (5s) -> Oxidizer Start (.2s) -> Firing(3s) -> Purge(3s) -> Safety (Auto End)']]
                        
    dash = '-' * 100 
    for i in range(len(valid_transitions)):
        if i == 0:
            print(dash+ "\r")
            print('|{:^98s}|'.format(valid_transitions[i][0])+ "\r")
            print(dash+ "\r")
        else:
            print('|{:<98s}|'.format(valid_transitions[i][0])+ "\r")
        
    print(dash+ "\r") 

# Reading serial for cases where using readline() is not possible.
def write_comand(comand):
    # serial Write 
    try:
        we = len(comand)
        serial_port.write(bytes(comand,'utf8'))
        serial_port.flushInput() 

    except (OSError,serial.serialutil.SerialException) as e:
            print (f" There was a problem communicating with AC: {e}\n")
            print('Resetting Actuator Controller...\n')
            write_comand('B6D8\r')# reset command

def comand_check(argument, q2):

        if argument == 'loxprefill':
            write_comand('7A12\r')

        elif argument == 'valvecheck':
                write_comand('4DF2\r')
                #print(data)
   
        elif argument == 'cyclevalves':
            write_comand('E23A\r')

        elif argument == 'stop':
            write_comand('2B4E\r')

        elif argument == 'loxfill':
            write_comand('5E1A\r')

        elif argument == 'resetac':
            write_comand('B6D8\r')

        elif argument == 'resetm':
            #activating the flag so that marionette can be reset
            q2.put('RESET_MARIONETTE')
            #print('Resseting command.........................................................')

        elif argument == 'loxchill':
            write_comand('6F8B\r')

        elif argument == 'off':
            write_comand('1AB3\r')

        elif argument == 'safety':
            write_comand('9C45\r')

        elif argument == 'pressurize':
            write_comand('8B23\r')

        elif argument == 'starthandler':
            write_comand('CD89\r')

        elif argument == 'ignition':
            write_comand('AB67\r')

        elif argument == 'status':
            write_comand('E5F2\r')

        elif argument == 'status':
            write_comand('E5F2\r')

        elif argument == 'pvf01on':
            write_comand('5F4E\r')

        elif argument == 'pvf01off':
            write_comand('8D2A\r')

        elif argument == 'pvl01on':
            write_comand('3DEA\r')

        elif argument == 'pvl01off':
            write_comand('6B88\r')

        elif argument == 'pvp01on':
            write_comand('F1AB\r')

        elif argument == 'pvp01off':
            write_comand('C23D\r')

        elif argument == 'sovf01on':
            write_comand('B3ED\r')

        elif argument == 'sovf01off':
            write_comand('8E4A\r')

        elif argument == 'pvf02on':
            write_comand('2AE7\r')

        elif argument == 'pvf02off':
            write_comand('4A6C\r')

        elif argument == 'pvl03on':
            write_comand('67A8\r')

        elif argument == 'pvl03off':
            write_comand('3EE2\r')

        elif argument == 'pvlf01on':
            write_comand('55A6\r')

        elif argument == 'pvlf01off':
            write_comand('C5B7\r')

        elif argument == 'pvl02on':
            write_comand('B3F8\r')

        elif argument == 'pvl02off':
            write_comand('88FF\r')

        elif argument == 'pvp02on':
            write_comand('B9AD\r')

        elif argument == 'pvp02off':
            write_comand('DD3F\r')

        elif argument == 'igon':
            write_comand('4947\r')

        elif argument == 'igoff':
            write_comand('34A5\r')

        elif argument == 'cyclevalves':
                write_comand('E23A\r')
                
        elif argument == 'help':
                help_menu()
                        
        # handle invalid command and marionette data packet  
        elif argument[:4] == '3F99':
                write_comand(argument+'\r')
                #print(argument[4:], end='\r', flush= True)
                new_argument = argument.strip('3F99 ')
                averagin_sensor_values(new_argument)
        # At this point no valid command was entered
        
        else:
            if 'None' in argument:
                pass
            else:
                print("Please enter a valid command...\n")
                    
def averagin_sensor_values(new_argument):
                
                #get the string values and put them in a list
                a_list = new_argument.split()
                mapping = map(int, a_list)
                values = list(mapping)
                global multi_list
                multi_list.append(values) 
                global var                     
                var = var +  1
                #print(var)
                
                if var == 16:
                    # calculate the averages
                    avg = list(map(lambda x: str(round((sum(x)/len(x)),1)), zip(*multi_list)))
                    # need to convert values into a string 
                    str_avg = ' '.join(avg)
                    str_avg2 = 'Sensor values: '+ str(str_avg)
                    print(str_avg2, end = '\033[F\r', flush = True)
                    var = 0

def init_ac(q, q2):             
        greeting()
        while True:
            # Get the data from marionette and user input
            # from queue and process it
            if q.empty():
                pass
            else:
                da = q.get()
                comand_check(da, q2)

def main():
      running_flag = multiprocessing.Value("i", 1)
      q = Queue()   #This FIFO Queue handles the processing of data packets and commands 
      q2  = Queue() # To pass a signal from AC to Marionette 
      
      #opening AC port
      serial_port = opening_port(q2)
      #open marionette port 
      tty = open_port_marionette()

      # Function that handles the Actuator controller
      ac = multiprocessing.Process(target=init_ac, args=(q,q2))
      ac.daemon = True
      ac.start()
      
      #Function that handles the Marionette
      m = multiprocessing.Process(target=init_marionette, args=(q,q2,tty))
      #will terminate with the main process
      m.daemon = True
      m.start()
      
      #userIn(q)
      while True:
            user_in = input("")
            user_input = user_in.lower()
            if 'exit' in user_input:
                serial_port.close()
                tty.close()
                q.close()
                q2.close()
                break
            else:
                q.put(user_input)
            
            
                    
        
if __name__ == '__main__':
    
    main()# call main function
    

