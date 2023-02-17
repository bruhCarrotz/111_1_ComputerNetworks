import struct, threading
from socket import *

def check_icmp(rand_socket):
    for i in range(10):
        try:
            checkICMP, checkAddress = rand_socket.recvfrom(1024)
        except:
            continue

addressPort = ('140.114.89.43', 55556)
clientSocket = socket(AF_INET, SOCK_DGRAM)
clientSocket.settimeout(1)

icmpSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)
icmpSocket.settimeout(1)

openThread = threading.Thread(target=check_icmp(icmpSocket))
openThread.start()

for i in range(10):
    msg = ''
    try:
        clientSocket.sendto(msg.encode(), addressPort)
        icmp, address = icmpSocket.recvfrom(1024)

        icmpHeader = icmp[20:28]
        type, code, checksum, packetID, sequence = struct.unpack('bbHHh', icmpHeader) 
        del checksum, packetID, sequence

    except timeout:
        continue

    if(type == 0):
        message = 'echo reply'
    if(type == 3):
        message = 'destination port unreachable'
    if(type == 4):
        message = 'source quench'
    if(type == 5):
        message = 'redirect'

    print('ICMP Info: type={0} code={1}, message: {2}.'.format(type, code, message))

openThread.join() 

clientSocket.close()
icmpSocket.close()

