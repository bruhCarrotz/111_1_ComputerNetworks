import time
from socket import *

addressPort = ('140.114.89.43', 55555)
clientSocket = socket(AF_INET, SOCK_DGRAM)
clientSocket.settimeout(1)

count = 1
lossCount = 0
rttList = []

for i in range(10):
    msg = 'PING ' + str(count)
    try:
        clientSocket.sendto(msg.encode(), addressPort)
        start = time.time()
        msg, address = clientSocket.recvfrom(1024)
        end = time.time()
        elapsed = end - start
        
        rttTime = round(elapsed*1000, 3)
        rttList.append(rttTime)

        print(msg.decode(), rttTime)

        count = count + 1
    except timeout:
        print('Request time out.')
        lossCount = lossCount + 1
        count = count + 1

print('Result: ')

rttAverage = sum(rttList)/len(rttList)
print('Average RTT', rttAverage)

print('Packet loss rate', lossCount/10)

clientSocket.close()
