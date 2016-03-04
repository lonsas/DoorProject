import socket
import sys
import logging
from subprocess import call

logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind the socket to the address given on the command line
server_name = sys.argv[1]
server_address = (server_name, 8000)
logging.info('starting up on %s port %s' % server_address)
sock.bind(server_address)
sock.listen(1)


while True:
    logging.info('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        logging.info('client connected: %s, %s' % client_address)

        data = connection.recv(8)
        logging.info('received "%s"' % data)
        if data:
            if data == 'DOOROPEN':
                call(['playerctl', 'play-pause'])
            connection.sendall(data)
    finally:
        connection.close()
        logging.info('client disconnected: %s, %s' % client_address)
sock.close()