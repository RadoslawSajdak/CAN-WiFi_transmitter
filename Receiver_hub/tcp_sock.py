import socket
import time
import DATA as dt
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('192.168.0.157', 1313))
#sock.listen(1)

BUFFER_LEN = 100
start_time = 0
stop_time = 0
lock = True
interval = 1
sum_data = 0
if __name__ == "__main__":
    print("Program started")
    try:
        while 1:
            #client, address = sock.accept()
            #print ("Connected from", address)
            # loop serving the new client
            while True:
                print("Waiting for data")
                receivedData, address = sock.recvfrom(BUFFER_LEN * 12)



                if not receivedData: break
                else:
                    if lock:
                        start_time = time.time_ns()
                        lock = False
                        #print(start_time)
                    interval -= 1
                    sum_data += len(receivedData)
                    print(receivedData)
                if interval == 0:

                    break


            #print("NO DATA")
            if start_time > 0:
                time.sleep(1e-6)
                stop_time = time.time_ns()
                #print("TIME")
                print(sum_data)
                print(stop_time, start_time)
                print((1e9 * sum_data)/(stop_time - start_time))
                start_time = 0
                lock = True
                interval = 1
                sum_data = 0
            #client.close()
            print("Disconnected from", address)
    finally:
        sock.close()
    

