
import pygame
import numpy as np
import serial
import struct

##### CONFIG #####
window_size = (
    (1920, 980),
    (1280, 720)
)[1]

fps = 60

accel = 0.25

baudrate = 9600

##################


# pygame init
pygame.init()
window = pygame.display.set_mode(window_size)
clock = pygame.time.Clock()

window_half_size = np.array(window_size) / 2

acceleration = np.array([0, 0], dtype=np.float32)

visual_range_size = min(window_half_size) * 0.8

# serial init
ser = serial.Serial("COM5", baudrate)
print(f"Connected to {ser.name} at {ser.baudrate} baudrate")

pump = False

pump_time = 0

# main loop
while True:
    # events, keypresses, mouse input etc.
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()
    
    # keypresses
    keys = pygame.key.get_pressed()
    if keys[pygame.K_w]:
        acceleration[1] += accel
    elif keys[pygame.K_s]:
        acceleration[1] -= accel
    else:
        acceleration[1] = 0

    if keys[pygame.K_a]:
        acceleration[0] -= accel
    elif keys[pygame.K_d]:
        acceleration[0] += accel
    else:
        acceleration[0] = 0

    # p key for
    if keys[pygame.K_p]:
        pump = True

    # clamp acceleration
    acceleration = np.clip(acceleration, -1, 1)

    # send data
    data = struct.pack("ff?", acceleration[0], acceleration[1], pump)
    ser.write(data + b"\n")


    window.fill((0, 0, 0))


    # draw range rectangle
    pygame.draw.rect(
        window,
        (255, 255, 255),
        (
            window_half_size[0] - visual_range_size,
            window_half_size[1] - visual_range_size,
            visual_range_size * 2,
            visual_range_size * 2
        ),
        2
    )


    # draw acceleration
    pygame.draw.circle(
        window,
        (255, 150, 50),
        (
            window_half_size[0] + acceleration[0] * visual_range_size,
            window_half_size[1] - acceleration[1] * visual_range_size
        ),
        20
    )

    if pump:
        if pump_time < 120:
            pygame.draw.circle(
                window,
                (255, 150, 50),
                (window_size[0] - 100, window_half_size[1]),
                60
            )
        else:
            pygame.draw.circle(
                window,
                (100, 150, 255),
                (window_size[0] - 100, window_half_size[1]),
                60
            )
    else:
        pygame.draw.circle(
            window,
            (100, 100, 100),
            (window_size[0] - 100, window_half_size[1]),
            60
        )

    if pump and pump_time < 121:
        pump_time += 1
        


    # update
    pygame.display.update()
    delta = clock.tick(fps) / 1000