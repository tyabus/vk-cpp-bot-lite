import cv2, os
import numpy as np
from PIL import Image
from threading import Thread

faceCascade = cv2.CascadeClassifier("py/haarcascade_frontalface_default.xml")

if not os.path.exists('face'):
	os.mkdir('face')
if not os.path.exists('face/imgs'):
	os.mkdir('face/imgs')
if not os.path.exists('face/faces'):
	os.mkdir('face/faces')

def f():
	outMsg['attachment']=''
	for url in msg_imgs:
		name = 'face/imgs/'+str(msg_id)+'_'+url.split('/')[-1]
		net_download(url, name, '')
		img = Image.open(name)
		image = np.array(img.convert('L'), 'uint8')
		faces = faceCascade.detectMultiScale(image, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30),flags=cv2.CASCADE_SCALE_IMAGE)
		i=0
		for (x, y, w, h) in faces:
			img.crop((x, y, x+w, y+h)).save('face/faces/'+str(i)+'-'+str(msg_id)+'.jpg', 'JPEG')
			outMsg['attachment']+=vk_upload('face/faces/'+str(i)+'-'+str(msg_id)+'.jpg', outMsg['peer_id'], 'photo')+','
			i+=1

thread = Thread(target=f, args=())
thread.daemon = True
thread.start()