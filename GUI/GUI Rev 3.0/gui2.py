
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

start_parsing = 0
state_preview = ''
ok_to_send = False
sig = False
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
    #Add button 
    '''
    button1 = Button(root, text="Logged Data", command=lambda: openfile())
    button1.place(relx=0.736, rely=0.127, relwidth= 0.12, relheight = 0.045)
    '''
    
    logo = ImageTk.PhotoImage(file = "logo2.png")
    logo_label = Label(root, image= logo, bg = "white")
    logo_label.place(relx=0.047, rely=0.75, relwidth= 0.1, relheight = 0.17)
    #my_canvas.create_image(50,520, image=logo, anchor= "sw" )  


    button2 = Button(root, text="Help", command =lambda: help_win())
    button2.place(relx=0.867, rely=0.127, relwidth= 0.05, relheight = 0.045)
    button3 = Button(root, text="P&ID", command=lambda m='opened': openWindow(m))
    button3.place(relx=0.82, rely=0.127, relwidth= 0.04, relheight = 0.045)
    button4 = Button(root, text = "Shutdown",border = "2", font=("Helvetica", 16, "italic"),fg = "white", bg = "#d43a3a",command = lambda: auto_shutdown())
    button4.place(relx=0.06, rely=0.4, relwidth= 0.2, relheight = 0.2)
    #button3.place(relx=0.686, rely=0.127, relwidth= 0.04, relheight = 0.045)

    #Add label for cmd box
    command = Label(text ="Enter a command:",font = ("Helvetica", 15), bg= "white")
    command.place(relx=0.06, rely=0.14, relwidth= 0.2, relheight = 0.07)

    #Command entry box
    entry = Entry(root, font=("Helvetica", 16),fg ="dark blue", border=3)#, command = user_command)
    entry.place(relx=0.06, rely=0.2, relwidth= 0.2, relheight = 0.07)
    entry.bind("<Return>", (lambda event:user_command_event(entry.get())))
    #Add label for the output window
    output_label = Label(text ="Output:",font = ("Helvetica", 15), bg= "white")
    output_label.place(relx=0.27, rely=0.14, relwidth= 0.2, relheight = 0.07)

    output = Text(root, font=("Helvetica", 16), width=50, height= 10, fg ="dark blue", border=3)
    output.place(relx=0.35, rely=0.2, relwidth= 0.6, relheight = 0.6)
    
    root.protocol("WM_DELETE_WINDOW", on_closing_main)
    logo_label.bind("<Configure>",resize_logo)
    
    root.mainloop()

def resize_logo(e):
    logo2 = Image.open("logo2.png")
    resize_logo2 = logo2.resize((e.width, e.height), Image.ANTIALIAS)
    new_logo2 = ImageTk.PhotoImage(resize_logo2)
    logo_label.config(image= new_logo2)
    logo_label.image = new_logo2 # to avoid garbage collection 
    
  
#change the text widget box depending in the value 
def box_color_change(self, col_bg):
    self["bg"] = col_bg
    self.insert(END,"")

def valve_position_color(self, col_bg):
    self["bg"] = col_bg
    self.insert(END, "")
'''
def help_window():
   def h_w(): 
     os.system('help.png')
   help_t = threading.Thread(target=h_w)
   help_t.start()
'''
def auto_shutdown():
        if flag_1:
            chan.send("stop\n")
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
    global c1,c2, c3, c4, c5, c6, c7, c8, c9,c_color, col_bg, my_canvas2, newWindow, bg1, box1, box2, box3, box4, box5, box6,\
         box7, box8, box9, box10, box11, box12, box13, box14, box15, box16, box17, box18 
    newWindow = Toplevel(root)
    newWindow.geometry("1400x905")
    newWindow.title("P&ID")
    my_canvas2 = Canvas(newWindow, width=3000, height=1703)   
    my_canvas2.pack()

    bg1 = ImageTk.PhotoImage(file = "pid04.png")
    my_canvas2.create_image(0,0, image = bg1, anchor = "nw")

    #default colors for the sensor boxes
    col_bg = "light yellow"
    c_color = "light grey"
    #PT-F01
    box1 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box1.place(relx=0.17, rely=0.17, relwidth= 0.07, relheight = 0.05)
    #PT-F02
    box2 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box2.place(relx=0.41, rely=0.17, relwidth= 0.07, relheight = 0.05)
    #PT-F03
    box3 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box3.place(relx=0.494, rely=0.17, relwidth= 0.07, relheight = 0.05)
    #TT-F01
    box4 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box4.place(relx=0.57, rely=0.17, relwidth= 0.06, relheight = 0.05)
    #PT-F04
    box5 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box5.place(relx=0.66, rely=0.15, relwidth= 0.06, relheight = 0.05)
    #TT-F02
    box6 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box6.place(relx=0.70, rely=0.32, relwidth= 0.06, relheight = 0.05)
    #PT-M02
    box7 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box7.place(relx=0.90, rely=0.31, relwidth= 0.06, relheight = 0.05)
    #TT-L05
    box8 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box8.place(relx=0.84, rely=0.56, relwidth= 0.06, relheight = 0.05)
    #TT-L04
    box9 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box9.place(relx=0.65, rely=0.75, relwidth= 0.06, relheight = 0.05)
    #PT-L04
    box10 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box10.place(relx=0.67, rely=0.53, relwidth= 0.06, relheight = 0.05)
    #TT-L03
    box11 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box11.place(relx=0.58, rely=0.555, relwidth= 0.06, relheight = 0.05)
    #PT-L03
    box12 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box12.place(relx=0.51, rely=0.5555, relwidth= 0.06, relheight = 0.05)
    #PT-L02
    box13 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box13.place(relx=0.44, rely=0.555, relwidth= 0.06, relheight = 0.05)
    #TT-L01
    box14 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box14.place(relx=0.40, rely=0.71, relwidth= 0.06, relheight = 0.05)
    #TT-L02
    box15 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box15.place(relx=0.40, rely=0.77, relwidth= 0.06, relheight = 0.05)
    #PT-L01
    box16 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box16.place(relx=0.18, rely=0.55, relwidth= 0.06, relheight = 0.05)
    #PT-M01
    box17 = Text(newWindow, font=("Helvetica", 16), width=8, height= 1, fg = "black", bg = col_bg, border=4 )  #fg ="dark blue", border=4, bg = "light yellow")
    box17.place(relx=0.05, rely=0.36, relwidth= 0.06, relheight = 0.05)
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
        #flag variable
        global start_parsing
        start_parsing = 1

    newWindow.protocol("WM_DELETE_WINDOW", on_closing_pid)
    my_canvas2.bind("<Configure>",resizer)
    newWindow.mainloop()

def resizer(e):
    global bg2, resized_bg2, new_bg2
    c_color = "white"
    #open the desired image
    bg2 = Image.open("pid04.png")
    resize_bg2 = bg2.resize((e.width, e.height), Image.ANTIALIAS)
    new_bg2 = ImageTk.PhotoImage(resize_bg2)
    my_canvas2.create_image(0,0, image = new_bg2, anchor = "nw")

def help_win():
    global help_label
    help_window = Toplevel(root)
    help_window.geometry("700x807")
    help_window.title("Help")
    my_canvas3 = Canvas(help_window, width=2000, height=2000)   
    my_canvas3.pack()


    help_menu = ImageTk.PhotoImage(file = path.join(__location__, 'help.png'))
    help_label = Label(help_window, image= help_menu, bg = "white")
    help_label.place(relx=0, rely=0)

    my_canvas3.bind("<Configure>",help_img)
    help_window.mainloop()

def help_img(e):
    help_picture = Image.open(path.join(__location__, 'help.png'))
    resize_picture = help_picture.resize((e.width, e.height), Image.ANTIALIAS)
    new_picture = ImageTk.PhotoImage(resize_picture)
    help_label.config(image= new_picture)
    help_label.image = new_picture # to avoid garbage collection 
#################################################################################
def user_command_event(command):
    flag_to_clear = False
    if command == 'clear':
        #command to Clear the output section 
        output.delete(1.0, END)
        entry.delete(0, END)
        flag_to_clear = True
        
    if command == 'exit':
        chan.send("exit\n")

    if not flag_to_clear: 
        chan.send(str(command)+'\n')
        entry.delete(0, END)
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

        chan = client.invoke_shell()
        print(repr(client.get_transport()))
        output.insert("end-1c",'*** SSH Connection to BB_AI stablished ***\n\n')
        global flag_1
        flag_1 = True # This flag is created with the objective that Automatic Shutdown button can only 
        # work if an SSH connection is actually stablished
        #chan.flush()
        chan.send('python3 multiprocessingbb.py\n')
        
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
                            x = u(chan.recv(1024).decode('utf8'))
                            if len(x) == 0:
                                sys.stdout.write("\r\n*** Done ***\r\n")
                                chan.close()
                                client.close()
                                break

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
                            
                            if 'State :' in str(x) or 'tate :' in str(x):
                                global state_preview
                                state_preview = x[8:].replace('[K', '')    


                            if start_parsing:
                                box18.delete(1.0, END)
                                box18.insert("1.0", state_preview)
                                # Parse the sensor values 
                                if 'Sensor' in str(x):
                                    #change the box color to red to indicate warning
                                    #for testing 
                                    #x = x.split(' ')
                                    x = x + ' 0 1 2 1 1 2 1 0 0'

                                    s_val = x.replace('\x1b[F\r', '').split(' ')
  
                                    #for testing only (for the case where we get less than 30 sensor values)
                                    if not len(s_val) == 41:
                                        while len(s_val) < 41:
                                            s_val.insert(32,'0')


                                    s_val = s_val[2:19] + s_val[32:41]
                                    s_val = map(float, s_val)
                                    s_val = list(s_val)

                                    
                                    
                                    obj_list = [box1, box2, box3, box4, box5, box6, box7, box8, box9, box10, box11, box12, box13, box14, box15, box16, \
                                                box17, c1, c2, c3, c4, c5, c6, c7, c8, c9]
                                    lenq = len(s_val)
                                    for index in range(len(s_val)):

                                        for index in range(0, 17):
                                            if s_val[index] > 1:
                                                #change the box color when outside valid range
                                                box_color_change(obj_list[index], "#fb9898" )
                                            # if the sensor value is withing the spected range then keep box color yellow
                                            obj_list[index].delete(1.0, END)
                                            obj_list[index].insert('1.0', s_val[index])
                                        
                                        for index in range(17,26):
                                            # if the valve is turned ON then light up GREEN 
                                            if s_val[index] == 1:
                                                valve_position_color(obj_list[index],"#58df52")
                                            # if the valve is turned OFF then light up BLUE
                                            elif s_val[index] == 0:
                                                valve_position_color(obj_list[index], "blue")
                                            # if there is an error with the valve light up RED
                                            elif s_val[index] == 2:
                                                valve_position_color(obj_list[index], "red")

    
                            if 'Sensor' in str(x): 
                                continue
                            x = x.replace('[K', '')     
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
        #############################################################################
        # if we are in a windows.
        #############################################################################
        else:
            #global count
            
            def writeall(sock):
                global sig_to_resend
                while True:
                    data = ' '
                    data = sock.recv(9999).decode('utf8')
                    file.write(data+'\n')
                    if not data or not ON:
                        sys.stdout.write("\r\n*** Done ***\r\n\r\n")
                        sys.stdout.flush()
                        file.close()
                        chan.close()
                        client.close()
                        break


                    #Make sure we always have only one data packet 
                    if 'Sensor' in data:
                       
                        spl = data.split('\r')

                        #splitting the sensor values and adding the to a FIFO Queue
                        for index in range(len(spl)):
                            if len(spl[index]) > 0:
                                q.put(spl[index])

                                
                            

                    #for testing
                    #print(data, end= '\r', flush= True)


                    #strip non useful info with a more specific approach is better 
                    if 'The programs' in str(data) or 'GNU/Linux' in str(data) \
                        or 'exact distribution' in str(data) or '@beaglebone' in str(data) \
                        or 'python3' in str(data) or 'bb.py' in str(data):
                        length = len(data)
                        data = data[length:]

                    if 'PORTLAND' in str(data):
                        global sig
                        sig = True
                    
                    # Once done slicing the greeting section lest get State status 
                    #global sig
                    global count
                    
                    if sig:
                        #wait till next time around 
                        if count >= 1:
                            chan.send('status\n')
                            #global sig
                            sig = False
                            #sig_to_resend = True
                        #global count
                        count += 1
                    #for testing    
                    #print(data)
                    

                    if 'State :' in str(data) or 'tate :' in str(data):
                        global state_preview
                        index = data.find(':')
                        if not index == -1:
                            state_preview = str(data[index+1:]).replace('[K', '').replace('\x1b', ' ')
                            #for testing
                            print(state_preview)
                        else: 
                            #state_preview = str(data[8:]).replace('[K', '').replace('\x1b', ' ')
                            pass 
                    
                    if start_parsing:
                        #Always display the current state in the P&ID window
                        box18.delete(1.0, END)
                        box18.insert("1.0", state_preview)
                       

                        # Parse the sensor values 
                        if 'Sensor values:' in str(data):
                            # for testing (prints sensor values )
                            #print(data, end= '\r', flush= True)
                                                                #change the box color to red to indicate warning
                                    #for testing 
                                    #x = x.split(' ')
                                    data = q.get()
                                    index2 = data.find(':')
                                    if not index == -1:
                                        data = data[index2+2:].replace('\x1b[F', '')
                                        data = data + ' 0 1 2 1 1 2 1 0 0'

                                    s_val = data.split(' ')
  
                                    #for testing only (for the case where we get less than 30 sensor values)
                                    if not len(s_val) == 39:
                                        while len(s_val) < 39:
                                            s_val.insert(30,'0')


                                    s_val = s_val[0:17] + s_val[30:39]
                                    mapping = map(float, s_val)
                                    s_val = list(mapping)

                                    
                                    
                                    obj_list = [box1, box2, box3, box4, box5, box6, box7, box8, box9, box10, box11, box12, box13, box14, box15, box16, \
                                                box17, c1, c2, c3, c4, c5, c6, c7, c8, c9]
                                    lenq = len(s_val)
                                    for index in range(len(s_val)):

                                        for index in range(0, 17):
                                            if s_val[index] > 1:
                                                #change the box color when outside valid range
                                                box_color_change(obj_list[index], "#fb9898" )
                                            # if the sensor value is withing the spected range then keep box color yellow
                                            obj_list[index].delete(1.0, END)
                                            obj_list[index].insert('1.0', s_val[index])
                                        
                                        for index in range(17,26):
                                            # if the valve is turned ON then light up GREEN 
                                            if s_val[index] == 1:
                                                valve_position_color(obj_list[index],"#58df52")
                                            # if the valve is turned OFF then light up BLUE
                                            elif s_val[index] == 0:
                                                valve_position_color(obj_list[index], "blue")
                                            # if there is an error with the valve light up RED
                                            elif s_val[index] == 2:
                                                valve_position_color(obj_list[index], "red")
                                        #format the data back to distinguis from the screen we want to print to 
                                        data = f'Sensor values: {data}'


                    if 'Sensor' in str(data): 
                        continue
                   
                    output.insert("end-1c",str(data).replace('\x1b[K', ' ') )
                    output.see("end")
                   
                if not ON:
                    return "Exiting GUI"

            writer = threading.Thread(target=writeall, args=(chan,))
            #writer.daemon = True
            writer.start()
         
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




