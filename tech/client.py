import socket

SERVER_ADDR = ('localhost', 12345)

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(SERVER_ADDR)
    
    while True:
        message = input("Enter message to send (or 'exit' to quit): ")
        command = message.split()
        if command[0] == 'exit':
            break
        elif command[0] == 'UPLOAD':
            filename = command[1]
            target_filename = command[2]
            try:
                with open(filename, 'rb') as file:
                    file_data = file.read()
                client_socket.sendall(f"UPLOAD {target_filename}".encode())
                response = client_socket.recv(1024).decode()
                if response == "READY":
                    client_socket.sendall(file_data)
                    print(f"File '{filename}' uploaded successfully.")
                else:
                    print("Server did not acknowledge upload request.")
            except FileNotFoundError:
                print(f"File '{filename}' not found.")
        else:
            client_socket.sendall(message.encode())
        response = client_socket.recv(1024).decode()
        print(f"Received: {response}")



if __name__ == "__main__":
	main()
     
     