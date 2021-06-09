#########################################################################################
# Capstone 2020 
# by Josh Pradera 
# Actuator Controller script
#
# This script works with the Actuator Controller only. 
# File name: ac.py
# 
##########################################################################################

import serial, sys, time, os, re
import serial.tools.list_ports
ports = list(serial.tools.list_ports.comports())
import multiprocessing
#import threading 

# SETUP pyserial parameters for the Actuator Controller
def opening_port():
    try:
        global serial_port
        # Port initialization 
        ac =""
        for p in ports:
          #check for PSAS AC nucleo serial number
          if '0662FF544856846687095229' in p.serial_number:
             ac = p.device
             
             serial_port = serial.Serial( 
             port= str(ac),
             baudrate=9600, 
             bytesize= serial.EIGHTBITS, 
             parity= serial.PARITY_NONE, 
             stopbits= serial.STOPBITS_ONE, 
             timeout= .5)
        
        # Tread writer
        writer = multiprocessing.Process(target=writeall, args=(serial_port, ))
        #writer = threading.Thread(target = writeall, args = (serial_port,))
        writer.daemon = True
        writer.start()
  
    except (OSError,serial.serialutil.SerialException) as e:
        print(f" Problem stablishing serial connection with Actuator Controller: {e})")    
    

def greeting():
    greeting= [['PORTLAND STATE AEROSPACE SOCIETY '],['\nTSAR SOFTWARE'],
              ['Type help followed by the enter key to access the help  menu.'],
              ['To finish use exit.']]
    dash = '*' * 60
    print('\r\n')
    print('\r\n')
    for i in range(len(greeting)):
        if i == 0:
            print('\r\n')
            print(dash + "\r")
            print('{:^60s}'.format(greeting[i][0])+"\r")
            print(dash + "\r")
        else:
            print('{:<60s}'.format(greeting[i][0])+"\r") 

#The menu contains all the valid transitions and valid commands that we can use to work with the Actuator Controller
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
def write_comand(comand):
    # serial Write 
    try:
        #Reset the buffer before sending a command 
        serial_port.reset_output_buffer()
        serial_port.reset_input_buffer()  
        serial_port.write(bytes(comand,'utf8'))
        time.sleep(0.003)
        
    except (OSError,serial.serialutil.SerialException) as e:
          print(e)
          print('Resetting Actuator Controller...\n')
          write_comand('B6D8\r')# reset command
 
###############################################################################################
# Writer Process that writes anything in stdout
# Daemon status (ends when main finish)
################################################################################################
def writeall(serial_port):
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

                    else:
                        #append all the bytes that are being received
                        all_bytes.append(str(data))
                

            except (OSError,serial.serialutil.SerialException) as e:
                print(f'Actuator Controller: {e}') 

# This function process the user commands and sends the HEX command that the AC expects.                
def comand_check(argument):
          
        if argument == 'loxprefill':
            write_comand('7A12\r')

        elif argument == 'valvecheck':
                write_comand('4DF2\r')
   
        elif argument == 'cyclevalves':
            write_comand('E23A\r')

        elif argument == 'stop':
            write_comand('2B4E\r')

        elif argument == 'loxfill':
            write_comand('5E1A\r')

        elif argument == 'resetac':
            write_comand('B6D8\r')

        elif argument == 'loxchill':
            write_comand('6F8B\r')

        elif argument == 'off':
            write_comand('1AB3\r')

        elif argument == 'safety':
            write_comand('9C45\r')

        elif argument == 'pressurize':
            write_comand('8B23\r')

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
        
        else:
            if 'None' in argument:
                pass
            else:
                print("Please enter a valid command...\n")



# Main function handles the AC serial setup, greeting and the user commands 
def main():
    opening_port()
    greeting()
    while True:                
               user_in = input("> ")
               user_in = user_in.lower()
               if 'exit' in user_in:
                    serial_port.close()
                    break
               else:

                    comand_check(user_in)  
                    
if __name__ == '__main__':
    main()

