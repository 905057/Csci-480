import socket

HOST = "127.0.0.1"
PORT = 1234

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.connect((HOST, PORT))

    while True:
        inStr = input("What do you want to send? ")

        # byteSize = len(inStr.encode())
        # sentBytes = 0
        # while sentBytes < byteSize:
        #     tempBytes = sock.send(inStr.encode())
        #     sentBytes += tempBytes

        # same as above
        sock.send(inStr.encode())

        recvBuffer = sock.recv(1024)
        print(f"Receved from server: {recvBuffer.decode()}")


# udp
# with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
#     sock.connect((HOST, PORT))

#     while True:
#         inStr = input("What do you want to send? ")
#         sock.send(inStr.encode())
