import socket
import time
import DATA as dt
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('192.168.0.157', 1313))
#sock.listen(1)

BUFFER_LEN = 10


def decode(toDecode):
    address_local = ""
    data = 0
    for i in range(12):
        if i < 4:
            address_local += (str(hex(toDecode[i])))[2:4]
        else:
            data |= toDecode[i] << (56 - 8*(i - 4))
    return (("0x" + address_local[-4:]), hex(data))

if __name__ == "__main__":
    print("Program started")
    try:
        while 1:
            while True:
                print("Waiting for data")
                receivedData, address = sock.recvfrom(BUFFER_LEN * 12)

                if not receivedData: break
                else:
                    for i in range(0, len(receivedData), 12):
                        val = decode(receivedData[i:i + 12])
                        dt.CAN[val[0]].put(val[1])
                    print(dt.CAN["0x0660"].get())

            print("Disconnected from", address)
    finally:
        sock.close()
    

