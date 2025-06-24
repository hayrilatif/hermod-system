import socket
import threading
import time
from queue import Queue

class TCPServer:
    def __init__(self, port):
        self.port = port
        self.server_socket = None
        self.client_socket = None
        self.ping_pong_wait_sec = 1  # 1000 ms
        self.message_queue = Queue()

    def start(self):
        try:
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server_socket.bind(('', self.port))
            self.server_socket.listen(1)
            print(f"Sunucu calisiyor (TCP): {self.port}")
            return True
        except Exception as e:
            print(f"Soket olusturma veya baglama hatasi: {e}")
            return False

    def accept_client(self):
        try:
            self.client_socket, addr = self.server_socket.accept()
            print(f"Client baglandi: {addr}")
            self.ping_pong_loop()
            return True
        except Exception as e:
            print(f"Client kabul hatasi: {e}")
            return False

    def receive_data(self):
        try:
            data = self.client_socket.recv(1024)
            if not data:
                print("Bağlantı istemci tarafından sonlandırıldı.")
                self.client_socket.close()
                return "<|closed|>"
            return data.decode()
        except Exception as e:
            print(f"Ağ problemi! {e}")
            self.client_socket.close()
            return "<|error|>"

    def send_data(self, data):
        try:
            self.client_socket.sendall(data.encode())
        except Exception as e:
            print(f"Gönderim hatası: {e}")

    def close_server(self):
        if self.client_socket:
            self.client_socket.close()
        if self.server_socket:
            self.server_socket.close()

    def ping_pong_loop(self):
        is_error = False
        while not is_error:
            time.sleep(self.ping_pong_wait_sec)

            self.send_data("PING")
            received = self.receive_data()

            if received in ["<|closed|>", "<|error|>"]:
                print(received)
                break

            print(received)

    def enqueue_message(self, message):
        self.message_queue.put(message)
