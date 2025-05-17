import socket

ESP32_IP = "192.168.52.211"  # Sostituisci con l'IP reale dell'ESP32
PORT = 8888

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.settimeout(5)
    s.connect((ESP32_IP, PORT))
    s.sendall(b"Ciao ESP32!\n")
    data = s.recv(1024)