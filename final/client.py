# Final project for networks
# Bryleigh Koci

import socket
import pygame

HOST = "127.0.0.1"
PORT = 1234

def drawScreen(screen, data) -> None:
    """draws pygame screen

    Args:
        screen (pygame.surface): pygame screen
        data (list[str]): data receved from server to draw
    """
    screen.fill((0, 0, 0)) # draws black
    if len(data) % 6 == 0:
        for i in range(len(data) // 6):
            x = int(data[0 + (6 * i)])
            y = int(data[1 + (6 * i)])
            r = int(data[2 + (6 * i)])
            g = int(data[3 + (6 * i)])
            b = int(data[4 + (6 * i)])
            size = int(data[5 + (6 * i)])
            pygame.draw.rect(screen, (r, g, b), (x, y, size, size))
    pygame.display.update()

def main():
    # create socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        # connect
        sock.connect((HOST, PORT))

        # receve screen dimentions
        size = sock.recv(1024)
        dimentions = size.decode().strip().split(", ")

        # initate pygame screen
        screen = pygame.display.set_mode((int(dimentions[0]), int(dimentions[1])))
        pygame.display.set_caption("Game")
        clock = pygame.time.Clock()

        run = True
        while run:
            clock.tick(60)

            # close pygame window
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    run = False

            # detect and send input
            key = pygame.key.get_pressed()

            if key[pygame.K_LEFT] or key[pygame.K_a]:
                sock.send("left".encode())
            elif key[pygame.K_RIGHT] or key[pygame.K_d]:
                sock.send("right".encode())
            elif key[pygame.K_UP] or key[pygame.K_w]:
                sock.send("up".encode())
            elif key[pygame.K_DOWN] or key[pygame.K_s]:
                sock.send("down".encode())

            # receve data from server
            data = sock.recv(1024).decode().rstrip(", ").split(", ")
            # print(f"receved: {data}")

            drawScreen(screen, data)

    pygame.quit()

if __name__ == "__main__":
    main()
