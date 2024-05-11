import socket

HOST = "127.0.0.1"
PORT = 1234
HEADERLENGTH = 10

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()

        socketList = [s]
        clients = {}
        print(f"Listening for connections on : {HOST}: {PORT}")

        while True:
            readSockets, _, exeptionSocket = socket.select(socketList, [], socketList)
            for notifiedSocket in readSockets:
                if notifiedSocket:
                    conn, addr = s.accept()
                    socketList.append(conn)

                    userNameLength = int(conn.recv(HEADERLENGTH).decode().strip())
                    userName = conn.recv(userNameLength).decode

                    clients[conn] = userName
                    print(f"Accepted new connection from {conn}, username: {userName}")
                else:
                    user = clients[notifiedSocket]
                    print(f"Received messge from {user}:{notifiedSocket}")
                # recvMessage = conn.recv(1024).decode()
                    recvHeader = int(notifiedSocket.recv(10).decode().strip)
                    print(f"message sixe ")


if __name__ == "__main__":
    main()