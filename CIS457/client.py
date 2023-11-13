import random
import signal
import socket
import sys
import time

interruptReceived = False

def interruptHandler(sigNum, signalFrame):
    global interruptReceived
    interruptReceived = True

def receivedFromServer():
    print("Here!")

def generateNumbers():
    factorA = random.randrange(1, 8)
    factorB = random.randrange(1, 8)
    factorC = random.randrange(1, 8)
    factorD = random.randrange(1, 8)

    factors = [factorA, factorB, factorC, factorD]

    return factors


serverIPAddress = ""
portNumber = 0

clientSocket = socket.socket()

clientInput = ""
isConnected = False

try:
    clientSocket.connect((serverIPAddress, portNumber))

except socket.error as clientError:
    print("Error: Could not connect to server. Exiting now")


isConnected = True  # FOR TESTING

if isConnected:
    valueIndex = 0
    calculatedValue = 0
    factorsByIndex = []

    signal.signal(signal.SIGINT, interruptHandler)

    while not interruptReceived:
        factors = generateNumbers()
        factorsByIndex.append(factors)

        calculatedValue = (2 ** factors[0]) * (3 ** factors[1]) * (5 ** factors[2]) * (7 ** factors[3])
        print("{}, {}".format(valueIndex, calculatedValue))

        

        # send to server
        # when server 

        time.sleep(0.5)
        valueIndex += 1

    print("\nInterrupt received. Exiting now")
        