import time
import json
t = time.time()
j = json.loads(vk_send('messages.getHistory', {'count': '200', 'peer_id':outMsg['peer_id']}, 1))['response']
c = j['count']
ct = 24*60*60
offset = 0
ids = ''
for i in range(c):
	if i >= offset+200:
		offset += 200
		j = json.loads(vk_send('messages.getHistory', {'count': '200', 'peer_id':outMsg['peer_id'], 'offset':str(offset)}, 1))['response']
	if t - j['items'][i-offset]['date'] > ct:
		break
	if j['items'][i-offset]['out']:
		ids+=str(j['items'][i-offset]['id'])+','
vk_send('messages.delete',{'message_ids':ids, 'delete_for_all':'1'}, 1)
outMsg={}