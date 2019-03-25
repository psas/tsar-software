import socket #because it is a server client
import json #for use with json strings
import time #for the sleep oject.
from threading import Thread #for multithreading

#will create a while look
#then take in a argument in python, stash into variable
#then pass variable into encoder, and then put that into the queue.
#and then send off all items in queue.

#get local host machine name
host = '192.168.0.14' #the hosts IP
port = 8080 #configured port

#create the socket
#AF_NET is used because you are using two strings host, port .
#SOCK_STREAM used because using a TCP Socket
serv_sock = socket.socket(socket.AF_INET , socket.SOCK_STREAM)
serv_sock.connect((host,port))


#This function writes all the data server sends to a file.
def recieve(sock):
    while(1):
        data = sock.recv(1024) #This will be in a seperate thread
        i_data = open("writing.txt" , "a")
        i_data.write(repr(data))
        i_data.close()
        #print('Recieved:\n\n' , repr(data) , '\n\n') #as will this.


#allows the user to select the commands they send to the server.
def commands(serv_sock):
    #python dict's containing commands:
    com0 = {'Power': 'FALSE', 'Lights': 'FALSE', 'Camera': 'FALSE'}
    com1 = {'Power': 'TRUE' , 'Lights': 'TRUE' , 'Camera': 'TRUE'}
    com2 = {'Power': 'TRUE' , 'Lights': 'FALSE', 'Camera': 'TRUE'}
    com3 = {'Power': 'TRUE' , 'Lights': 'TRUE' , 'Camera': 'FALSE'}

    seq0 = {'Sequencer_Start': 'FALSE'}
    seq1 = {'Sequencer_Start': 'TRUE'}
    seq2 = {'Sequencer_Start': 'HALT'}

    choice = 0; #set the choice to 0.
    while (choice != 'exit'):

        choice = input("\n\nWhat would you like to do?: \n 1) Turn all off. \n 2) Turn all on.\n 3) Turn lights off.\n 4) Turn Camera off. \n 5) End Sequencer. \n 6) Start Sequencer. \n 7) Halt Sequencer. \n\n type 'exit' to shut down.\n\n")

        if(choice == '1'):
            json_com = json.dumps( com0 )
            send_mess = bytes(json_com, 'utf-8')
            serv_sock.sendall( send_mess  )
        elif(choice == '2'):
            json_com = json.dumps( com1 )
            send_mess = bytes(json_com, 'utf-8')
            serv_sock.sendall( send_mess  )
        elif(choice == '3'):
            json_com = json.dumps( com2 )
            send_mess = bytes(json_com, 'utf-8')
            serv_sock.sendall( send_mess  )
        elif(choice == '4'):
            json_com = json.dumps( com3 )
            send_mess = bytes(json_com, 'utf-8')
            serv_sock.sendall( send_mess  )
        elif(choice == '5'):
            json_com = json.dumps( seq0 )
            send_mess = bytes(json_com, 'utf-8')
            serv_sock.sendall( send_mess  )
        elif(choice == '6'):
            json_com = json.dumps( seq1 )
            send_mess = bytes(json_com, 'utf-8')
            serv_sock.sendall( send_mess  )
        elif(choice == '7'):
            json_com = json.dumps( seq2 )
            send_mess = bytes(json_com, 'utf-8')
            serv_sock.sendall( send_mess  )

        print(send_mess)
if __name__ == "__main__":
    
    thread = Thread(target = recieve , args = (serv_sock , )) #creates the thread to collect the data
    thread_comm = Thread(target = commands , args = (serv_sock , ))

    thread.start() #starts the thread to write all sensor changes to a file.
    thread_comm.start() #options thread

    thread.join() #force the threads to stop.
    thread_comm.join()

    serv_sock.close()
    print('\n\nConnection Closed Sucessfully\n')
