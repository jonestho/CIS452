import random
import signal
import socket
import sys


def signalHandler(sigNum, signalFrame):
    print("Interrupt received. Closing client connection.")
    sys.exit(0)


def generateNumbers():
    factorA = random.randrange(1, 8)
    factorB = random.randrange(1, 8)
    factorC = random.randrange(1, 8)
    factorD = random.randrange(1, 8)

    factors = [factorA, factorB, factorC, factorD]

    return factors


serverIPAddress = ""
portMPI = 0

clientSocket = socket.socket()

clientInput = ""
isConnected = False

try:
    clientSocket.connect((serverIPAddress, portMPI))

except socket.error as clientError:
    print("Error: Could not connect to server. Exiting now")


isConnected = True  # FOR TESTING

if isConnected:
    valueIndex = 0
    calculatedValue = 0

    signal.signal(signal.SIGINT, signalHandler)

    while clientInput != -1 or clientInput != "-1":
        factors = generateNumbers()

        print("Factor A: {}".format(factors[0]))
        print("Factor B: {}".format(factors[1]))
        print("Factor C: {}".format(factors[2]))
        print("Factor D: {}".format(factors[3]))

        clientInput = -1
