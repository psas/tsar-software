# BBB serial communication over USB.
# BaudRate : How fast your COM port operates.
# timeout : sets a read time out value
# use : python -m serial.tools.list_ports to see the ports available

import serial, sys, time, os, re

def opening_port():
    
    port_used = ''
    # To account for the case where connection drops.
    if os.path.exists('/dev/ttyACM0')== True:
         port_used = '/dev/ttyACM0'

    elif os.path.exists('/dev/ttyACM1')== True:
        port_used = '/dev/ttyACM1'
    
    print ("\nConnected to port: ", port_used)
    
    # Port initialization 
    global serial_port
    serial_port = serial.Serial( 
       port= str(port_used), 
       baudrate=9600, 
       bytesize= serial.EIGHTBITS, 
       parity= serial.PARITY_NONE, 
       stopbits= serial.STOPBITS_ONE, 
       timeout= .3)
    
    
    #serial_port.reset_input_buffer() #If the port is open flush the Input Buffer.
    #serial_port.reset_output_buffer()# also flush the output buffer.
         
    
      
def greeting():
    greeting= [['PORTLAND STATE AEROSPACE SOCIETY '],['\nTSAR SOFTWARE'],
              ['Type help followed by the enter key to access the help  menu.'],
              ['To finish use exit.']]
    dash = '*' * 60
    for i in range(len(greeting)):
        if i == 0:
            print('\n')
            print(dash)
            print('{:^60s}'.format(greeting[i][0]))
            print(dash)
        else:
            print('{:<60s}'.format(greeting[i][0]))
        
    #print(dash)    


def help_menu():
   
    commands=[[' GENERAL COMMANDS  ',' VALVE CHECK COMMANDS ', ' Valves '],
          [' status: Current state. ',' cyclevalves: Automatic valve check.',' SOV1: Fuel Pressure'],
          [' loxprefill',' cyclevoff: Automatic sequence OFF.',' SOV2: Lox Pressure '],
          [' loxfill', 'Turn ON valve: sov1on ', ' SOV3: Fuel Purge N2'],[' loxchill','Turn OFF Valve: sov1off',' SOV4: Fuel Vent'],
          [' safety','',' SOV5: Fuel Main '],[' pressurize',' ',' SOV6: Lox Main '],[' ignition',' ',' SOV7: Lox Fill'],
          [' off: Terminate AC software.',' ',' SOV8: Lox Vent'], [' valvecheck',' ',' SOV9: Lox Purge'], [' ',' ',' IG:   Igniter']]
            
            
    dash = '-' * 93 
    for i in range(len(commands)):
        if i == 0:
            print(dash)
            print('|{:^28s}|{:^40s}|{:^21}|'.format(commands[i][0],commands[i][1],commands[i][2]))
            print(dash)
        else:
            print('|{:<28s}|{:^40s}|{:<21}|'.format(commands[i][0],commands[i][1], commands[i][2]))
        
    print(dash)     


    valid_transitions=[[' VALID TRANSITIONS BETWEEN STATES '],['Start Handler -> Valve Check -> OFF'],
                      ['Start Handler -> Lox Pre Fill -> Lox Fill -> Lox Chill -> Safety -> Pressurize -> Ignition'],
                      ['(Auto Start):Ignition (5s) -> Oxidizer Start (.2s) -> Firing(3s) -> Purge(3s) -> Safety (Auto End)']]
                      
    dash = '-' * 100 
    for i in range(len(valid_transitions)):
        if i == 0:
            print(dash)
            print('|{:^98s}|'.format(valid_transitions[i][0]))
            print(dash)
        else:
            print('|{:<98s}|'.format(valid_transitions[i][0]))
        
    print(dash)     


# Reading serial for cases where using readline() is not possible.
comand = b''
def write_comand(comand):
    # serial Write 
    try:
        serial_port.write(bytes(comand,'utf8'))
        #time.sleep(0.3)
    except (OSError,serial.serialutil.SerialException) as e:
          print(e)
          print ("Attempting to reestablish connection ...")
          serial_port.close()
          opening_port()
 


def read_comand(chunk_size=180):
    read_buffer = byte_chunk = b''
    
    # serial Read
    while True:
        # Reading chunks of data,  wait dependent on timeout
        try:
            byte_chunk = serial_port.read(size=chunk_size)
                
        except (OSError,serial.serialutil.SerialException) as e:
            print(e)
        
        read_buffer += byte_chunk
      
        if not len(byte_chunk) == chunk_size:
            # Check if the chunck of data is empty or contain spaces only
            message = read_buffer.decode('utf8')
            
            if not message or re.search("^\s*$", message) :
                print ("Comand was sent but no Data was received...")
                
                #serial_port.close()
                #opening_port()                
                break
            else:
                #there is data 
                #time.sleep(0.5)
                # Print status only when valid Data is received.
                #FaultTransitions(message)
                print ("Received: ",message)
                break

    
def comand_check(argument):
          
        if argument == 'loxprefill':
             write_comand('7A12\r')
             time.sleep(.5)
             read_comand()
             
        elif argument == 'valvecheck':
             write_comand('4DF2\r')
             time.sleep(.5)
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
             #time.sleep(.1)
             read_comand()
             time.sleep(4.7)
             read_comand()  # for Oxidizer and firing (only .2 sec)
             time.sleep(3)
             read_comand()
             time.sleep(3)
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
             time.sleep(.5)
             read_comand()
             
        elif argument == 'cyclevoff':
             write_comand('6FF3\r')
             time.sleep(.5)
             read_comand()
             
                  
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
            
        else:
            print("Please enter a valid command...\n")


opening_port()
greeting()
while True:
           
           user_in = input("> ")
           user_in = user_in.lower()
           comand_check(user_in)  