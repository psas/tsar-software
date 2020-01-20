#!/usr/bin/env python3

import socketserver
from ftfutils import Mode, log

ADDR = "localhost"
PORT = 8081
DEFAULT_CONNECTION = (ADDR, PORT)

class Handler(socketserver.StreamRequestHandler):
    def handle(self):
        self.data = str(self.rfile.readline().strip())
        log(Mode.INFO, "[" + self.client_address[0] + "]: " + str(self.data))
        self.wfile.write(bytes(self.data.upper()), "UTF-8")

def main():
    with socketserver.TCPServer(DEFAULT_CONNECTION, Handler) as server:
        server.serve_forever()

if __name__ == "__main__": main()
