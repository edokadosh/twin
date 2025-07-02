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
            local_filename = command[1]
            target_filename = command[2]
            try:
                with open(local_filename, 'rb') as file:
                    file_data = file.read()
                print(f"Uploading file '{local_filename}' as '{target_filename}'...")
                print(f"File size: {len(file_data)} bytes")
                client_socket.sendall(f"UPLOAD {target_filename} {len(file_data)}".encode())
                response = client_socket.recv(1024).decode()
                if response == "READY":
                    client_socket.sendall(file_data)
                    print(f"File '{local_filename}' uploaded successfully.")
                else:
                    print("Server did not acknowledge upload request.")
            except FileNotFoundError:
                print(f"File '{local_filename}' not found.")
        elif command[0] == 'DOWNLOAD':
            local_filename = command[1]
            target_filename = command[2]
            client_socket.sendall(f"DOWNLOAD {target_filename}".encode())
            response = client_socket.recv(1024).decode()
            if response.startswith("FILE"):
                file_size = int(response.split()[1])
                client_socket.sendall(b"READY")
                with open(local_filename, 'wb') as file:
                    bytes_received = 0
                    while bytes_received < file_size:
                        data = client_socket.recv(1024)
                        if not data:
                            break
                        file.write(data)
                        bytes_received += len(data)
                print(f"File '{target_filename}' downloaded successfully as '{local_filename}'.")
            else:
                print(f"Failed to download file '{target_filename}': {response}")
        else:
            client_socket.sendall(message.encode())
        response = client_socket.recv(1024).decode()
        print(f"Received: {response}")



if __name__ == "__main__":
	main()
     
     