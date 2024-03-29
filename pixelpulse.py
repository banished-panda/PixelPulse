import serial_IO
import serial
import time
import numpy as np
import pygame
import threading
import keyboard

baud_rate = 57600
image_width = 400
image_height = 225
scale_factor = 2
max_frames = 1

upper_sky_color = [125, 145, 255]
lower_sky_color = [255, 255, 255]

sphere_list = [
    # x, y, z, radius,
    [0.0, 0.0, -1.0, 0.5],
    [0.0, -100.5, -1.0, 100.0],
]

# Initialize pygame
pygame.init()

# Create a blank image (initially black)
image_array = np.zeros((image_width, image_height, 3), dtype=np.uint8) + 100
cumulative_image = np.zeros((image_width, image_height, 3), dtype=np.uint32)
N = 1
image_surface = pygame.surfarray.make_surface(image_array)

# Create a window
scaled_width = int(image_width * scale_factor)
scaled_height = int(image_height * scale_factor)
window_size = (scaled_width, scaled_height)
screen = pygame.display.set_mode(window_size)
pygame.display.set_caption('Image Display')

selected_port = serial_IO.select_serial_port()
if not selected_port: exit()
port = serial.Serial(selected_port, baud_rate, timeout=1)

# Receive size of float as stored on arduino
arduino_float_size = serial_IO.get_bytes(port, 1)[0]

# array of float data to be sent to arduino
'''
Structure of data:
1.  Image width
2.  Image height
3.  SkyColor1 R (UP)
4.  SkyColor1 G (UP)
5.  SkyColor1 B (UP)
6.  SkyColor2 R (DOWN)
7.  SkyColor2 G (DOWN)
8.  SkyColor2 B (DOWN)
9.  Number of spheres
10. Stride (in no. of floats)
'''
float_data = [
    image_width,
    image_height,
]
for c in upper_sky_color:
    float_data.append(c/255.0)
for c in lower_sky_color:
    float_data.append(c/255.0)
float_data.append(len(sphere_list))
float_data.append(len(sphere_list[0]) if len(sphere_list) != 0 else 0)
for sphere in sphere_list:
    for val in sphere:
        float_data.append(val)


num_pixels = image_height * image_height

first_update = True
def update_image():

    global first_update
    global N

    # Wait for arduino to say READY
    serial_IO.wait_for_bytes(port, b'READY') 

    # Send choice: either '1' or '2'
    choice = 1
    serial_IO.send_bytes(port, choice.to_bytes(1, 'little'))
    # print(serial_IO.get_debug_message(port))

    if choice == 1:
        num_floats = len(float_data)
        # Send the number of floats to be received first
        serial_IO.send_bytes(port, num_floats.to_bytes(1, 'little'))
        serial_IO.send_floats(port, float_data, arduino_float_size)
        print(float_data)
        # for i in range(num_floats):
        #     print(serial_IO.get_debug_message(port))
        serial_IO.wait_for_bytes(port, b'OK')

    serial_IO.wait_for_bytes(port, b'OUT')
    for y in range(image_height):
        for x in range(image_width):
            # pixel = serial_IO.get_bytes(port, 3)
            pixel = serial_IO.get_color(port)
            if first_update:
                image_array[x, y] = pixel
                cumulative_image[x, y] = cumulative_image[x, y] + pixel
            else:
                cumulative_image[x, y] = cumulative_image[x, y] + pixel
                image_array[x, y] = cumulative_image[x, y] / N
        if not running:
            return
    
    first_update = False
    N+=1

running = True
def update_image_thread():
    frame_count = 0
    while running and frame_count < max_frames:
        start_time = time.time()
        update_image()
        end_time = time.time()
        frame_count += 1
        print(f"Image updated in {end_time-start_time :.2f} seconds")
        #time.sleep(0.1)

# Start a thread for continuous image updates
update_thread = threading.Thread(target=update_image_thread)
update_thread.start()

# Main loop for handling events and updating the display
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Convert the NumPy array to a surface
    image_surface = pygame.surfarray.make_surface(image_array)

    # Scale the surface
    scaled_surface = pygame.transform.scale(image_surface, window_size)

    # Blit the surface onto the screen
    screen.blit(scaled_surface, (0, 0))

    # Update the display
    pygame.display.flip()

    # Control the frame rate
    pygame.time.delay(30)

update_thread.join()
port.close()
pygame.quit()