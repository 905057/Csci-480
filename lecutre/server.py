import socket

HOST = "127.0.0.1"
PORT = 1234

#echo
# sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((HOST, PORT))
    sock.listen(5)
    conn, addr = sock.accept()

    data = conn.recv(1024)
    print(f"receved data {data.decode()}")
    conn.send(data)

    while conn:
        print(f"connected by {addr}")
        while True:
            data = conn.recv(1024)
            if not data:
                break
            print(f"receved data {data}")
            conn.send(data)

# socket.close()

# udp
with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((HOST, PORT))
    while True:
        data = sock.recv(1024)
        print(f"receved data {data.decode()}")
