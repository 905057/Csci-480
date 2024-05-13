import socket
import errno
import sys
import select

HOST = "127.0.0.1"
PORT = 1234
HEADERLENGTH = 10

def main():
    username = input("what is you username?")

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
        client.connect((HOST, PORT))
        client.setblocking(False)

        client.send(f"{len(username):<{HEADERLENGTH}}{username}".encode())

        while True:
            message = input("Enter a message: ")
            if message:
                print(f"DEBUG: sending: {message}")
                messageLen = len(message)
                print(f"DEBUG: message Header: {messageLen:<{HEADERLENGTH}}{message}")
                client.sendall(f"{messageLen:<{HEADERLENGTH}}{message}".encode())

            try:
                while True:
                    userHeader = int(client.recv(10).decode().strip())
                    username = client.recv(userHeader).decode()
                    recvHeader = int(client.recv(10).decode().strip())
                    # print(f"message size: {recvHeader}")
                    recvMessage = client.recv(recvHeader).decode()
                    print(f"{username}: {recvMessage}")

            except IOError as e:
                # if server has terminated connection
                if e.errno != errno.EAGAIN and e.errno != errno.EWOULDBLOCK:
                    print(f"reading error: {str(e)}")
                    sys.exit()
                continue

if __name__ == "__main__":
    main()
