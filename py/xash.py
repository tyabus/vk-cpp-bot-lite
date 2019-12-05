import os

net_download('https://github.com/FWGS/xash3d-deploy/blob/anewengine-master/xashdroid.apk?raw=true', 'xashdroid.apkk', '')
outMsg['attachment']=vk_upload('xashdroid.apkk', outMsg['peer_id'], 'doc')
outMsg['message']=net_send('https://raw.githubusercontent.com/FWGS/xash3d-deploy/anewengine-master/README.md', {}, 0).split('```')[1]
os.remove('xashdroid.apkk')
