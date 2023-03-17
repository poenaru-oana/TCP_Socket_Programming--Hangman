import socket

IP = "172.30.113.98"
PORT = 1600
ADDR = (IP, PORT)


def main():

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)

    ln = client.recv(4)
    ln = int.from_bytes(ln, "little")
    print("lnmsg = " + str(ln))
    msg = client.recv(ln).decode()
    ln = client.recv(4)
    ln = int.from_bytes(ln, "little")
    print("lnword = " + str(ln))
    word = client.recv(ln).decode()

    print(msg)
    print(word)

    while True:
        c = input("Input letter: ")
        client.send(c.encode())

        ln = client.recv(4)
        ln = int.from_bytes(ln, "little")
        print("lnmsg = " + str(ln))
        msg = client.recv(ln).decode()
        ln = client.recv(4)
        ln = int.from_bytes(ln, "little")
        print("lnword = " + str(ln))
        word = client.recv(ln).decode()

        print(msg)
        print(word)

#    #figure out how to disconnect after end


if __name__ == "__main__":
    main()
