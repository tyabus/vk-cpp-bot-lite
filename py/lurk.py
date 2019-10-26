import json
arg={'format':'json', 'action':'opensearch' ,'search':msg}
j=json.loads(net_send('http://lurkmore.to/api.php',arg,1))
outMsg['message']=''
for s in j[1]:
	outMsg['message']+='lurkmore.to/'+s.replace(' ', '_')+'\n'
if outMsg['message']=='':
	outMsg['message']='нетю такого'
#else:
#	outMsg['message']+='<<->> замени на точку'