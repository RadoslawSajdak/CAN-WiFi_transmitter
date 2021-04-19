import socket
from time import time
import DATA as dt
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('192.168.0.157', 1313))
sock.listen(1)

start_time = 0
stop_time = 0
lock = True
interval = 10
if __name__ == "__main__":
    print("Program started")
    try:
        while 1:
            client, address = sock.accept()
            print ("Connected from", address)
            # loop serving the new client
            while True:
                receivedData = client.recv(1024)

                if not receivedData: break
                else:
                    if lock:
                        start_time = time()
                        lock = False
                        #print(start_time)
                    interval -= 1
                if interval == 0:
                    #print(len(receivedData))
                    #print(receivedData)
                    break

            client.close()
            #print("NO DATA")
            if start_time > 0:
                stop_time = time()
                #print("TIME")
                print(stop_time - start_time)
                start_time = 0
                lock = True
                interval = 10
            print("Disconnected from", address)
    finally:
        sock.close()
    

