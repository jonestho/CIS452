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

        time.sleep(0.5)
        valueIndex += 1


def validateFactors():
    global clientSocket
    global factorsByIndex
    global valueIndex

    while not interruptReceived:
        print("Server: Index ({}) and Factors {} are correct.\n".format(valueIndex, factorsByIndex[valueIndex]))
        time.sleep(0.5)


if __name__ == '__main__':
    interruptReceived = False

    serverIPAddress = ""
    portNumber = 0

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
        receiverThread = threading.Thread(target=validateFactors)

        senderThread.start()
        receiverThread.start()

        senderThread.join()
        receiverThread.join()

        print("\nInterrupt received. Exiting now")
