import socket

SERVER_ADDR = ('localhost', 12345)

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client_socket.connect(SERVER_ADDR)
	client_socket.send('ping')


if __name__ == "__main__":
	main()