# Final project for networks
# Bryleigh Koci

import socket
import threading
import time

HOST = "127.0.0.1"
PORT = 1234

WIDTH = 500
HEIGHT = 500
COLORS = [(255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 255, 255)]
SIZE = 50

class Player():
    """player class"""
    def __init__(self, x, y, size, color, client) -> None:
        """initalization

        Args:
            x (int): _description_
            y (int): _description_
            size (int): _description_
            color (tuple[int, int, int]): color of player
            client (_type_): client that controls player
        """
        self.size = size
        self.x = x
        self.y = y
        self.color = color
        self.speed = 5
        self.client = client

    def move(self, clientInput: str) -> None:
        """handels player movement

        Args:
            clientInput (str): direction
        """
        if clientInput == "left":
            if self.x > 0:
                self.x -= self.speed
        if clientInput == "right":
            if self.x < WIDTH - self.size:
                self.x += self.speed
        if clientInput == "up":
            if self.y > 0:
                self.y -= self.speed
        if clientInput == "down":
            if self.y < HEIGHT - self.size:
                self.y += self.speed

    def dataToSend(self) -> str:
        """condenses player data int str

        Returns:
            str: player data
        """
        return f"{self.x}, {self.y}, {self.color[0]}, {self.color[1]}, {self.color[2]}, {self.size}"

def handle_client(client_socket, clients, players) -> None:
    """handles incoming client data
    """
    while True:
        try:
            input = client_socket.recv(1024).decode()
            if not input:
                break
            # print(f"message receved: {input}")
            for player in players:
                if player.client == client_socket:
                    player.move(input)
        except ConnectionResetError:
            break
        except Exception as e:
            print(f"Error: {e}")
            break

    client_socket.close()
    clients.remove(client_socket)
    for player in players:
        if player.client == client_socket:
            players.remove(player)
            break  # Exit loop after removing the player

def sendData(players) -> str:
    """generates data to send

    Args:
        players (list[Players]): list of players

    Returns:
        str: string of data to send
    """
    data = ""
    for player in players:
        data += f"{player.dataToSend()}, "
    return data

def broadcast_data(clients, players) -> None:
    """sends data to all clients
    """
    while True:
        # Broadcast data to all clients
        for client in clients:
            try:
                client.send(sendData(players).encode())
            except Exception as e:
                print(f"Error broadcasting data: {e}")

        time.sleep(.025)

def main() -> None:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((HOST, PORT))
        server_socket.listen()
        # print(f"Server listening on {HOST}:{PORT}")

        clients = []
        players = []

        # Start broadcasting thread
        broadcast_thread = threading.Thread(target=broadcast_data, args=(clients, players))
        broadcast_thread.start()

        while True:
            client_socket, client_address = server_socket.accept()
            client_socket.send(f"{WIDTH}, {HEIGHT}".encode())

            # print(f"Accepted connection from {client_address}")

            clients.append(client_socket)
            players.append(Player(WIDTH //2, HEIGHT //2, SIZE,
                                  COLORS[(len(clients) -1) % 4], client_socket))

            # Start a new thread to handle the client
            client_thread = threading.Thread(target=handle_client,
                                             args=(client_socket, clients, players,))
            client_thread.start()

if __name__ == "__main__":
    main()
