#########################################################################################
# Capstone 2020 
# by Josh Pradera 
# Actuator Controller and Marionette handler script
#
# This script merges the Actuator Controller and Marionette using threads.
# - Data packets containing the sensor values and valve  position is parse in this script
# File name: multibb.py
#
##########################################################################################
import serial, sys, time, readline, re
import concurrent.futures # uses the tHread pool executor 
#from queue import Queue # To use with threading only
from multiprocessing import Queue, Manager
import serial.tools.list_ports
#import threading
import multiprocessing 
from ctypes import c_char
ports = list(serial.tools.list_ports.comports())
# Using Semaphores in multiprocessing to fix race condition 
lock = multiprocessing.Lock()
#lock = threading.Lock()
##################################################################################
# Marionette
# This section handles the communication with the marionette
##################################################################################
var = 0 # initialization variable for the number of averages per second

#Create a list to hold all the sensor values
multi_list =[]

def data_pack_to_AC(tty, q2):
          # Removing the comma from the data packet and replacing it with a space.
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
                #Set initial parameters to set echo off
                tty.write("\r\n".encode('utf-8'))
                tty.write("+noecho\r\n".encode('utf-8'))
                tty.write("+noprompt\r\n".encode('utf-8'))
                tty.write("\r\n".encode('utf-8'))
                # Configure the Marionette to 16 samples per second
                tty.write('adc.config(0,16)\r\n'.encode('utf-8'))
                tty.write('adc.config(1,16)\r\n'.encode('utf-8')) 
                #reset is done 
                
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

def init_marionette(q, q2, tty):  
      # SEtup and Initialization of the Marionette  
      #tty.write("\r\n".encode('utf-8'))
      tty.write("+noecho\r\n".encode('utf-8'))
      tty.write("+noprompt\r\n".encode('utf-8'))
      #tty.write("\r\n".encode('utf-8'))
      
      # Set Marionette to 16 samples per second
      tty.write('adc.config(0,16)\r\n'.encode('utf-8'))
      tty.write('adc.config(1,16)\r\n'.encode('utf-8'))
      
      #  Infinite While loop in charge of saving the data packets into a FIFO QUEUE
      while True:
            data_pack = data_pack_to_AC(tty, q2)
            sensor_values = str(data_pack)
            lock.acquire()
            # Add data packets to the FIFO QUEUE     
            q.put(sensor_values)
            #print(str(sensor_values)+'\n')
            #print("Add data packet to queue\n")
            lock.release()
            #print("Putting Marionette data\n")



############################################################################################
# AC controller section 
# Consumer Process
############################################################################################        
def opening_port(q2):
    global serial_port
    try:
            ac =""
            for p in ports:
                #if 'COM9' in p.device: #for windows
                #if "066BFF343633464257245637" in p.serial_number: #my nucleo 
                if '0662FF544856846687095229' in p.serial_number: # PSAS nucleo 
                    ac = p.device
                    # Port initialization 
                    serial_port = serial.Serial( 
                    port= ac,
                    baudrate=9600, 
                    bytesize= serial.EIGHTBITS, 
                    parity= serial.PARITY_NONE, 
                    stopbits= serial.STOPBITS_ONE, 
                    timeout= .5)#, 
                    #write_timeout = 1)#,
                    #timeout  is for pyserial .read()
                    #write_timeout=0)
                    
            # Discarding anything in the I/O buffer.        
            serial_port.reset_output_buffer()
            serial_port.reset_input_buffer()     

            return serial_port

    except (OSError,serial.serialutil.SerialException) as e:
        print(f" Problem stablishing serial connection with Actuator Controller: {e})")
        

def greeting():          
    print('\n************************************************************************\r')
    print('               PORTLAND STATE AEROSPACE SOCIETY              \r')
    print('************************************************************************\n')
    global sig_to_print # variable to wait until the greeting before automatically sending the first status command.
    sig_to_print = True
    
# The help menu in this script shows all the valid transitions as well as all the valid user commands.
def help_menu():
    
    commands=[[' GENERAL COMMANDS  ',' VALVE CHECK COMMANDS ', ' Valves          '],
          [' status: Current state. ',' cyclevalves: Automatic valve check.',' PV-F01: Fuel Pressure'],
          [' loxprefill',' cyclevoff: Automatic sequence OFF.',' PV-L01: Lox Pressure '],
          [' loxfill', 'Turn ON valve: sov1on ', ' PV-P01: Fuel Purge N2'],[' loxchill','Turn OFF Valve: sov1off',' SOV-F01: Fuel Vent   '],
          [' safety','',' PV-F02: Fuel Main    '],[' pressurize',' ',' PV-L03: Lox Main     '],[' ignition',' ',' PV-LF01: Lox Fill    '],
          [' off: Terminate AC software.',' ',' PV-L02: Lox Vent     '], [' valvecheck',' ',' PV-P02: Lox Purge    '], [' exit: python script. ',' ',' IG:   Igniter        ']]
            
            
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
    
    
# This function writes all the commands to the AC
def write_comand(comand):
    try:
        #Reset the buffer before sending a command 
        serial_port.reset_output_buffer()
        #serial_port.reset_input_buffer()
        #time.sleep(0.002)
        #serial_port.write(bytes(comand,'utf8'))
        
        if serial_port.inWaiting() == 0:
        	for index in comand:
        		serial_port.write(bytes(index, 'UTF-8'))
        		time.sleep(0.0008)
        		
        #serial_port.write(comand.encode())
        # Testing wheter the Queue is getting the user commands and if we are actually sending them!
        '''
        if '3F99' in str(comand):
            pass
        else : 
            print(f" cmd sent {comand}\n")
        '''
        '''
        while stop -start < 0.003:
            stop = time.time()
            pass
            '''
        # After testing time.sleep() induces a wait time after writing, needed otherwise 
        # commands may be missed sometimes.        
        #time.sleep(0.005)

    except (OSError,serial.serialutil.SerialException) as e:
            print (f" There was a problem communicating with AC: {e}\n")
            print('Resetting Actuator Controller...\n')
            write_comand('B6D8\r')# reset command



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
                
                if not data:
                    pass
                    
                else:
                    
                    if data == '\n':
                        data = ''.join(all_bytes)
                        #Print to screen everything the AC sends
                        print(data, end = '\033[K\n\n', flush=True)
                        all_bytes.clear()
                        #serial_port.reset_input_buffer()

                    else:
                        #append all the bytes that are being received
                        all_bytes.append(str(data))
                        
            except (OSError,serial.serialutil.SerialException) as e:
                print(f'Actuator Controller: {e}')

           
# This function process the user commands and send the AC the HEX commands is expecting as a command.
def comand_check(argument, q2, q):
         
        if argument == 'loxprefill':
            write_comand('7A12\r')
            
        elif argument == 'valvecheck':
                #print("Sending user cmd to AC\n")
                write_comand('4DF2\r')
   
        elif argument == 'cyclevalves':
            #print("Sending user cmd to AC\n")
            write_comand('E23A\r')


        elif argument == 'shutdown':
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
            #print("Sending user cmd to AC\n")
            #print(f"Queue size by the time we send comd:{q.qsize}")
            write_comand('E5F2\r')
            
            
            #writer()

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

        # At this point no valid command was entered
        
        else:
            if 'None' in argument:
                pass
            else:
                print("Please enter a valid command...\n")
        '''                
        # handle marionette data packet  
        elif argument[:4] == '3F99':
                write_comand(argument+'\r')
                #print(argument[4:], end='\r', flush= True)
                new_argument = argument.strip('3F99 ')
                averagin_sensor_values(new_argument)
                '''
                
# This function handles the average of the sensors coming from the data packet
# The average is set to be 16 samples per second            
def averagin_sensor_values(new_argument):
                
                #get the string values and put them in a list
                a_list = new_argument.split()
                mapping = map(int, a_list)
                values = list(mapping)
                global multi_list
                multi_list.append(values) 
                global var                     
                var = var +  1

                #was set to 16 before
                if var == 11:
                    # calculate the averages
                    avg = list(map(lambda x: str(round((sum(x)/len(x)),1)), zip(*multi_list)))
                    # need to convert values into a string 
                    str_avg = ' '.join(avg)
                    str_avg2 = 'Sensor values: '+ str(str_avg)
                    #\033[K clean the line and \033[1A moves cursor up one line
                    print(str_avg2, end = '\033[K\r\033[1A', flush = True)
                    #print(str_avg2, end = '\033[K\r')
                    var = 0
                    
# This function handles the commands and data packets that are saved in a FIFO QUEUE
def init_ac(q, q2):             
        greeting()
        while True:
            # Get the data from marionette and user input
            # from queue and process it
            if q.empty():
                pass
            else:
                da = q.get()
                # handle marionette data packet  
                if da[:4] == '3F99':
                    write_comand(da+'\r')
                    #print(argument[4:], end='\r', flush= True)
                    new_argument = da.strip('3F99 ')
                    averagin_sensor_values(new_argument)
                else:
                    comand_check(da, q2, q)

def main():
      # example of how set a global variable that different processes can share
      #running_flag = multiprocessing.Value("i", 1)
      
      q = Queue(maxsize = 5)   #This FIFO Queue handles the processing of data packets and commands. 
      q2 = Queue() # To pass a signal from AC to Marionette to reset the marionette. 
      q3 = Queue() 
      #opening AC port
      serial_port = opening_port(q2)
      #open marionette port 
      tty = open_port_marionette()

      # Process that handles the Actuator controller
      ac = multiprocessing.Process(target=init_ac, args=(q,q2))
      #ac = threading.Thread(target=init_ac, args=(q,q2))
      #will terminate with the main process
      ac.daemon = True
      ac.start()
      
      # Process that handles the Marionette
      m = multiprocessing.Process(target=init_marionette, args=(q,q2,tty))
      #m = threading.Thread(target=init_marionette, args=(q,q2,tty))
      #will terminate with the main process
      m.daemon = True
      m.start()
      
      # Process writer
      # This process writes to screen everything the AC outputs.
      writer = multiprocessing.Process(target=writeall, args=(serial_port,q2))
      #writer = threading.Thread(target=writeall, args=(serial_port,q2))
      #Deamon = True automatically finishes the process as soon as main ends.
      writer.daemon = True
      writer.start()

      
      
      # This while loop gets the user commands and saves them in the FIFO QUEUE designed
      # for data packets and user commands.
      while True:
            user_in = input("")
            user_input = user_in.lower()
            if 'exit' in user_input:
                q.close() #works with multiprocessing
                q2.close()
                serial_port.close()
                tty.close() 

                break
            else:
                lock.acquire()
                # Add data packet/user command to the FIFO QUEUE
                q.put(user_input)
                #print("****** Add user command to queue *******\n")
                lock.release()
                
                #print("Putting User command \n")
                #print(f"Size of Queue user cmd: {q.qsize}")

             
if __name__ == '__main__':
    
    main()# call main function
    

