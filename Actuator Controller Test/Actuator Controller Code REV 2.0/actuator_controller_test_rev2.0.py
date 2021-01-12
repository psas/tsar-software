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
       timeout= 0.5)
         
def greeting():
    
    print ("\n* PSAS ACTUATOR CONTROLLER TEST *\n")
    print ("\nType help followed by the enter key to access the help  menu.\n To finish use exit comand. \n")

def help_menu():
    print ("\n   status: shows the current status of each valve and igniters...")
    print("        - 0 : OFF \n        - 1 : ON \n ")
    print ("   Comand Example: Turn valve SOV1 ON, sov1on \n")
    print (" \n **** Valves and Igniters **** \n \n SOV1: Fuel Pressure.\n SOV2: LOX Pressure.\n SOV3: Purge N2.\n SOV4: Solenoid Vent Valve 1.")
    print (" SOV5: Fuel Main. \n SOV6: LOX Main.\n SOV7: LOX Fill.\n SOV8: Solenoid Vent Valve 2.\n SOV9: LOX PURGE.\n IG1:  Igniter 1.\n IG2:  Igniter 2.\n")

# Reading serial for cases where using readline() is not possible.
comand = b''
def write_comand(comand):
    # serial Write 
    try:
        serial_port.write(bytes(comand,'utf8'))
        
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
            # Check if the chunck of data is empty or contain spaces only using regex
            message = read_buffer.decode('utf8')
            
            if not message or re.search("^\s*$", message) :
                print ("Comand was sent but no Data was received. Reestablishing connection...")               
                serial_port.close()
                opening_port()
                break
 
            else:
                #there is data                
                # Print status only when valid Data is received.
                print ("Received: ",message)
                break

    

def comand_check(argument):
      
        if argument == 'sov1on':
             write_comand('1\r')
           
        elif argument == 'sov1off':
             write_comand('2\r')

        elif argument == 'sov2on':
             write_comand('3\r')

        elif argument == 'sov2off':
             write_comand('4\r')

        elif argument == 'sov3on':
             write_comand('5\r')

        elif argument == 'sov3off':
             write_comand('6\r')

        elif argument == 'sov4on':
             write_comand('7\r')

        elif argument == 'sov4off':
             write_comand('8\r')

        elif argument == 'sov5on':
             write_comand('9\r')

        elif argument == 'sov5off':
             write_comand('10\r')

        elif argument == 'sov6on':
             write_comand('11\r')

        elif argument == 'sov6off': 
             write_comand('12\r')

        elif argument == 'sov7on':
             write_comand('13\r')

        elif argument == 'sov7off':
             write_comand('14\r')

        elif argument == 'sov8on':
             write_comand('15\r')

        elif argument == 'sov8off':
             write_comand('16\r')
             
        elif argument == 'sov9on':
             write_comand('17\r')

        elif argument == 'sov9off':
             write_comand('18\r')

        elif argument == 'ig1on':
             write_comand('19\r')

        elif argument == 'ig1off': 
             write_comand('20\r')

        elif argument == 'ig2on':
             write_comand('21\r')

        elif argument == 'ig2off':
             write_comand('22\r')

        elif argument == 'status':
            write_comand('23\r')
            read_comand()
            
        elif argument == 'help':
            help_menu()
        
        elif argument == 'exit':
            #Turn off all the valves and igniters            
            write_comand('24\r')
            time.sleep(0.3)
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

