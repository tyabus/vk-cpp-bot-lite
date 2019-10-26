import json
import time
import re
import datetime
import math
c = json.loads(vk_send('messages.getHistory',{'count':'1', "peer_id": outMsg["peer_id"], 'rev':'1'},1))['response']
t=datetime.datetime.fromtimestamp(c['items'][0]['date']).strftime('%Y-%m-%d %H:%M:%S')
respC = int((c['count']+200-c['count']%200)/200)
res={}
words={}
count=0
ncount=0
ucount=0
uncount=0
old_id=0
outMsg['message']='0%'
m=str(json.loads(vk_send('messages.send', outMsg, 1))['response'])
for i in range(respC):
	if i % 25 == 0 and i!=0:
		vk_send("messages.edit", {"message_id": m, "message": str(int(i/respC*100))+'%', "peer_id": outMsg["peer_id"]}, 1)
	j=json.loads(vk_send('messages.getHistory',{'count':'200', 'offset': str(i*200), "peer_id": outMsg["peer_id"], 'rev':'1'},1))
	for msg in j['response']['items']:
		res[msg['user_id']]=res.get(msg['user_id'], 0)+1
		for word in re.findall(r"(\w+)", msg['body'].lower(), re.UNICODE):
			if len(word)>2:
				words[word]=words.get(word, 0)+1
		count+=1
		if msg['user_id']!=old_id:
			ncount+=1
		if msg['user_id']==user_id:
			ucount+=1
			if msg['user_id']!=old_id:
				uncount+=1
		old_id=msg['user_id']
vk_send("messages.edit", {"message_id": m, "message": 'done', "peer_id": outMsg["peer_id"]}, 1)
top=[(k, res[k]) for k in sorted(res.keys(), key=res.get, reverse=True)]
words=[(k, words[k]) for k in sorted(words.keys(), key=words.get, reverse=True)][:50]
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
outMsg['message']='История сообщений начинается: '+t+'\n\n'
outMsg['message']+='Сообщений: '+str(count)+'('+str(ncount)+')\n'
outMsg['message']+='Твоих: '+str(ucount)+'('+str(uncount)+')\n'
outMsg['message']+='\nТоп человек:\n'
for i in range(len(top)):
	if top[i][1]==1:
		break
	outMsg['message']+=str(i+1)+') '+names[top[i][0]]+'-'+str(top[i][1])+'\n'
outMsg['message']+='\nТоп 50 слов:\n'
for i in range(len(words)):
	outMsg['message']+=str(i+1)+') '+words[i][0]+'-'+str(words[i][1])+'\n'
lines=outMsg['message'].split('\n')
r=math.ceil(len(lines)/100)
for i in range(r):
	outMsg['message']=''
	for line in lines[i*100:100+i*100]:
		outMsg['message']+=line+'\n'
	if i<r-1:
		vk_send('messages.send', outMsg, 1) 