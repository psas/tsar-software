# BBB serial communication over USB.
# BaudRate : How fast your COM port operates.
# timeout : sets a read time out value

import serial
import sys
#import itertools
#sov1= sov2= sov3= sov4= sov5= sov6= sov7= sov8= ig1= ig2 = 0

serial_port = serial.Serial( port='/dev/ttyACM0', baudrate=9600, bytesize=8, parity='N', stopbits= 1, timeout=1)# open serial port
serial_port.reset_input_buffer() #tells serial port  to clear the queue so that data dosnt overlap and create erroneous data points.
serial_port.reset_output_buffer()




print ("\n* PSAS ACTUATOR CONTROLLER TEST *\n")
print ("\nType help followed by the enter key to access the help  menu.\n To finish use exit command. \n")
def help_menu():
    #print ("\n  status: shows the current status of each valve and ignitors...")
    print ("   Command Example: Turn valve SOV1 ON, sov1on \n")
    print (" \n **** Valves and Igniters **** \n \n SOV1: Fuel Pressure.\n SOV2: LOX Pressure.\n SOV3: Purge N2.\n SOV4: Solenoid Vent Valve 1.")
    print (" SOV5: Fuel Main. \n SOV6: LOX Main.\n SOV7: LOX Fill.\n SOV8: Solenoid Vent Valve 2.\n IG1:  Igniter 1.\n IG2:  Igniter 2.\n")

    

def comand_check(argument):
       

        if argument == 'sov1on':
             #sov1 = 1
             serial_port.write(bytes('1\r','utf8'))
             serial_port.flush()
             print ("Received: ", serial_port.readline().decode('utf8'))
             
        elif argument == 'sov1off':
             serial_port.write(bytes('2\r','utf8'))
             serial_port.flush()
             print ( "Received: ",serial_port.readline().decode('utf8'))
             # sov1 = 0
        elif argument == 'sov2on':
             serial_port.write(bytes('3\r','utf8'))
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8'))
             #sov2 = 1
        elif argument == 'sov2off':
             serial_port.write(bytes('4\r','utf8'))
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8'))
             #sov2 = 0
        elif argument == 'sov3on':
             serial_port.write(bytes('5\r','utf8'))
             print ( "Received: ",serial_port.readline().decode('utf8'))
             #sov3 = 1
        elif argument == 'sov3off':
             serial_port.write(bytes('6\r','utf8'))
             print ("Received: ",serial_port.readline().decode('utf8'))
             #sov3 = 0
        elif argument == 'sov4on':
             serial_port.write(bytes('7\r','utf8')) 
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8')) 
             #sov4 = 1
        elif argument == 'sov4off':
             serial_port.write(bytes('8\r','utf8'))
             serial_port.flush()
             print ("Received: ", serial_port.readline().decode('utf8'))
             #sov4 = 0
        elif argument == 'sov5on':
             serial_port.write(bytes('9\r','utf8'))
             serial_port.flush()
             print ( "Received: ",serial_port.readline().decode('utf8'))
             #sov5 = 1
        elif argument == 'sov5off':
             serial_port.write(bytes('10\r','utf8'))
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8'))
             #sov5 = 0
        elif argument == 'sov6on':
             serial_port.write(bytes('11\r','utf8')) 
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8')) 
             #sov6 = 1
        elif argument == 'sov6off': 
             serial_port.write(bytes('12\r','utf8'))
             serial_port.flush()
             print ( "Received: ",serial_port.readline().decode('utf8'))
             #sov6 = 0
        elif argument == 'sov7on':
             serial_port.write(bytes('13\r','utf8'))
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8'))
             #sov7 = 1
        elif argument == 'sov7off':
             serial_port.write(bytes('14\r','utf8')) 
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8')) 
             #sov7 = 0
        elif argument == 'sov8on':
             serial_port.write(bytes('15\r','utf8'))
             serial_port.flush()
             print ( "Received: ",serial_port.readline().decode('utf8'))
             #sov8 = 1
        elif argument == 'sov8off':
             serial_port.write(bytes('16\r','utf8'))
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8'))
             #sov8 = 0
        elif argument == 'ig1on':
             serial_port.write(bytes('17\r','utf8')) 
             print ("Received: ",serial_port.readline().decode('utf8'))
             #ig1 = 1
        elif argument == 'ig1off': 
             serial_port.write(bytes('18\r','utf8'))
             serial_port.flush()
             print ( "Received: ",serial_port.readline().decode('utf8'))
             #ig1 = 0
        elif argument == 'ig2on':
             serial_port.write(bytes('19\r','utf8'))
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8'))
             #ig2 = 1
        elif argument == 'ig2off':
             serial_port.write(bytes('20\r','utf8')) 
             serial_port.flush()
             print ("Received: ",serial_port.readline().decode('utf8'))
             #ig2 = 0



            #elif argument == 'status':
            #print("\n*** Valves and Igniters Status ***\n\n SOV1: {}\n SOV2: {}\n SOV3: {}\n SOV4: {}\n SOV5: {}\n SOV6: {}\n SOV7: {}\n SOV8: {}\n IG1:  {}\n IG2:  {}\n" .format( sov1, sov2, sov3, sov4, sov5, sov6, sov7, sov8, ig1, ig2))
            #print("\n*** Valves and Igniters Status ***\n\n"" SOV1: \n",sov1," SOV2: \n",sov2," SOV3: \n",sov3," SOV4: \n",sov4," SOV5: \n",sov5," SOV6: \n",sov6," SOV7: \n",sov7," SOV8: \n",sov8," IG1:  \n",ig1," IG2:  \n" ,ig2)
            #serial_port.write(bytes('21\r','utf8'))
            #serial_port.flush()
            #print (serial_port.read(150).decode('utf8'))
            #serial_port.flush()
        
          
            
        elif argument == 'help':
            help_menu()
        
        elif argument == 'exit':
            
            #Turn off all the valves and igniters
            #serial_port.write(bytes('22\r','utf8'))
            #serial_port.reset_input_buffer() #tells serial port  to clear the queue so that data dosnt overlap and create erroneous data points.
            #serial_port.reset_output_buffer()
            #serial_port.flush()
            serial_port.close()
            sys.exit()
            
        else:
            print("Please enter a valid command...\n")

while True:
    user_in = input("> ")
    user_in = user_in.lower()
    comand_check(user_in)
    

