
from tkinter import *
from tkinter import ttk
from tkinter.messagebox import showinfo
from PIL import Image, ImageTk
from tkinter import messagebox
###############################
# Paramiko
import base64
import getpass
import os
import socket
import sys, time
import traceback
#import interactive
from paramiko import client
from paramiko.py3compat import input
import concurrent.futures # uses the tHread pool executor 
from queue import Queue 
import paramiko
from paramiko.py3compat import u
import threading
import os.path
from os import path
from tkinter import filedialog

#q = Queue()
start_parsing = 0
ON = 1

def init_gui():
    global root, output, entry
    # now, connect and use paramiko Client to negotiate SSH2 across the connection
    root = Tk() 
    root.title(" PORTLAND STATE AEROSPACE SOCIETY ") 
    root.geometry("1050x580") 
    # Create the canvas
    my_canvas = Canvas(root, width=3000, height=1703)
    my_canvas.pack()
    #Add label
    tsar_label = Label(text= "TSAR", font = ("Helvetica", 25), fg = "dark blue")
    tsar_label.place(relx=0.05, rely=0.06, relwidth= 0.2, relheight = 0.07)
    #Add button 
    '''
    button1 = Button(root, text="Logged Data", command=lambda: openfile())
    button1.place(relx=0.736, rely=0.127, relwidth= 0.12, relheight = 0.045)
    '''
    button2 = Button(root, text="Help", command =lambda: help_window())
    button2.place(relx=0.867, rely=0.127, relwidth= 0.05, relheight = 0.045)
    button3 = Button(root, text="P&ID", command=lambda m='opened': openWindow(m))
    button3.place(relx=0.82, rely=0.127, relwidth= 0.04, relheight = 0.045)
    #button3.place(relx=0.686, rely=0.127, relwidth= 0.04, relheight = 0.045)

    #Add label for cmd box
    command = Label(text ="Enter a command:",font = ("Helvetica", 15))
    command.place(relx=0.06, rely=0.14, relwidth= 0.2, relheight = 0.07)

    #Command entry box
    entry = Entry(root, font=("Helvetica", 16),fg ="dark blue", border=3)#, command = user_command)
    entry.place(relx=0.06, rely=0.2, relwidth= 0.2, relheight = 0.07)
    entry.bind("<Return>", (lambda event:user_command_event(entry.get())))
    #Add label for the output window
    output_label = Label(text ="Output:",font = ("Helvetica", 15))
    output_label.place(relx=0.27, rely=0.14, relwidth= 0.2, relheight = 0.07)

    output = Text(root, font=("Helvetica", 16), width=50, height= 10, fg ="dark blue", border=3)
    output.place(relx=0.35, rely=0.2, relwidth= 0.6, relheight = 0.6)
    root.protocol("WM_DELETE_WINDOW", on_closing_main)
    root.mainloop()
'''    
#change the text widget box depending in the value 
def box_color_change(col_bg, col_fg, box):
    box["bg"] = col_bg
    box["fg"] = col_fg
    box.insert(END,"")

def openfile():
    os.system('log_gui.txt')
'''
def help_window():
   os.system('help.txt')
#########################   
# Closing PID window
def on_closing_pid():
    if messagebox.askokcancel("Quit", "Do you want to close the P&ID window?"):
        global start_parsing
        start_parsing = 0
        newWindow.destroy()
#Closing the main window
def on_closing_main():
    if messagebox.askokcancel("Quit", "Do you want to close the main window?"):
        global ON
        ON = 0
        root.destroy()
            
def openWindow(m):
    global col_bg, col_fg, my_canvas2, newWindow, bg1, box1, box2, box3, box4, box5, box6, box7, box8, box9, box10, box11, box12, box13, box14, box15, box16 
    newWindow = Toplevel(root)
    newWindow.geometry("1400x795")
    newWindow.title("P&ID")
    my_canvas2 = Canvas(newWindow, width=3000, height=1703)   
    my_canvas2.pack()

    bg1 = ImageTk.PhotoImage(file = "pid.png")
    my_canvas2.create_image(0,0, image = bg1, anchor = "nw")

    #default colors for the sensor boxes
    col_bg = "light yellow"
    col_fg = "black"
    #PT-F01
    box1 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box1.place(relx=0.2, rely=0.05, relwidth= 0.07, relheight = 0.05)
    #PT-F02
    box2 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box2.place(relx=0.44, rely=0.05, relwidth= 0.07, relheight = 0.05)
    #PT-F03
    box3 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box3.place(relx=0.51, rely=0.05, relwidth= 0.07, relheight = 0.05)
    #TT-F01
    box4 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box4.place(relx=0.58, rely=0.05, relwidth= 0.06, relheight = 0.05)
    #PT-F04
    box5 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box5.place(relx=0.66, rely=0.03, relwidth= 0.06, relheight = 0.05)
    #PT-M02
    box6 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box6.place(relx=0.85, rely=0.23, relwidth= 0.06, relheight = 0.05)
    #TT-L05
    box7 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box7.place(relx=0.82, rely=0.51, relwidth= 0.06, relheight = 0.05)
    #TT-L04
    box8 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box8.place(relx=0.62, rely=0.73, relwidth= 0.06, relheight = 0.05)
    #PT-L04
    box9 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box9.place(relx=0.68, rely=0.54, relwidth= 0.06, relheight = 0.05)
    #TT-L03
    box10 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box10.place(relx=0.59, rely=0.52, relwidth= 0.06, relheight = 0.05)
    #PT-L03
    box11 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box11.place(relx=0.53, rely=0.52, relwidth= 0.06, relheight = 0.05)
    #PT-L02
    box12 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box12.place(relx=0.467, rely=0.51, relwidth= 0.06, relheight = 0.05)
    #TT-L01
    box13 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box13.place(relx=0.42, rely=0.68, relwidth= 0.06, relheight = 0.05)
    #TT-L02
    box14 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box14.place(relx=0.42, rely=0.73, relwidth= 0.06, relheight = 0.05)
    #PT-L01
    box15 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box15.place(relx=0.20, rely=0.516, relwidth= 0.06, relheight = 0.05)
    #PT-M01
    box16 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = col_fg, bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box16.place(relx=0.07, rely=0.29, relwidth= 0.06, relheight = 0.05)
    
    if 'opened' in m:
        #flag variable
        global start_parsing
        start_parsing = 1

    newWindow.protocol("WM_DELETE_WINDOW", on_closing_pid)
    my_canvas2.bind("<Configure>",resizer)
    newWindow.mainloop()

def resizer(e):
    global bg2, resized_bg2, new_bg2
    #open the desired image
    bg2 = Image.open("pid.png")
    resize_bg2 = bg2.resize((e.width, e.height), Image.ANTIALIAS)
    new_bg2 = ImageTk.PhotoImage(resize_bg2)
    my_canvas2.create_image(0,0, image = new_bg2, anchor = "nw")

#################################################################################
def user_command_event(command):
    if command == 'clear':
        #command to Clear the output section 
        output.delete(1.0, END)
    if command == 'exit':
        chan.send("exit\n")
        return 'closing'
    chan.send(str(command)+'\n')
    entry.delete(0, END)
##############################################################################

def init_paramiko():
    # needed parameters for connection    
    port = 22
    hostname = 'beaglebone'
    username = 'debian'
    password = 'Capstone2020'
    ##############################
    #determine if we are in linux or windows 
    try:
        import termios
        import tty

        has_termios = True
    except ImportError:
        has_termios = False

    try:
        global chan
        client = paramiko.SSHClient()
        client.load_system_host_keys()
        client.set_missing_host_key_policy(paramiko.WarningPolicy())
        print("*** Connecting...")
        client.connect(hostname, port, username, password)

        chan = client.invoke_shell()
        print(repr(client.get_transport()))
        output.insert("end-1c",'*** SSH Connection to BB_AI stablished ***\n')
        chan.send('python3 bb.py\n')
        print("*** SSH Connection to BB_AI stablished!\n")
        #creating the log file 
        #if  the file exist 
        if path.exists("log_gui.txt"):
            #then append to the existing file
            file = open("log_gui.txt", "+a")
        else:
            #create a new one
            file = open("log_gui.txt", "+w")
        ###########################################################################
        if has_termios:
            import select

            oldtty = termios.tcgetattr(sys.stdin)
            try:
                tty.setraw(sys.stdin.fileno())
                tty.setcbreak(sys.stdin.fileno())
                chan.settimeout(0.0)

                while True:
                    r, w, e = select.select([chan, sys.stdin], [], [])
                    if chan in r:
                        try:
                            c = 0
                            x = u(chan.recv(1024))
                            if len(x) == 0:
                                sys.stdout.write("\r\n*** Done ***\r\n")
                                chan.close()
                                client.close()
                                break

                            #strip non useful info
                            if c <= 1:
                                data = data[400:]
                                c += 1
                            if str(data).startswith('debian@beaglebone:~$'):
                                data = str(data).replace('debian@beaglebone:~$ python3 bb.py', ' ') 
                            if str(data).startswith('python3'):
                                data = str(data).replace('python3 bb.py', ' ')
                                                
                            if start_parsing:
                                # Parse the sensor values 
                                if 'Sensor' in str(data):
                                    #change the box color to red to indicate warning
                                    if float(data[16:19]) > 1.0:
                                        box1["bg"] = "#fb9898" #light red
                                        box1["fg"] = "black"
                                        box1.insert(END,"")
                                        #box_color_change("red", "black", "box1")
                                    box1.delete(1.0, END)
                                    box1.insert('1.0', data[16:19])
                                    #
                                    box2.delete(1.0, END)
                                    box2.insert("1.0",data[20:23]) 
                                    #
                                    box3.delete(1.0, END)
                                    box3.insert("1.0",data[24:27]) 
                                    #
                                    box4.delete(1.0, END)
                                    box4.insert("1.0",data[28:31]) 
                                    #
                                    box5.delete(1.0, END)
                                    box5.insert("1.0",data[32:35])
                                    #
                                    box6.delete(1.0, END)
                                    box6.insert("1.0",data[36:39])  
                                    #
                                    '''
                                    box7.delete(1.0, END)
                                    box7.insert("1.0",data[40:43])
                                    #
                                    
                                    box8.delete(1.0, END)
                                    box8.insert("1.0",data[44:47])
                                    #
                                    box9.delete(1.0, END)
                                    box9.insert("1.0",data[40:43])
                                    #
                                    box10.delete(1.0, END)
                                    box10.insert("1.0",data[44:47])
                                    #
                                    box11.delete(1.0, END)
                                    box11.insert("1.0",data[48:51])
                                    #
                                    box12.delete(1.0, END)
                                    box12.insert("1.0",data[52:55])
                                    #
                                    box13.delete(1.0, END)
                                    box13.insert("1.0",data[56:59])
                                    #
                                    box14.delete(1.0, END)
                                    box14.insert("1.0",data[60:63])
                                    #
                                    box15.delete(1.0, END)
                                    box15.insert("1.0",data[64:67])
                                    #
                                    box16.delete(1.0, END)
                                    box16.insert("1.0",data[68:71])
                                    '''
                            if 'Sensor' in str(x): 
                                continue     
                            output.insert("end-1c",x)
                            output.see("end")
                            #for testing
                            #print(x, end= '\r', flush= True)

                            #sys.stdout.write(x)
                            #sys.stdout.flush()
                        except socket.timeout:
                            pass
                if not ON:
                            return "Exiting GUI"

                #writing in console
                '''    
                if sys.stdin in r:
                    x = sys.stdin.read(1)
                    if len(x) == 0:
                        break
                    chan.send(x)
                '''

            finally:
                termios.tcsetattr(sys.stdin, termios.TCSADRAIN, oldtty)

        # if we are in a windows based environment
        else:
            def writeall(sock):
                count = 0
                while True:
                    data = sock.recv(9999).decode('utf8')
                    file.write(data+'\n')
                    if not data or not ON:
                        sys.stdout.write("\r\n*** Done ***\r\n\r\n")
                        sys.stdout.flush()
                        file.close()
                        chan.close()
                        client.close()
                        break
                    #strip non useful info
                    if count <= 1:
                        data = data[400:]
                        count += 1
                    if str(data).startswith('debian@beaglebone:~$'):
                        data = str(data).replace('debian@beaglebone:~$ python3 bb.py', ' ') 
                    if str(data).startswith('python3'):
                        data = str(data).replace('python3 bb.py', ' ')
                                        
                    if start_parsing:
                        # Parse the sensor values 
                        if 'Sensor' in str(data):
                            #change the box color to red to indicate warning
                            if float(data[16:19]) > 1.0:
                                box1["bg"] = "#fb9898" #light red
                                box1["fg"] = "black"
                                box1.insert(END,"")
                            box1.delete(1.0, END)
                            box1.insert('1.0', data[16:19])
                            #
                            if float(data[20:23]) > 1.0:
                                box2["bg"] = "#fb9898" #light red
                                box2["fg"] = "black"
                                box2.insert(END,"")
                            box2.delete(1.0, END)
                            box2.insert("1.0",data[20:23]) 
                            #
                            if float(data[24:27]) > 1.0:
                                box3["bg"] = "#fb9898" #light red
                                box3["fg"] = "black"
                                box3.insert(END,"")
                            box3.delete(1.0, END)
                            box3.insert("1.0",data[24:27]) 
                            #
                            if float(data[28:31]) > 1.0:
                                box4["bg"] = "#fb9898" #light red
                                box4["fg"] = "black"
                                box4.insert(END,"")
                            box4.delete(1.0, END)
                            box4.insert("1.0",data[28:31]) 
                            #
                            if float(data[32:35]) > 1.0:
                                box5["bg"] = "#fb9898" #light red
                                box5["fg"] = "black"
                                box5.insert(END,"")
                            box5.delete(1.0, END)
                            box5.insert("1.0",data[32:35])
                            #
                            if float(data[36:39]) > 1.0:
                                box6["bg"] = "#fb9898" #light red
                                box6["fg"] = "black"
                                box6.insert(END,"")
                            box6.delete(1.0, END)
                            box6.insert("1.0",data[36:39])  
                            #
                            '''
                            box7.delete(1.0, END)
                            box7.insert("1.0",data[40:43])
                            #
                            
                            box8.delete(1.0, END)
                            box8.insert("1.0",data[44:47])
                            #
                            box9.delete(1.0, END)
                            box9.insert("1.0",data[40:43])
                            #
                            box10.delete(1.0, END)
                            box10.insert("1.0",data[44:47])
                            #
                            box11.delete(1.0, END)
                            box11.insert("1.0",data[48:51])
                            #
                            box12.delete(1.0, END)
                            box12.insert("1.0",data[52:55])
                            #
                            box13.delete(1.0, END)
                            box13.insert("1.0",data[56:59])
                            #
                            box14.delete(1.0, END)
                            box14.insert("1.0",data[60:63])
                            #
                            box15.delete(1.0, END)
                            box15.insert("1.0",data[64:67])
                            #
                            box16.delete(1.0, END)
                            box16.insert("1.0",data[68:71])
                            '''
                    if 'Sensor' in str(data): 
                        continue     
                    output.insert("end-1c",data)
                    output.see("end")
                    #for testing
                    #print(data, end= '\r', flush= True)
                if not ON:
                    return "Exiting GUI"

            writer = threading.Thread(target=writeall, args=(chan,))
            writer.start()
         
    except Exception as e:
        print("*** Caught exception: %s: %s" % (e.__class__, e))
        traceback.print_exc()
        try:
            client.close()
        except:
            pass
        sys.exit(1)
    
#Running GUI and Paramiko in threaded way 
def main():      
    with concurrent.futures.ThreadPoolExecutor() as executor:
        # submit once and returns a future object
        t1 = executor.submit(init_paramiko) # producer thread
        init_gui()
        
        print(t1.result(), end= '\r\n', flush=True)
        #Exiting
        if t1.done():
            executor._threads.clear()
            concurrent.futures.thread._threads_queues.clear()
            sys.exit()
              

if __name__ == "__main__" : main() 




