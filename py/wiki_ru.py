import json
import binascii
outMsg['message']=''
res=json.loads(net_send('https://ru.wikipedia.org/w/api.php',{'action':'opensearch','search':msg,'limit':'10','namespace':'0','format':'json'}, 1))
if len(res[2]):
	outMsg['message']+='¤'+res[1][0]+'\n'+res[2][0]+'\n( '+binascii.a2b_qp(res[3][0].replace('%','=').encode("utf-8")).decode()+' )\n\n'
	if len(res[2])>1:
		outMsg['message']+='Другие варианты:\n'
		for title in res[1][1:]:
			outMsg['message']+='•'+title+'\n'