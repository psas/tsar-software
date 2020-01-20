#!/usr/bin/env python3

import socket, sys, time
from ftfutils import Mode, log

host = 'localhost'
port = 8081

log(Mode.INFO, "Creating the socket: " + str(host) + ":" + str(port))

try:    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error:
    log(Mode.ERROR, "Failed to create socket!")
    sys.exit(-1)

log(Mode.INFO, "Getting remote IP...")
try: remote_ip = socket.gethostbyname(host)
except socket.gaierror:
    log(Mode.ERROR, "Failed to find a server!")
    sys.exit(-1)

log(Mode.INFO, "Connecting to server: " + str(remote_ip) + ":" + str(port))
sock.connect((remote_ip, port))

while True:
    request = "GET / HTTP/1.0\r\n\r\n"
    log(Mode.INFO, "Sending request: " + request)

    try: sock.sendall(bytes(request, "UTF-8"))
    except socket.error:
        log(Mode.INFO, "Failed to send request!")
        # sys.exit(-1)

    response = sock.recv(4096)
    log(Mode.INFO, "Recieved response: " + str(response))

    time.sleep(0.01)
