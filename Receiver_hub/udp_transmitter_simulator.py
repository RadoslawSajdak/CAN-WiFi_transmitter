import socket
import numpy as np
from time import sleep

HOST = "192.168.0.157"
PORT = 1313
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def toBytes(address,value):
    result = []
    for i in range(4):
        result.append((address & (0xff << (24 - 8 * i))) >> (24 - 8 * i))
    for i in range(8):
        result.append((value & (0xff << (56 - 8 * i))) >> (56 - 8 * i))
    return bytes(result)


if __name__ == "__main__":
    t = 0
    while True:
        a = np.sin(np.pi/2 + t)
        print(a)
        sock.sendto(toBytes(0x80, 0x11223344aabbccddee), (HOST, PORT))
        t += 0.01
        sleep(0.01)
