import struct, time
from socket import *
from collections import defaultdict

MESSAGE = ''
DESTINATION_ADDRESS = gethostbyname('140.114.89.43')  

def UDP(ttl):
    UDP = socket(AF_INET, SOCK_DGRAM)
    UDP.setsockopt(SOL_IP, IP_TTL, ttl)
    return UDP

def ICMP(maxTimeout=1):
    ICMP = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)
    ICMP.settimeout(maxTimeout)
    return ICMP

def check_info(packet, port):
    if packet[20] == 11: return 1 #ttl
    if packet[20] == 3: return 1 #destination unreachable
    if struct.unpack('!H', packet[50:52])[0] != port: return False

def checkPort():
    while True:
        for i in range(1,2):
            yield i

def listenICMP(mySocket, port):
    start = time.time()

    while True:
        if time.time()-start > 1:
            raise timeout

        ICMPpacket, address = mySocket.recvfrom(1024)

        if not check_info(ICMPpacket, port):
            continue
        if check_info(ICMPpacket, port) == False:
            continue

        return address[0]

def trace(ip_addr, ttl):
    port = checkPort()

    UDPsocket = UDP(ttl)
    ICMPsocket = ICMP()

    try:
        next_port = next(port)
        start = time.time()

        UDPsocket.sendto(MESSAGE.encode(), (ip_addr, next_port))
        addr = listenICMP(ICMPsocket, next_port)

        end = time.time()
        ttlTime = end-start
        return (addr, ttlTime)

    finally:
        if UDPsocket is not None:
            UDPsocket.close()
        if ICMPsocket is not None:
            ICMPsocket.close()

def count_ttl(time):
    if time == None:
        return False #timeout
    return f'{time * 1000:.3f} ms'

def traceroute(DESTINATION_ADDRESS, ttl):
    resultList = []
    for i in range(3):
        try:
            ipAddress, ttlTime = trace(DESTINATION_ADDRESS, ttl)
            resultList.append((ipAddress, count_ttl(ttlTime)))
        except timeout:
            resultList.append(('*', count_ttl(None)))

    return resultList

def output(seq, lists):
    result = defaultdict(list)

    for i, j in lists:
        result[i].append(j)

    for index, (addr, time) in enumerate(result.items()):
        try:
            host_details = gethostbyaddr(addr)
            if len(host_details) > 0:
                domain = host_details[0]
        except:    
            domain = addr
                
        if len(time) < 3:
            print('{} *'.format(seq))
            continue

        if time[0] == False:
            print('Request timed out.')
            seq += 1
            continue

        print('{0} {1} ({2}) {3} {4} {5}'.format(seq, domain, addr, time[0], time[1], time[2]))

#main starter
for i in range(1,30):
    result = traceroute(DESTINATION_ADDRESS, i)
    output(i, result)
    if result[0][0] == DESTINATION_ADDRESS:
        break
