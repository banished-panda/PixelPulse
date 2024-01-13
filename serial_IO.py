import serial
from serial.tools import list_ports
import struct

float_format_map = {
    # mapping between size occupied and format name for C struct
    # for the float data type
    # bytes : format
    8 : 'd',
    4 : 'f',
    2 : 'e',
}

def select_serial_port():
    ports = list(list_ports.comports())
    if not ports:
        print("No serial ports available.")
        return None

    print("Available serial ports:")
    for i, port in enumerate(ports):
        print(f"{i + 1}. {port}")
    
    if(len(ports) == 1):
        print(f'Attempting: {ports[0]}')
        selected_port = ports[0].device
        return selected_port        

    selected_port_index = int(input("Enter the number of the port to connect: ")) - 1

    if 0 <= selected_port_index < len(ports):
        selected_port = ports[selected_port_index].device
        return selected_port
    else:
        print("Invalid port selection.")
        return None

def wait_for_bytes(ser : serial.Serial, look_str : bytes):
    matched = b''
    looking_for = look_str[0:1]
    while True:
        msg = ser.read(size=1)
        if msg == b'' : continue
        if looking_for == msg:
            matched = matched + msg
            if matched == look_str:
                return
            looking_for = look_str[len(matched):len(matched)+1]
        else:
            matched = b''
            looking_for = look_str[0:1]

def get_debug_message(ser : serial.Serial):
    msg_str = b''
    while True:
        msg = ser.read(size=1)
        if msg == b''  : continue
        if msg == b'\n' : return "DEBUG: " + msg_str.decode()
        msg_str += msg

def send_bytes(ser : serial.Serial, byte):
    n = ser.write(byte)
    ser.flush()
    return n

def get_bytes(ser : serial.Serial, n: int):
    bytes_received = []
    received = 0
    while received < n:
        msg = ser.read(size=1)
        if msg == b''  : continue

        bytes_received.append(int.from_bytes(msg, "big"))
        received += 1
    return bytes_received

def get_color(ser: serial.Serial):
    color = 0
    # Read the long value from Arduino
    line = ser.readline().decode('utf-8').rstrip()
    
    # Check if the received data is a valid long value
    try:
        color = int(line)
    except ValueError:
        color = 0
    # color = int.from_bytes(msg, "big")
    color = bin(color)[-(8*3):]
    b = int('0b'+color[:8]  , 2)
    g = int('0b'+color[8:16], 2)
    r = int('0b'+color[16:] , 2)
    return [r, g, b]

def send_float(ser : serial.Serial, val : float, arduino_float_size=4):
    f = float_format_map[arduino_float_size]
    float_bytes = struct.pack(f, val)
    ser.write(float_bytes)

def send_floats(ser : serial.Serial, vals : float, arduino_float_size=4):
    f = float_format_map[arduino_float_size]
    float_bytes = struct.pack(f*len(vals), *vals)
    ser.write(float_bytes)