import serial
import socket
import re

def get_serial(baud: int=9600) -> serial.Serial:
    """
    Get the serial port of the robot
    :param baud: baudrate of the serial port
    :return: the serial port object
    """
    usbPort = input("Port du robot (/dev/ttyUSB0, COM1, ...): ")
    ser = serial.Serial(usbPort, baud, timeout=1)
    return ser

def get_socket() -> socket.socket:
    """
    Initialize a udp socket to send data
    :return: the socket object
    """
    return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send_data(socket : socket.socket, data : str, ip : str="127.0.0.1", port : int=47269) -> None:
    """
    Send data to the server
    :param socket: the socket object
    :param data: the data to send
    :param ip: the ip of the server
    :param port: the port of the server
    """
    socket.sendto(data.encode("utf-8"), (ip, port))

def get_line(ser : serial.Serial) -> str:
    """
    Get a line from the serial port
    :param ser: the serial port object
    :return: the line read
    """
    return ser.readline().decode("utf-8")

def main(baud : int, ip : str, port : int) -> None:
    """
    Main function
    :param baud: baudrate of the serial port
    :param ip: ip of the server
    :param port: port of the server
    """
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
