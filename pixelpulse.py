import serial_IO
import serial
import time

baud_rate = 9600

selected_port = serial_IO.select_serial_port()
if not selected_port: exit()
port = serial.Serial(selected_port, baud_rate, timeout=1)

# Receive size of float as stored on arduino
arduino_float_size = serial_IO.get_bytes(port, 1)[0]
print(f'{arduino_float_size=}')


# array of float data to be sent to arduino
float_data = [2.0, 0.1, 0.2, 0.3]

prv_time = time.time() * 1000
total_time = 0
frame_count = 0

for _ in range(100):

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
    #print(serial_IO.get_bytes(port, 20), end='\t')

    curr_time =  time.time() * 1000
    del_time = curr_time - prv_time
    total_time += del_time
    frame_count+=1
    prv_time=curr_time
    print(f'{del_time:.2f}ms')

print(f'Average time per frame = {total_time / frame_count:.2f}')


port.close()