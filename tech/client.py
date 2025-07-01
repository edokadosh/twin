import socket

SERVER_ADDR = ('localhost', 12345)

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(SERVER_ADDR)
    
    while True:
        message = input("Enter message to send (or 'exit' to quit): ")
        if message.lower() == 'exit':
            break
        client_socket.sendall(message.encode())
        response = client_socket.recv(1024).decode()
        print(f"Received: {response}")



if __name__ == "__main__":
    main()