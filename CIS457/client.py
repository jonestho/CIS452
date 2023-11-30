import random
import signal
import socket
import threading
import time


def signalHandler(sigNum, sigHandler):
    global interruptReceived
    interruptReceived = True
    exit(0)


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
        if int(factors[i]) != factorsByIndex[index][i]:
            isMatching = False
            break

    return isMatching


def receiveFactors():
    global clientSocket
    global factorsByIndex
    global workingThreads


    message = clientSocket.recv(1024).decode()
    receivedValues = message.split(",")

    receivedIndex = int(receivedValues[0]) - 1
    receivedValues = receivedValues[1:]

    print("Received: {}, {}\n".format(receivedIndex, receivedValues))
    print("Length of Factor List: {}".format(factorsByIndex))

    if checkFactors(receivedIndex, receivedValues):
        print("Factors at Index {} are correct.\n".format(receivedIndex + 1)) # print with 1 indexing
    else:
        print("Factors at Index {} are incorrect.\n".format(receivedIndex))

    workingThreads -= 1
    print("Threads Remaining: {}".format(workingThreads))


def sendFactors():
    global clientSocket
    global factorsByIndex
    global valueIndex
    global workingThreads

    counter = 0
    upperBound = 10

    while not interruptReceived and counter <= upperBound:
        currentFactors = generateNumbers()
        factorsByIndex.append(currentFactors)

        calculatedValue = ((2 ** currentFactors[0]) * (3 ** currentFactors[1]) * (5 ** currentFactors[2])
                           * (7 ** currentFactors[3]))

        time.sleep(0.2)

        clientSocket.send("{}, {}".format(valueIndex, calculatedValue).encode())
        print("Sent: {}, {}".format(valueIndex, calculatedValue))

        workingThreads += 1
        receiverThread = threading.Thread(target=receiveFactors)
        receiverThread.start()

        if not interruptReceived:
            valueIndex += 1
            counter += 1
    clientSocket.send("{}, {}".format(0, 0).encode()) # zero index is what kills server



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

    if isConnected:
        signal.signal(signal.SIGINT, signalHandler)

        workingThreads = 0
        factorsByIndex = []
        valueIndex = 1

        senderThread = threading.Thread(target=sendFactors)
        senderThread.start()
        senderThread.join()

        print("\nInterrupt received. Shutting down.")

        while workingThreads != 0:
            continue

        print("Thank you!")
        # clientSocket.send('0'.encode())
