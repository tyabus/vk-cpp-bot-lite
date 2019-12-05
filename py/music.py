import json
import random
q=msg
j = json.loads(vk_send('audio.search',{'v':'5.63', 'q':q, 'count':'300'}, True))
audios=[]
if(len(j['response']['items'])!=0):
	for a in j['response']['items']:
		if(a.setdefault('access_key', '')==''):
			audios+=['audio'+str(a['owner_id'])+'_'+str(a['id'])]
		else:
			audios+=['audio'+str(a['owner_id'])+'_'+str(a['id'])+'_'+str(a['access_key'])]
outMsg['attachment']=''
random.shuffle(audios)
for audio in audios:
	outMsg['attachment']+=audio+','
