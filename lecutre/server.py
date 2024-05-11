import socket

HOST = "127.0.0.1"
PORT = 1234

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    # set options
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    sock.bind((HOST, PORT))
    # sock.listen(5)

    # conn, addr = sock.accept()

    # while conn:
    #     print(f"conntected by {addr}")
    #     while True:
    data = conn.recv(1024)
            # if not data:
            #     break
    print(f"Received {data}")
        # conn.send(data)
