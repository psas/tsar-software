#########################################################################################
# Capstone 2020- 2021
# by Josh Pradera 
# Graphical User Interface (Linux Version)
# 
#This script handles the Tkinter graphical interface as well as the Paramiko SSH to the 
# BB_AI script. From paramiko we can read all the stdout data and parse it to what we want to
# display in the GUI.
#
##########################################################################################
from tkinter import *
from tkinter.messagebox import showinfo
from PIL import Image, ImageTk
from tkinter import messagebox

###############################
# Paramiko
import socket
import sys
import traceback
#import interactive
from paramiko import client
from paramiko.py3compat import input
import concurrent.futures # uses the tHread pool executor 
import paramiko
from paramiko.py3compat import u
import threading
from os import path
import os
from queue import Queue

#Flag variable to change solenoid valve to red color
flag_valve_red = False
#Flag to signal that the P&ID is open
start_parsing = 0
# variable to hold the state status in the P&ID window
state_preview = ''
#FLag to wait untill the greeting from multibb.py is received to send a status command
ok_to_send = False

sig = False
# resending the status commands if it fails
sig_to_resend = False
count = 0
ON = 1

q = Queue()

__location__ = path.realpath(path.join(os.getcwd(), path.dirname(__file__)))


def init_gui():
    global root, output, entry, my_canvas, logo_label
    # now, connect and use paramiko Client to negotiate SSH2 across the connection
    root = Tk() 
    root.title(" PORTLAND STATE AEROSPACE SOCIETY ") 
    root.geometry("1050x580") 
    # Create the canvas
    my_canvas = Canvas(root, width=3000, height=1703, bg= "white")
    my_canvas.pack()

    #Add label
    tsar_label = Label(text= "TSAR", font = ("Helvetica", 25), fg = "dark blue", bg= "white")
    tsar_label.place(relx=0.05, rely=0.06, relwidth= 0.2, relheight = 0.07)

    logo = ImageTk.PhotoImage(file = "logo.png")
    logo_label = Label(root, image= logo, bg = "white")
    logo_label.place(relx=0.047, rely=0.75, relwidth= 0.1, relheight = 0.17)
  
    ###############################################################################
    # Buttons
    #################################################################################
    button2 = Button(root, text="Help", command =lambda: help_win())
    button2.place(relx=0.867, rely=0.127, relwidth= 0.05, relheight = 0.045)
    button3 = Button(root, text="P&ID", command=lambda m='opened': openWindow(m))
    button3.place(relx=0.82, rely=0.127, relwidth= 0.04, relheight = 0.045)
    button4 = Button(root, text = "Shutdown",border = "2", font=("Helvetica", 16, "italic"),fg = "white", bg = "#d43a3a",command = lambda: auto_shutdown())
    button4.place(relx=0.06, rely=0.4, relwidth= 0.2, relheight = 0.2)

    #Add label for cmd box
    command = Label(text ="Enter a command:",font = ("Helvetica", 15), bg= "white")
    command.place(relx=0.06, rely=0.14, relwidth= 0.2, relheight = 0.07)

    #Command entry box
    entry = Entry(root, font=("Helvetica", 16),fg ="dark blue", border=3)
    entry.place(relx=0.06, rely=0.2, relwidth= 0.2, relheight = 0.07)
    entry.bind("<Return>", (lambda event:user_command_event(entry.get())))

    #Add label for the output window
    output_label = Label(text ="Output:",font = ("Helvetica", 15), bg= "white")
    output_label.place(relx=0.27, rely=0.14, relwidth= 0.2, relheight = 0.07)
    
    #Output BOX
    output = Text(root, font=("Helvetica", 16), width=50, height= 10, fg ="dark blue", border=3)
    output.place(relx=0.35, rely=0.2, relwidth= 0.6, relheight = 0.6)
    # Protocol needed for the window closing interaction
    root.protocol("WM_DELETE_WINDOW", on_closing_main)
    logo_label.bind("<Configure>",resize_logo)
    
    # Tkinter's main loop 
    root.mainloop()

# This function handles the resizing of the GUI LOGO as the windows gets resized
def resize_logo(e):
    logo2 = Image.open("logo.png")
    resize_logo2 = logo2.resize((e.width, e.height), Image.ANTIALIAS)
    new_logo2 = ImageTk.PhotoImage(resize_logo2)
    logo_label.config(image= new_logo2)
    logo_label.image = new_logo2 # to avoid garbage collection 
    
  
# Change the text widget box COLOR depending in the value!
def box_color_change(self, col_bg):
    self["bg"] = col_bg
    self.insert(END,"")


# This function chnages the valve position color
def valve_position_color(self, col_bg):
    self["bg"] = col_bg
    self.insert(END, "")


# This function overwrites the valve position color to red 
def valve_position_handler(prob):
                # Handling the case where the color of the valve status goes RED 
                # lets give the corresponding list index to the problematic valve
                if prob == 'pvf01':
                        prob = 18
                elif prob == 'pvl01':
                        prob = 19
                elif prob == 'pvp01':
                        prob = 20
                elif prob == 'sovf01':
                        prob = 21
                elif prob == 'pvf02':
                        prob = 22
                elif prob == 'pvl03':
                        prob = 23
                elif prob == 'pvlf01':
                        prob = 24
                elif prob == 'pvl02':
                        prob = 25
                elif prob == 'pvp02':
                       prob = 26
                
                return prob
                    
#This function sends the shutdown command when the Shutdown Button gets clicked.                    
def auto_shutdown():
        if flag_1:
            chan.send("shutdown\n")

############################################################################   
# Closing PID window
############################################################################
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



#############################################################################################
# Opening the P&ID window 
# Tkinter's objects and widgets for the P&ID window
#############################################################################################
def openWindow(m):
    global c1,c2, c3, c4, c5, c6, c7, c8, c9,c_color, col_bg, my_canvas2, newWindow, bg1, box1, box2, box3, box4, box5, box6,\
         box7, box8, box9, box10, box11, box12, box13, box14, box15, box16, box17, box18 
    newWindow = Toplevel(root)
    newWindow.geometry("1400x905")
    newWindow.title("P&ID")
    my_canvas2 = Canvas(newWindow, width=3000, height=1703)   
    my_canvas2.pack()
    
    # Set the P&ID diagram
    bg1 = ImageTk.PhotoImage(file = "pid.png")
    my_canvas2.create_image(0,0, image = bg1, anchor = "nw")

    #default colors for the sensor boxes
    col_bg = "light yellow"
    c_color = "light grey"
    bg_label = "white"
    #PT-F01
    box1 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box1.place(relx=0.17, rely=0.17, relwidth= 0.07, relheight = 0.05)
    label1 = Label(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg )
    label1.place(relx=0.17, rely=0.12, relwidth= 0.07, relheight = 0.05)
    label1 = Label(newWindow,text = "kPa",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label1.place(relx=0.17, rely=0.12, relwidth= 0.07, relheight = 0.05)
    #PT-F02
    box2 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box2.place(relx=0.41, rely=0.17, relwidth= 0.07, relheight = 0.05)
    label2 = Label(newWindow,text = "kPa",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label2.place(relx=0.41, rely=0.12, relwidth= 0.07, relheight = 0.05)
    #PT-F03
    box3 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box3.place(relx=0.494, rely=0.17, relwidth= 0.07, relheight = 0.05)
    label3 = Label(newWindow,text = "kPa",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label3.place(relx=0.494, rely=0.12, relwidth= 0.07, relheight = 0.05)
    #TT-F01
    box4 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box4.place(relx=0.57, rely=0.17, relwidth= 0.06, relheight = 0.05)
    label4 = Label(newWindow,text = "C",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label4.place(relx=0.565, rely=0.12, relwidth= 0.07, relheight = 0.05)
    #PT-F04
    box5 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box5.place(relx=0.66, rely=0.15, relwidth= 0.06, relheight = 0.05)
    label5 = Label(newWindow,text = "kPa",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label5.place(relx=0.655, rely=0.10, relwidth= 0.07, relheight = 0.05)
    #TT-F02
    box6 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box6.place(relx=0.70, rely=0.32, relwidth= 0.06, relheight = 0.05)
    label6 = Label(newWindow,text = "C",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label6.place(relx=0.695, rely=0.37, relwidth= 0.07, relheight = 0.05)
    #PT-M02
    box7 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box7.place(relx=0.90, rely=0.31, relwidth= 0.06, relheight = 0.05)
    label7 = Label(newWindow,text = "kPa",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label7.place(relx=0.895, rely=0.26, relwidth= 0.07, relheight = 0.05)
    #TT-L05
    box8 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box8.place(relx=0.84, rely=0.56, relwidth= 0.06, relheight = 0.05)
    label8 = Label(newWindow,text = "C",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label8.place(relx=0.835, rely=0.62, relwidth= 0.07, relheight = 0.05)
    #TT-L04
    box9 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box9.place(relx=0.65, rely=0.75, relwidth= 0.06, relheight = 0.05)
    label9 = Label(newWindow,text = "C",   font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = bg_label  )
    label9.place(relx=0.645, rely=0.80, relwidth= 0.07, relheight = 0.05)
    #PT-L04
    box10 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box10.place(relx=0.67, rely=0.53, relwidth= 0.06, relheight = 0.05)
    label10 = Label(newWindow,text = "kPa",   font=("Helvetica", 14), width=1, height= 1, fg = "black", bg = bg_label  )
    label10.place(relx=0.735, rely=0.53, relwidth= 0.03, relheight = 0.05)
    #TT-L03
    box11 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box11.place(relx=0.58, rely=0.555, relwidth= 0.06, relheight = 0.05)
    label11 = Label(newWindow,text = "C",   font=("Helvetica", 14), width=1, height= 1, fg = "black", bg = bg_label  )
    label11.place(relx=0.59, rely=0.505, relwidth= 0.03, relheight = 0.05)
    #PT-L03
    box12 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box12.place(relx=0.51, rely=0.5555, relwidth= 0.06, relheight = 0.05)
    label12 = Label(newWindow,text = "kPa",   font=("Helvetica", 14), width=1, height= 1, fg = "black", bg = bg_label  )
    label12.place(relx=0.525, rely=0.505, relwidth= 0.03, relheight = 0.05) 
    #PT-L02
    box13 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box13.place(relx=0.44, rely=0.555, relwidth= 0.06, relheight = 0.05)
    label13 = Label(newWindow,text = "kPa",   font=("Helvetica", 14), width=8, height= 1, fg = "black", bg = bg_label  )
    label13.place(relx=0.455, rely=0.505, relwidth= 0.03, relheight = 0.04)
    #TT-L01
    box14 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box14.place(relx=0.40, rely=0.71, relwidth= 0.06, relheight = 0.05)
    label14 = Label(newWindow,text = "C",   font=("Helvetica", 14), width=1, height= 1, fg = "black", bg = bg_label  )
    label14.place(relx=0.368, rely=0.71, relwidth= 0.03, relheight = 0.05)      
    #TT-L02
    box15 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box15.place(relx=0.40, rely=0.77, relwidth= 0.06, relheight = 0.05)
    label15 = Label(newWindow,text = "C",   font=("Helvetica", 14), width=1, height= 1, fg = "black", bg = bg_label  )
    label15.place(relx=0.415, rely=0.82, relwidth= 0.03, relheight = 0.05)
    #PT-L01
    box16 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box16.place(relx=0.18, rely=0.55, relwidth= 0.06, relheight = 0.05)
    label16 = Label(newWindow,text = "kPa",   font=("Helvetica", 14), width=8, height= 1, fg = "black", bg = bg_label  )
    label16.place(relx=0.19, rely=0.50, relwidth= 0.03, relheight = 0.04)
    #PT-M01
    box17 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box17.place(relx=0.05, rely=0.36, relwidth= 0.06, relheight = 0.05)
    label17 = Label(newWindow,text = "kPa",   font=("Helvetica", 14), width=8, height= 1, fg = "black", bg = bg_label  )
    label17.place(relx=0.05, rely=0.32, relwidth= 0.03, relheight = 0.04)
    #State status
    states = Label(newWindow, text ="Current State",font = ("Helvetica", 14), fg= "black", bg = "white")
    states.place(relx=0.82, rely=0.75, relwidth= 0.1, relheight = 0.06)
    box18 = Text(newWindow, font=("Helvetica", 14), width=8, height= 1, fg = "black", bg = col_bg, border=4 )
    box18.place(relx=0.80, rely=0.8, relwidth= 0.14, relheight = 0.05)
    
    #Valve posistion squares
    #pvf01
    c1 = Text(newWindow, bg = c_color, border = 2)
    c1.place(relx=0.28, rely=0.225, relwidth= 0.01, relheight = 0.015)
    #sov-f01
    c2 = Text(newWindow, bg = c_color, border = 2)
    c2.place(relx=0.42, rely=0.25, relwidth= 0.01, relheight = 0.015)
    #pv-f02
    c3 = Text(newWindow, bg = c_color, border = 2)
    c3.place(relx=0.742, rely=0.22, relwidth= 0.01, relheight = 0.015)
    #pv-p01
    c4 = Text(newWindow, bg = c_color, border = 2)
    c4.place(relx=0.345, rely=0.408, relwidth= 0.01, relheight = 0.015)
    #pv-p02
    c5 = Text(newWindow, bg = c_color, border = 2)
    c5.place(relx=0.395, rely=0.485, relwidth= 0.01, relheight = 0.015)
    #pv-l01
    c6 = Text(newWindow, bg = c_color, border = 2)
    c6.place(relx=0.282, rely=0.596, relwidth= 0.01, relheight = 0.015)
    #pv-l02
    c7 = Text(newWindow, bg = c_color, border = 2)
    c7.place(relx=0.425, rely=0.63, relwidth= 0.01, relheight = 0.015)
    #pv-l03
    c8 = Text(newWindow, bg = c_color, border = 2)
    c8.place(relx=0.755, rely=0.596, relwidth= 0.01, relheight = 0.015)
    #pv-lf01
    c9 = Text(newWindow, bg = c_color, border = 2)
    c9.place(relx=0.38, rely=0.78, relwidth= 0.01, relheight = 0.015)


    if 'opened' in m:
        #flag variable indicating that P&ID window is opening 
        global start_parsing
        start_parsing = 1

    newWindow.protocol("WM_DELETE_WINDOW", on_closing_pid)
    my_canvas2.bind("<Configure>",resizer)
    newWindow.mainloop()

# This function is in charge of resizing the P&ID diagram as we resize the window.
def resizer(e):
    global bg2, resized_bg2, new_bg2
    c_color = "white"
    #open the desired image
    bg2 = Image.open("pid.png")
    resize_bg2 = bg2.resize((e.width, e.height), Image.ANTIALIAS)
    new_bg2 = ImageTk.PhotoImage(resize_bg2)
    my_canvas2.create_image(0,0, image = new_bg2, anchor = "nw")

#######################################################################################
# Help window function 
# Basically openning an image in a Tkinter window
#######################################################################################
def help_win():
    global help_label
    help_window = Toplevel(root)
    help_window.geometry("800x897")
    help_window.title("Help")
    my_canvas3 = Canvas(help_window, width=2000, height=2000)   
    my_canvas3.pack()


    help_menu = ImageTk.PhotoImage(file = path.join(__location__, 'help.png'))
    help_label = Label(help_window, image= help_menu, bg = "white")
    help_label.place(relx=0, rely=0)

    my_canvas3.bind("<Configure>",help_img)
    help_window.mainloop()


#This function handles the resizing of the help window as we resize the window
def help_img(e):
    help_picture = Image.open(path.join(__location__, 'help.png'))
    resize_picture = help_picture.resize((e.width, e.height), Image.ANTIALIAS)
    new_picture = ImageTk.PhotoImage(resize_picture)
    help_label.config(image= new_picture)
    help_label.image = new_picture # to avoid garbage collection 
    
#################################################################################
# This function handles the user commands
# *******************************************************************************
#################################################################################
def user_command_event(command):
    # flag variable to signal when to clear the Output Box
    flag_to_clear = False
    if command == 'clear':
        #command to Clear the output section 
        output.delete(1.0, END)
        entry.delete(0, END)
        flag_to_clear = True

    #Command to exit multibb.py  
    if command == 'exit':
        chan.send("exit\n")
    
    if not flag_to_clear: 
        #Sending all the user commands
        chan.send(str(command)+'\n')
        entry.delete(0, END)


##############################################################################
# This sections handles the setup and initialization of PARAMIKO
#
##############################################################################

def init_paramiko():
    # needed parameters for connection    
    port = 22
    hostname = 'beaglebone'
    username = 'debian'
    password = 'Capstone2020'
    #For testing with usb 
    #hostname = '192.168.7.2'
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


        # Creating a channel to start the communication 
        chan = client.invoke_shell()
        print(repr(client.get_transport()))
        output.insert("end-1c",'*** SSH Connection to BB_AI stablished ***\n\n')
        global flag_1
        flag_1 = True # This flag is created with the objective that Automatic Shutdown button can only 
        # work if an SSH connection is actually stablished
        #chan.flush()
        chan.send('python3 multibb.py\n')
        
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
        # Making sure we are in Linux
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
                            x = u(chan.recv(1024).decode('utf8'))
                            # Log the data
                            file.write(str(x)+'\n')
                            if len(x) == 0:
                                sys.stdout.write("\r\n*** Done ***\r\n")
                                file.close()
                                chan.close()
                                client.close()
                                break

                            ################################################################################## 
                            # Parsing the data 
                            # Whenever we get data that we dont want to handle we can skip the whole procedure
                            ##################################################################################
                            if str(x) == '':
                                continue

                            if '\x1b[K' in str(x):
                                x = x.replace('\x1b[K', '').replace('\r\x1b[1A', '')
                            # Check for the ERROR message for a problematic valve
                            # The idea is to know which valve is the one in error so that we can print a 2 for the valve position value
                            if 'Error:' in str(x):
                                    index3 = x.find('Valve ')
                                    # stract the problematic valve name
                                    if not index3 == -1:
                                        
                                        problematic_valve = x[index3:index3+6].replace(' ', '')
                                        #Passing the name to a function that will return the list index so that we can modify it with a 2 
                                        problematic_valve = valve_position_handler(problematic_valve)
                                        
                                        global flag_valve_red
                                        flag_valve_red = True  
                            
                            
                            #Make sure we always have only one x packet 
                            if 'Sensor' in x:
                                x = x.replace('\r\n', '').replace('\n', '')
                                spl = x.split('~')

                                #splitting the sensor values and adding the to a FIFO Queue
                                for index in range(len(spl)):
                                    if len(spl[index]) > 0:
                                        #Only when the x packet is equal or bigger than 150 put it in the queue
                                        # Doing this will help to control what items go into the queue 
                                        # This Queue helps to store the x packets
                                        index4 = spl[index].find(':')
                                        if not index4 == -1:
                                                #Slice the Sensor Values Part
                                                temp = str(spl[index])
                                                #For testing until the marionette software gets completed 
                                                #Lets add the valve position values 
                                                temp2 = temp[index4+2:] + ' 0 1 2 1 1 2 1 0 0'


                                                number_of_values = str(temp2).split(' ')
                                                #if we have a data packet with more than 39 items then we consider it garbage
                                                if len(number_of_values) > 40:
                                                    continue

                                                ##########################################################
                                                # For testing purposes only
                                                if not len(number_of_values) == 40:
                                                    while len(number_of_values) <= 39:
                                                        number_of_values.insert(30,'0')
                                                ###########################################################

                                                # map Sensor values into floats
                                                Sensor_val = number_of_values[0:17]
                                                mapping = map(float, Sensor_val)
                                                Sensor_val = list(mapping)

                                                # map the Valve_positions into ints
                                                Valve_Positions = number_of_values[30:39]
                                                mapping2 =map(int, Valve_Positions)
                                                Valve_Positions = list(mapping2)

                                                #Join both to restructure the data packet
                                                number_of_values2 = Sensor_val + Valve_Positions
                                                # Convert to string before storing in the QUEUE
                                                #number_of_values3 = ' '.join(map(str,number_of_values2))
                                                
                                        # 26 values between 17 sensor values and 9 valve positions       
                                        if len(number_of_values2) >= 26:
                                            #If we got here then we have shape the data packet to store it in the FIFO Queue
                                            q.put(number_of_values2)
                                            #Print the Data Packets that we are putting in the Queue to display in the P&ID window
                                            #print(str(number_of_values2)+'\n')
                            #for testing
                            #print(data, end= '\r', flush= True)



                            #strip non useful info
                            #strip non useful info with a more specific approach is better 
                            if 'The programs' in str(x) or 'GNU/Linux' in str(x) \
                                or 'exact distribution' in str(x) or '@beaglebone' in str(x) \
                                or 'python3' in str(x) or 'bb.py' in str(x):
                                length = len(x)
                                x = x[length:]

                            if 'PORTLAND' in str(x):
                                global sig
                                sig = True
                            
                            # Once done slicing the greeting section lest get State status 
                            #global sig
                            global count
                            if sig:
                                if count >= 1:
                                    chan.send('status\n')
                                    #global sig
                                    sig = False
                                #global count
                                count += 1
                            
                            if 'State :' in str(x):
                                global state_preview
                                # Lets get only the State for the preview window
                                index = x.find(':')
                                if not index == -1:
                                    state_preview = str(x[index+1:])
                                    #for testing
                                    #print(state_preview)
                                else: 
                                    #state_preview = str(data[8:]).replace('[K', '').replace('\x1b', ' ')
                                    pass   

                            # If the P&ID window is Open start parsing the data packet and display it 
                            if start_parsing:
                                box18.delete(1.0, END)
                                box18.insert("1.0", state_preview)

                                # Parse the sensor values 
                                if 'Sensor' in str(x):
                                    #change the box color to red to indicate warning
                                    # STart getting the formatted data packets from the QUEUE
                                    SV_from_q = q.get()
                                    q.task_done()

                                    # For troubleshooting purposes lets see what we are getting from the QUEUE
                                    print(str(SV_from_q)+'\n')

                                    # modify s_val list with a 2 for the corresponding problematic valve
                                    if flag_valve_red:
                                        SV_from_q[problematic_valve] = 2
                                        #Turn the flag off so that we only chnage the valve color to red when
                                        # an error message is received
                                        flag_valve_red = False
                                    # The obj_list is created to pass the objects into the function that chnages the color parameter.
                                    # boxes are the sensor boxes and c1...c9 are the valve position square indicators.
                                    obj_list = [box1, box2, box3, box4, box5, box6, box7, box8, box9, box10, box11, box12, box13, box14, box15, box16, \
                                                box17, c1, c2, c3, c4, c5, c6, c7, c8, c9]
                                    
                                    #######################################################
                                    # Handle the Sensor value box COLOR CHANGE
                                    ######################################################
                                    for index in range(len(SV_from_q)):

                                        for index in range(0, 17):
                                            if SV_from_q[index] > 1:
                                                #change the box color when outside valid range
                                                box_color_change(obj_list[index], "#fb9898" )
                                            # if the sensor value is withing the spected range then keep box color yellow
                                            obj_list[index].delete(1.0, END)
                                            obj_list[index].insert('1.0', SV_from_q[index])
                                        
                                        for index in range(17,26):
                                            # if the valve is turned ON then light up GREEN 
                                            if SV_from_q[index] == 1:
                                                valve_position_color(obj_list[index],"#58df52")
                                            # if the valve is turned OFF then light up BLUE
                                            elif SV_from_q[index] == 0:
                                                valve_position_color(obj_list[index], "blue")
                                            # if there is an error with the valve light up RED
                                            elif SV_from_q[index] == 2:
                                                valve_position_color(obj_list[index], "red")
                                        #format the data back to distinguis from the screen we want to print to 
                                        #data = f'Sensor values: {data}'

                            if str(x) == '\r\n':
                                #lets replace it
                                x = x.replace('\r\n', '')
                            
                            if 'Sensor' in str(x): 
                                continue
                            if  str(data) == 'status' or str(data) == 'loxprefill' or str(data) == 'loxfill' or \
                                str(data) == 'loxchill' or str(data) == 'safety' or str(data) == 'valvecheck'or \
                                str(data) == 'cyclevalves' or str(data) == 'pressurize' or str(data) == 'ignition' or\
                                str(data) == 'resetac' or str(data) == 'resetm' or str(data) == 'shutdown'   :
                                        if not '\r\n' in data:
                                            data = str(data)+'\r\n' 
                                            #print("Got here\n")
                            
                            #Display to the output BOX   
                            output.insert("end-1c",str(x))
                            output.see("end")
                 
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
         
    except Exception as e:
        print("*** Caught exception: %s: %s" % (e.__class__, e))
        traceback.print_exc()
        if '[Errno 11001] getaddrinfo failed' == str(e) :

            output.insert("end-1c",f' Error connecting to BB_AI.\n')
        elif '[WinError 10060]' in str(e):
            output.insert("end-1c",f' Error connecting to BB_AI.\n')
        

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




