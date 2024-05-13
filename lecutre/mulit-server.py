import socket
import select

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
            readSocket, _, exceptionSockts = select.select(socketList, [], socketList)
            for notifeiedSocket in readSocket:
                if notifeiedSocket == s:
                    conn, addr = s.accept()
                    socketList.append(conn)
                    usernameLen = int(conn.recv(HEADERLENGTH).decode().strip())
                    username = conn.recv(usernameLen).decode()
                    clients[conn] = username
                    print(f"accepted new connection from {conn}, username: {username}")
                else:
                    user = clients[notifeiedSocket]
                    print(f"receved message from {notifeiedSocket}")
                    # recvMessage = conn.recv(1024).decode()
                    recvHeader = notifeiedSocket.recv(HEADERLENGTH)
                    print(f"recvHeader: {recvHeader}")
                    recvHeaderLen = int(recvHeader.decode().strip())
                    print(f"message size: {recvHeaderLen}")
                    recvMessage = notifeiedSocket.recv(recvHeaderLen).decode()
                    print(f"message receved: {recvMessage}")
                    for clientSocket in clients:
                        if clientSocket != notifeiedSocket:
                            userHeader = f"{len(user):<{HEADERLENGTH}}"
                            sendHeader = f"{len(recvMessage)+len(user):<{HEADERLENGTH}}"

                            clientSocket.send(f"{userHeader}{user}{sendHeader}{recvMessage}".encode())


if __name__ == "__main__":
    main()
