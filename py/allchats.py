import json
import time
import re
import datetime
import math
res={}
words={}
count=0
ncount=0
chat_id=0
outMsg['message']=str(chat_id)
m=str(json.loads(vk_send('messages.send', outMsg, 1))['response'])
try:
	while True:
		old_id=0
		chat_id+=1
		vk_send("messages.edit", {"message_id": m, "message": str(chat_id), "peer_id": outMsg["peer_id"]}, 1)
		c = json.loads(vk_send('messages.getHistory',{'count':'1', 'chat_id':str(chat_id), 'rev':'1'},1))['response']
		respC = int((c['count']+200-c['count']%200)/200)
		for i in range(respC):
			j=json.loads(vk_send('messages.getHistory',{'count':'200', 'offset': str(i*200), 'chat_id':str(chat_id), 'rev':'1'},1))
			for msg in j['response']['items']:
				res[msg['user_id']]=res.get(msg['user_id'], 0)+1
				for word in re.findall(r"(\w+)", msg['body'].lower(), re.UNICODE):
					if len(word)>2:
						words[word]=words.get(word, 0)+1
				count+=1
				if msg['user_id']!=old_id:
					ncount+=1
				old_id=msg['user_id']
except:
	0
vk_send("messages.edit", {"message_id": m, "message": 'done', "peer_id": outMsg["peer_id"]}, 1)
top=[(k, res[k]) for k in sorted(res.keys(), key=res.get, reverse=True)]
words=[(k, words[k]) for k in sorted(words.keys(), key=words.get, reverse=True)]
users=[]
grups=[]
names={}
for u in top:
	if u[0] > 0:
		users+=[str(u[0])]
	else:
		grups+=[str(-u[0])]
for i in range(math.ceil(len(users)/1000)):
	us=''
	for u in users[1000*i:1000+1000*i]:
		us+=u+','
	if us!='':
		us=json.loads(vk_send('users.get',{'user_ids':us, 'fields':'domain'},1))['response']
		for u in us:
			names[u['id']]=u['first_name']+'( vk.com/'+u['domain']+' )'
for i in range(math.ceil(len(grups)/500)):
	gs=''
	for g in grups[500*i:500+500*i]:
		gs+=g+','
	if gs!='':
		gs=json.loads(vk_send('groups.getById',{'group_ids':gs},1))['response']
		for g in gs:
			names[-g['id']]=g['name']+'( vk.com/'+g['screen_name']+' )'
outMsg['message']='Сообщений: '+str(count)+'('+str(ncount)+')\n'
outMsg['message']+='\nТоп человек:\n'
for i in range(len(top)):
	if top[i][1]==1:
		break
	if names.get(top[i][0], '')=='':
		continue
	outMsg['message']+=str(i+1)+') '+names[top[i][0]]+'-'+str(top[i][1])+'\n'
outMsg['message']+='\nВсего употребленно слов: '+str(len(words))+'\n'
outMsg['message']+='\nТоп 500 слов:\n'
for i in range(len(words[:500])):
	outMsg['message']+=str(i+1)+') '+words[i][0]+'-'+str(words[i][1])+'\n'
lines=outMsg['message'].split('\n')
r=math.ceil(len(lines)/100)
for i in range(r):
	outMsg['message']=''
	for line in lines[i*100:100+i*100]:
		outMsg['message']+=line+'\n'
	if i<r-1:
		vk_send('messages.send', outMsg, 1) 