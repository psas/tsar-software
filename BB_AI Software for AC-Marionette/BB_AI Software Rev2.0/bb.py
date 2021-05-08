

import serial, sys, time, readline, re
import concurrent.futures # uses the tHread pool executor 
from queue import Queue 
import serial.tools.list_ports
ports = list(serial.tools.list_ports.comports())
#stdscr = curses.initscr()
#####curses.noecho()
#stdscr.nodelay(1) # set getch() non-blocking
#############################################################
## Marionette
## This section handles the communication with the marionette
##############################################################
var = 0
#Create a multidimentional list to hold all the sensor values
multi_list =[]
def marionette(q):
      try:
          marionette = ""
          global tty     
          for p in ports:
            if "COM11" in p.device:
                marionette = p.device
                tty = serial.Serial(port= marionette,timeout=3)
                # disable prompt and echo since we handle that localy with readline input
                tty.write("\r\n".encode('utf-8'))
                tty.write("+noecho\r\n".encode('utf-8'))
                tty.write("+noprompt\r\n".encode('utf-8'))
                tty.write("\r\n".encode('utf-8'))
                tty.write('adc.config(0,16)\r\n'.encode('utf-8'))
                tty.flushInput()

      except (OSError,serial.serialutil.SerialException) as e:
          print(f" Problem stablishing serial connection with Marionette: {e}.")

      def line0_to_AC():
          
          char_remove = ","
          #***********************************************
          try:
            tty.write('adc.single(0)\r\n'.encode('utf-8'))
          except (OSError,serial.serialutil.SerialException) as e:
            print(f" There was a problem sending command to Marionette: {e}")
          # read lines until we get an 'end' marker or we timeout
          try:
            line0 = tty.readline().decode().rstrip('\r\n')
          except (OSError,serial.serialutil.SerialException) as e:
                    print(f" There was a problem reading from Marionette: {e}")
                
            
          while len(line0) > 0:
            if line0.lower().startswith("u16:"):
              for character in char_remove:
                  line0 = line0.replace(character, ' ')
                  
              line0toAC = line0[12:]
              tty.flushInput() 
              return line0toAC
            elif line0.lower().startswith("end:"):
              break
            else:
                try:
                    line0 = tty.readline().decode().rstrip('\r\n')
                except (OSError,serial.serialutil.SerialException) as e:
                    print(f" There was a problem reading from Marionette: {e}")    

      while True:
        line0 = line0_to_AC()
        sensor_values = str(line0)
        q.put(sensor_values, timeout= 1)

######################################################
# AC controller section 
# Consumer Thread
###############################################
def acController(q):
        
        def opening_port():

            try:
                global serial_port
                ac =""
                for p in ports:
                    if "COM9" in p.device:
                        ac = p.device
                        # Port initialization 
                        serial_port = serial.Serial( 
                        port= ac,
                        baudrate=9600, 
                        bytesize= serial.EIGHTBITS, 
                        parity= serial.PARITY_NONE, 
                        stopbits= serial.STOPBITS_ONE, 
                        timeout= 0.5)
            except (OSError,serial.serialutil.SerialException) as e:
                print(f" Problem stablishing serial connection with Actuator Controller: {e})")
                #else: print(" Connection with AC controller failed ")
            
            #serial_port.reset_input_buffer() #If the port is open flush the Input Buffer.
            #serial_port.reset_output_buffer()# also flush the output buffer.
       
        def greeting():
            time.sleep(.3)
            greeting= [['PORTLAND STATE AEROSPACE SOCIETY '],['\nTSAR SOFTWARE'],
                      ['Type help followed by the enter key to access the help  menu.'],
                      ['To finish use exit.\n\r']]
            dash = '*' * 60
            
            for i in range(len(greeting)):
                 if i == 0:
                      print('\r\n')
                      print(dash + "\r")
                      print('{:^60s}'.format(greeting[i][0])+"\r")
                      print(dash + "\r")
                 else:
                      print('{:<60s}'.format(greeting[i][0])+"\r")
                      #print(dash)    

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
        comand = b''
        def write_comand(comand):
            # serial Write 
            try:
                we = len(comand)
                serial_port.write(bytes(comand,'utf8'))
                serial_port.flushInput() 
                
                #time.sleep(0.3)
            except (OSError,serial.serialutil.SerialException) as e:
                  print (f" There was a problem sending data to AC: {e}")
                  #serial_port.close()
                  #opening_port()

        def read_comand(chunk_size=180):
            byte_chunk = b''
            
            # serial Read
            while True:
                # Reading chunks of data,  wait dependent on timeout
                try:
                    byte_chunk = serial_port.readline().decode('utf8')
                        
                except (OSError,serial.serialutil.SerialException) as e:
                    print(e)
  
                if not byte_chunk or re.search("^\s*$", byte_chunk) :
                        print ("Comand was sent but no Data was received...\r\n")
                        
                        #serial_port.close()
                        #opening_port()                
                        break
                else:
                        #there is data 
                        print ("Received: ",byte_chunk, end='\n\r', flush = True)
                        serial_port.flushInput() 
                        break
       
        def comand_check(argument):
                  
                if argument == 'loxprefill':
                    write_comand('7A12\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'valvecheck':
                        write_comand('4DF2\r')
                        time.sleep(.2)
                        read_comand()
                
                elif argument == 'cyclevalves':
                        #make sure we are in valve check state
                        write_comand('E5F2\r')
                        time.sleep(.3)
                        val = serial_port.readline().decode('utf8')
                        
                        # if we are already in valve check 
                        if val[8:] == 'Valve Check\n':
                            #cycle valves inside valvecheck
                            write_comand('E23A\r')
                            time.sleep(.3)
                            read_comand()
                            # read all the AC valve transitions
                            for index in range(10):
                                time.sleep(.3)
                                print(serial_port.readline().decode('utf8'), end='\r\n', flush=True)
                        else:
                            #cycle valves outside valvecheck
                            write_comand('E23A\r')
                            time.sleep(.3)
                            read_comand()
                elif argument == 'stop':
                    write_comand('2B4E\r')
                    time.sleep(.5)
                    read_comand()
                elif argument == 'loxfill':
                    write_comand('5E1A\r')
                    time.sleep(.5)
                    read_comand()
                elif argument == 'loxchill':
                    write_comand('6F8B\r')
                    time.sleep(.5)
                    read_comand()

                elif argument == 'off':
                    write_comand('1AB3\r')
                    time.sleep(.5)
                    read_comand()

                elif argument == 'safety':
                    write_comand('9C45\r')
                    time.sleep(.5)
                    read_comand()

                elif argument == 'pressurize':
                    write_comand('8B23\r')
                    time.sleep(.5)
                    read_comand()

                elif argument == 'starthandler':
                    write_comand('CD89\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'ignition':
                    write_comand('AB67\r')
                    # read ignition
                    read_comand()
                    # wait for 5 sec Oxidizer  
                    start =  time.time()
                    tracker1 = time.time()
                    
                    while tracker1-start < 4.5:
                        value = q.get()
                        
                        if len(value) == 13:
                           #print(f"Sensor Values: {value}",end = '\r', flush= True)
                           write_comand('3F99'+str(value)+'\r')
                           averagin_sensor_values(value)

                        tracker1 = time.time() 
                    
                    read_comand() # for Oxidizer start (5 sec)
                    read_comand() # firing(.2 sec)
                    
                    # wait 3 sec for  purge
                    start = time.time()
                    tracker2 = time.time()

                    while tracker2 - start < 2.6:
                          value = q.get()
                          if len(value)== 13:
                               #print(f"Sensor Values: {value}",end = '\r', flush= True)
                               write_comand('3F99'+str(value)+'\r')
                               averagin_sensor_values(value)
                          tracker2 = time.time()

                    read_comand()
                    
                    # wait 3 sec for safety
                    start =  time.time()
                    tracker3 = time.time()

                    while tracker3 - start < 2.6:
                           value = q.get()
                           if len(value) == 13:
                               #print(f"Sensor Values: {value}",end = '\r', flush= True)
                               write_comand('3F99'+str(value)+'\r')
                               averagin_sensor_values(value)
                           tracker3 = time.time()

                    read_comand()
                    
                elif argument == 'status':
                    write_comand('E5F2\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov1on':
                    write_comand('5F4E\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'status':
                    write_comand('E5F2\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov1on':
                    write_comand('5F4E\r')
                    time.sleep(.5)
                    read_comand()
                    
                    
                elif argument == 'sov1off':
                    write_comand('8D2A\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov1on':
                    write_comand('5F4E\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'sov2on':
                    write_comand('3DEA\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov2off':
                    write_comand('6B88\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'sov3on':
                    write_comand('F1AB\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov3off':
                    write_comand('C23D\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'sov4on':
                    write_comand('B3ED\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov4off':
                    write_comand('8E4A\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'sov5on':
                    write_comand('2AE7\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov5off':
                    write_comand('4A6C\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'sov6on':
                    write_comand('67A8\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov6off':
                    write_comand('3EE2\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'sov7on':
                    write_comand('55A6\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov7off':
                    write_comand('C5B7\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'sov8on':
                    write_comand('B3F8\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov8off':
                    write_comand('88FF\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'sov9on':
                    write_comand('B9AD\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'sov9off':
                    write_comand('DD3F\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'igon':
                    write_comand('4947\r')
                    time.sleep(.5)
                    read_comand()
                
                elif argument == 'igoff':
                    write_comand('34A5\r')
                    time.sleep(.5)
                    read_comand()
                    
                elif argument == 'cyclevalves':
                        write_comand('E23A\r')
                        
                        while True:
                            #uin = stdscr.getch()
                            uin = sys.stdin.read(1)
                            time.sleep(.5)
                            read_comand()
                            
                            if uin == 'q':
                               break
                    
                elif argument == 'cyclevoff':
                    write_comand('6FF3\r')
                    time.sleep(.5)
                    read_comand()
                
                #check if we have a data packet from marionette
                # Length is not efficient but using for testing    
                #elif len(argument) ==  13:
                 #   write_comand(argument)


                elif argument == 'help':
                      help_menu()
                
                
                elif argument == 'exit':
              
                    try:
                        #serial_port.write(bytes('24\r','utf8'))
                        time.sleep(0.3)
                    except (OSError,serial.serialutil.SerialException) as e:
                        print(e)
                
                    serial_port.close()
                    sys.exit()
                # handle invalid command and marionette data packet  
                else:
                    #for testing 
                    if len(argument)== 13:
                        #gh = len(argument)
                        write_comand('3F99 '+argument+'\r')
                        #print(argument[4:], end='\r', flush= True)

                        averagin_sensor_values(argument)

                    '''
                    if len(argument)>0 and argument[:4] == "1234":
                        #Hey this is a data packet, send to AC nuclue
                        #write_comand(argument)
                        print(argument[4:], end=' ', flush= True)
                    else:
                        print("Please enter a valid command...\n")
                    '''
        
        
        def averagin_sensor_values(argument):
                        #print(var)
                        
                        #get the string values and put them in a list
                        a_list = argument.split()
                        mapping = map(int, a_list)
                        values = list(mapping)
                        global multi_list
                        multi_list.append(values) 
                        global var                     
                        var = var +  1
                        #print(var)
                        
                        if var == 16:
			                # calculate the averages
                            avg = list(map(lambda x: sum(x)/len(x), zip(*multi_list)))
                            print('\r'+f'Sensor Values: {avg[0]:.1f} {avg[1]:.1f} {avg[2]:.1f} {avg[3]:.1f} {avg[4]:.1f} {avg[5]:.1f} {avg[6]:.1f}', end="\r")
                            #sys.stdout.write("\033[K") # Cursor up one line
                            var = 0

        opening_port()
        greeting()

        while True:
            # Get the data from marionette and user input
            # from queue and process it
            data = q.get()
            # Just printing marionette values from now
            #if len(data) == 13:
                # print(f"Sensor Values: {data}", end='\r', flush=True)
                #could also write at this point
            if "exit" in data:
                #close ports
                serial_port.close()
                tty.close()
                return 'Exiting successfully.... '
            comand_check(data)

#**************************************************************************************
# Producer Thread
# User Input
#**************************************************************************************
def userIn(q):         
    while True:
        user_in = input("")
        user_input = user_in.lower()
        q.put(user_input)
        
#*************************************************************************************** 

def main():  
    q = Queue()
    with concurrent.futures.ThreadPoolExecutor() as executor:
        # submit once and returns a future object
        t1 = executor.submit(acController,q) # producer thread
        t2 = executor.submit(userIn,      q) # consumer thread
        t3 = executor.submit(marionette,  q) # producer thread
        
        print(t1.result(), end= '\r\n', flush=True)
        #Exiting
        if t1.done():
            executor._threads.clear()
            concurrent.futures.thread._threads_queues.clear()
            #curses.endwin()
            #resets echo 
            #os.system('stty sane')
            sys.exit()

if __name__ == '__main__':
    main()# call main function

