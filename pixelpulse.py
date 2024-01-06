import serial_IO
import serial
import time
import numpy as np
import pygame
import threading
import keyboard

baud_rate = 57600
image_width = 75
image_height = 50
scale_factor = 12
max_frames = 1

# Initialize pygame
pygame.init()

# Create a blank image (initially black)
image_array = np.zeros((image_width, image_height, 3), dtype=np.uint8) + 100
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
1. Image width
2. Image height
'''
float_data = [image_width, image_height]
num_pixels = image_height * image_height

def update_image():

    global prv_time
    global total_time
    global frame_count

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
        # for i in range(num_floats):
        #     print(serial_IO.get_debug_message(port))
        serial_IO.wait_for_bytes(port, b'OK')
        


    serial_IO.wait_for_bytes(port, b'OUT')
    for y in range(image_height):
        for x in range(image_width):
            pixel = serial_IO.get_bytes(port, 3)
            image_array[x, y] = pixel
        if not running:
            return

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