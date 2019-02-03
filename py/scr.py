import os
if not os.path.exists('scr'):
	os.mkdir('scr')
path = 'scr/'+str(msg_id)+'.png'
net_download('https://api.site-shot.com', path, 'url='+msg+'&width=1280&max_height=10000&full_size=1')
outMsg['attachment']=vk_upload(path, outMsg['peer_id'], 'photo')+','