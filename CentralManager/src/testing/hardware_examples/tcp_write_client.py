#!/usr/bin/env python3

import socket
import json

TCP_IP = '192.168.0.14'
TCP_PORT = 8080
BUFFER_SIZE = 1024
MESSAGE = "Hello, World!"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

while(True):
    s.send(b'hello')
    print("START MESSAGE SENT")

s.close()

