import json
try:
	id=''
	try:
		id=str(json.loads(vk_send('messages.getById', {'message_ids':str(msg_id)}, 1))['response']['items'][0]['fwd_messages'][0]['user_id'])
	except:
		id=msg.split('/')[-1].split('|')[0].replace('[', '')
		id=str(json.loads(vk_send('users.get',{'user_ids':id},1))['response'][0]['id'])
	vk_send('messages.addChatUser', {'chat_id':str(chat_id), 'user_id':id}, 1)
	outMsg={'peer_id':outMsg['peer_id']}
except:
	outMsg['message']='¯\\_(ツ)_/¯'