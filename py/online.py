import json
if chat_id:
	outMsg['message']=''
	j = vk_send('messages.getChat', {'chat_id':str(chat_id), 'fields':'online'}, 1)
	j=json.loads(j)['response']
	c=0
	for u in j['users']:
		if u['type'] == 'profile':
			if u['online']:
				outMsg['message']+=u['first_name']+' '+u['last_name']+'\n'
				c+=1
	outMsg['message']=j['title']+' ('+str(c)+'/'+str(len(j['users']))+')\n\n'+outMsg['message']
else:
	outMsg['message']='ну ты онлайн...'