import random
import signal
import socket
import threading
import time


def signalHandler(sigNum, sigHandler):
    global interruptReceived
    interruptReceived = True


def generateNumbers():
    factorA = random.randrange(1, 8)
    factorB = random.randrange(1, 8)
    factorC = random.randrange(1, 8)
    factorD = random.randrange(1, 8)

    factors = [factorA, factorB, factorC, factorD]

    return factors


def checkFactors(index, factors):
    global factorsByIndex
    isMatching = True

    for i in range(len(factors)):
        if factors[i] != factorsByIndex[index][i]:
            isMatching = False
            break

    return isMatching


def receiveFactors(index):
    global clientSocket
    global factorsByIndex

    time.sleep(2)
    message = clientSocket.recv(1024).decode()
    print(message)
    if checkFactors(index, factorsByIndex[index]):
        print("Server: Index ({}) and Factors {} are correct.\n"
              .format(index, factorsByIndex[index]))
    else:
        print("Server: wrong\n")


def sendFactors():
    global clientSocket
    global factorsByIndex
    global valueIndex

    while not interruptReceived:
        currentFactors = generateNumbers()
        factorsByIndex.append(currentFactors)

        calculatedValue = ((2 ** currentFactors[0]) * (3 ** currentFactors[1]) * (5 ** currentFactors[2])
                           * (7 ** currentFactors[3]))

        print("Client: Sending Index ({}) and Value ({})\n".format(valueIndex, calculatedValue))

        receiverThread = threading.Thread(target=receiveFactors, args=(valueIndex,))
        receiverThread.start()

        time.sleep(0.5)

        if not interruptReceived:
            valueIndex += 1


if __name__ == '__main__':
    interruptReceived = False

    serverIPAddress = "127.0.0.1"
    portNumber = 5000

    clientSocket = socket.socket()
    isConnected = False

    try:
        clientSocket.connect((serverIPAddress, portNumber))
        isConnected = True
    except socket.error as clientError:
        print("Error: Could not connect to server. Exiting now")
        isConnected = True  # FOR TESTING

    if isConnected:
        signal.signal(signal.SIGINT, signalHandler)

        factorsByIndex = []
        valueIndex = 0

        senderThread = threading.Thread(target=sendFactors)
        senderThread.start()

        senderThread.join()

        print("\nInterrupt received. Exiting now")
