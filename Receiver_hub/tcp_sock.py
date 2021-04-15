import socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('192.168.0.157', 1313))
sock.listen(1)


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
                # Echo back the same data you just received
                print(receivedData)
                client.send(receivedData)

            client.close()
            print("Disconnected from", address)
    finally:
        sock.close()
    

