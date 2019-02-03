import socket
import struct
import time
import re
gamedir = msg
if len(gamedir) == 0:
	gamedir = 'valve'
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
adr=('ms.xash.su', 27010)
sock.bind(('0.0.0.0', 0))
sock.sendto(b'1\xff0.0.0.0:0\0\\gamedir\\'+gamedir.encode()+b'\\clver\\1200\\\0', adr)
data = sock.recvfrom(4096)
#print(data)
s = 6
servs={}
slots={}
while True:
	IP = struct.unpack('BBBB',data[0][s:s+4])
	ipstr = '%d.%d.%d.%d' % IP
	port = struct.unpack('>H',data[0][s+4:s+6])[0]
	if port == 0: break
	sock.sendto(b'\xff\xff\xff\xffinfo 48',(ipstr,port))
	s = s + 6
starttime = time.time()
sock.setblocking(0)
while time.time() - starttime < 2:
	try:
		data = sock.recvfrom(4096)
		serv = re.sub(r'\^\d{1}', '', data[0][10: ].decode()).split('\\')
		servs[serv[15]]=servs.get(serv[15], [])+[[serv[1]]+[serv[3]]+[serv[11]]+[serv[13]]+[str(int((time.time() - starttime)*1000))]+[data[1][0]+':'+str(data[1][1])]]
		slot=slots.get(serv[15], [0, 0])
		slot[0]+=int(serv[11])
		slot[1]+=int(serv[13])
		slots[serv[15]]=slot
	except:
		pass
sock.close()
msg=''
for key in servs.keys():
	msg+=key+'-'+str(slots[key][0])+'/'+str(slots[key][1])+'-'+str(int(slots[key][0]*100/slots[key][1]))+'%\n'
	for serv in servs[key]:
		msg+='    '+serv[0]+'-'+serv[1]+'-'+serv[2]+'/'+serv[3]+'-('+serv[5]+'-'+serv[4]+'ms)\n'
outMsg["message"]=msg