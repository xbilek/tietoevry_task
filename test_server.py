import socket
import random
import string
import sys
import threading
import os
import tty
import termios

HOST = "127.0.0.1"
PORT = 10200
BUFFER_SIZE = 20

def generate_random_string(size):
    return ''.join(random.choices(string.printable, k=size))

def listen_keyboard():
    while True:
        key = sys.stdin.read(1)
        if key == ' ':
            data = generate_random_string(BUFFER_SIZE).encode()
            client.sendall(data)
        elif key == '\x1b': # ESC
            print("Exiting server...")
            os._exit(0)#exits whole program, not just a thread


if __name__ == "__main__":
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen()
    print("Server is running...")
    
    tty.setcbreak(sys.stdin.fileno()) #setting keyboard input to work immediately, without pressing enter


    keyboard_thread = threading.Thread(target=listen_keyboard) #starting thread waiting for key press
    keyboard_thread.daemon = True
    keyboard_thread.start()

    
    while True: #server running loop
        conn, addr = server_socket.accept()
        client = conn

