import socket
import errno
import sys

HOST = "127.0.0.1"
PORT = 1234
HEADERLENGTH = 10

def main():
    userName = input("what is your username? ")

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
        client.connect((HOST, PORT))
        client.setblocking(False)

        client.send(f"{len(userName):<{HEADERLENGTH}{userName}}".encode())

        while True:
            recvHeader = int(client.recv(10).decode().strip)
            print(f"{messageLen:<{HEADERLENGTH}}{message}")
            # message = input("Enter a message: ")
            # if message:
            #     print(f"DEBUG: sending: {message}")
            #     messageLen = len(message)
            #     print(f"DEBUG: message Header: {messageLen:<{HEADERLENGTH}}{message}")
            #     client.sendall(f"{messageLen:<{HEADERLENGTH}}{message}".encode())

            try:
                while True:
                    recvMessage = client.recv(1024).decode()
                    print(f"Receved: {recvMessage}")
            except IOError as e:
                # if server has terminated connection
                if e.errno != errno.EAGAIN and e.errno != errno.EWOULDBLOCK:
                    print(f"reading error: {str(e)}")
                    sys.exit()
                continue

if __name__ == "__main__":
    main()
