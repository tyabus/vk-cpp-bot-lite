# -*- coding: utf-8 -*-

"""
input messange:
	int chat_id
	int user_id
	int msg_id
	char msg_flags
	string msg
	string lp_msg
	list msg_imgs

modules:
	int user_get(string id)
	user_set(string id, int acess_level)
	acess levels:
		0 ban
		1 user
		2 vip
		3 coder
		4 -
		5 admin
	
	int msg_count()
	int msg_countComplete()
	
output message:
	dict outMsg
	(vk.api-message.send)

other:
	string getStartTime()

vk:
	string vk_upload(string path, string peer_id, string type)
	string vk_send(string method, dict params, bool sendtoken)

net:
	string net_upload(string url, string filename, string params)
	string net_download(string url, string filename, string params)
	string net_send(string url, dict params, bool post_params)
	string net_info <send dl, send ul, upload dl, upload ul, download dl, download ul>

"""
import os
import psutil
import cpuinfo
cpu=cpuinfo.cpuinfo.get_cpu_info()

net_info=net_info.split()
net_info+=[int(net_info[0]) + int(net_info[2]) + int(net_info[4])]
net_info+=[int(net_info[1]) + int(net_info[3]) + int(net_info[5])]
for i in range(8):
 net_info[i]=format(int(net_info[i])/(2**20), '.3f')

outMsg["message"] = "id(пользователь/чат): " + str(user_id) + "/" + str(chat_id) + "\n";
outMsg["message"] += "запущен на: " + os.uname()[3] + "\n";
try:
	outMsg["message"] += "CPU: " + cpu['brand'] + "\n";
except:
	1
for idx, cpu in enumerate(psutil.cpu_percent(interval=1, percpu=True)):
	outMsg["message"] += 'CPU '+str(idx+1)+': '+str(cpu)+'%\n'
mem = psutil.virtual_memory()
swp = psutil.swap_memory()
MB = 2**20
outMsg["message"] += 'RAM: '+str(int((mem.total - mem.available) / MB))+ '/' +str(int(mem.total / MB))+'MB\n'
if(swp.total):
	outMsg["message"] += 'SWAP: '+str(int(swp.used / MB))+ '/' +str(int(swp.total / MB))+'MB\n'
outMsg["message"] += 'Из которых мною сожрано: '+str(int(psutil.Process().memory_info().rss / 1000000 ))+'MB\n'
outMsg["message"] += "Сообщений: " + str(msg_countComplete()) + "/" + str(msg_count()) + "\n"
outMsg["message"] += "Запущен: " + getStartTime() + "\n"
outMsg["message"] += "\nТрафик: \n"
outMsg["message"] += "Запросы: ↑" + net_info[1] + "MB ↓" + net_info[0] + "MB\n"
outMsg["message"] += "Выгрузка: ↑" + net_info[3] + "MB ↓" + net_info[2] + "MB\n"
outMsg["message"] += "Закачка: ↑" + net_info[5] + "MB ↓" + net_info[4] + "MB\n"
outMsg["message"] += "Всего: ↑" + net_info[7] + "MB ↓" + net_info[6] + "MB\n"
