import socket #because it is a server client
import json #for use with json strings
from threading import Thread #for multithreading
import time

def Main():	
    print("Beginning") 
    CMTT = CentralManagerTCPTester(host = '127.0.0.1')    
    command_thread = Thread(target = CommandInterface , args = (CMTT))
    command_thread.start()
    CMTT.join()


def CommandInterface(CMTT):
    while True:
        commandChoices = ('Power': 'FALSE', 'Lights': 'FALSE', 'Camera': 'FALSE') ,
                ('Power': 'TRUE' , 'Lights': 'TRUE' , 'Camera': 'TRUE'),
                ('Power': 'TRUE' , 'Lights': 'FALSE', 'Camera': 'TRUE'),
                ('Power': 'TRUE' , 'Lights': 'TRUE' , 'Camera': 'FALSE'),
                ('Sequencer_Start': 'FALSE'),
                ('Sequencer_Start': 'TRUE'),
                ('Sequencer_Start': 'HALT')
        
        choice = int(input("\n\nWhat would you like to do?: \n 1) Turn all off. \n 2) Turn all on.\n 3) Turn lights off.\n 4) Turn Camera off. \n 5) End Sequencer. \n 6) Start Sequencer. \n 7) Halt Sequencer. \n\n type 'exit' to shut down.\n\n"))
        CMTT.sock.sendall(commandChoices[choice])



class CentralManagerTCPTester:
    def __init__(self, host = '192.168.0.14', port = 8080):
        print("Connecting TSAR Server")
        self.host = host
        self.port = port

        #create the socket
        #AF_NET is used because you are using two strings host, port .
        #SOCK_STREAM used because using a TCP Socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host,self.port))
        self.receiveThread = Thread(target = self.recieve) #creates the thread to collect the data
        print("Client Receive Threads Initialized")
        self.receiveThread.start()

    #TODO: For some reason, this is printout out python type prefixes on data? Just print raw data to text file!
    #TODO: Start new file for every execution of program. Use UUID library for name generation. Use format TSAR_DATA_[date]_[UUID]
    def recieve(self):
        print("Client Receive Thread executing")
        while(1):
            data = self.sock.recv(1024) #This will be in a separate thread
            i_data = open("writing.txt" , "a")
            i_data.write(repr(data))
            i_data.close()
            print('Recieved:\n\n' , repr(data) , '\n\n') #as will this.

    def sendCommand(self, command):
        #TODO: Send four-byte-long prefix containing length of JSON string
        self.sock.sendall(bytes(json.dumps(command),'utf-8'))

    def close(self):
        self.sock.close()
        self.receiveThread.exit()
        print('\n\nConnection Closed Sucessfully\n')

###########################################################
###########################################################
###########################################################

if __name__ == "__main__":
    Main()
