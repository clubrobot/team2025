import serial
import socket
import re

def get_serial(baud=9600):
    usbPort = input("Port du robot (/dev/ttyUSB0, COM1, ...): ")
    ser = serial.Serial(usbPort, baud, timeout=1)
    return ser

def get_socket():
    return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send_data(socket, data, ip="127.0.0.1", port=47269):
    socket.sendto(data.encode("utf-8"), (ip, port))

def get_line(ser):
    return ser.readline().decode("utf-8")

def main(baud, ip, port):
    socket = get_socket()
    ser = get_serial(baud)
    while(1):
        line = get_line(ser)
        if re.compile(r".+:.+").search(line):
            send_data(socket, line)

if __name__ == "__main__":
    baud = 9600
    ip = "127.0.0.1"
    port = 47269
    temp = input("Quelle baudrate ? (9600 par défaut): ")
    if temp != "":
        baud = int(temp)
    temp = input("Quelle IP ? (127.0.0.1 par défaut): ")
    if temp != "":
        ip = temp
    temp = input("Quel port ? (47269 par défaut): ")
    if temp != "":
        port = int(temp)
    main(baud, ip, port)
