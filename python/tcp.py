import socket 
import threading
import struct
import led
import numpy as np

bind_ip = "0.0.0.0" 
bind_port = 9999
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
server.bind((bind_ip, bind_port))
server.listen(5)

RIGHE = 360
COLONNE = 5
NUM_ELEMENTI = RIGHE * COLONNE
DATA_TYPE = 'B' # B = unsigned char (uint8_t), 1 byte
STRUCT_FORMAT = f'<{NUM_ELEMENTI}{DATA_TYPE}'
ARRAY_SIZE_BYTES = struct.calcsize(STRUCT_FORMAT)

outArray = []

def handle_client(client_socket): 
    #printing what the client sends 
    request = client_socket.recv(1024) 
    print(f"[+] Request: {request}")

    outArray = led.generate_image()
    print(len(outArray))

    packed_data = struct.pack(STRUCT_FORMAT, *outArray)

    client_socket.sendall(packed_data)
    
    print(f"[*] Inviato l'array")
    client_socket.close()

while True:
    client, addr = server.accept()
    print(f"[+] Accepted connection from: {addr[0]}:{addr[1]}")
    #spin up our client thread to handle the incoming data 
    client_handler = threading.Thread(target=handle_client, args=(client,))
    client_handler.start()