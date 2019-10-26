/*
vk-cpp-bot-lite
Copyright (C) 2019 tyabus,edombek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"
#include <mutex>
#include <iostream>
#ifndef _WIN32
#include "ThreadPool.h"
#else
#include "./3rd_party/ThreadPool.h"
#endif

bool forwardmessages;
json botname;

ThreadPool pool(MAXTHREADS);
mutex msgLock;
unsigned long long int msgCount = 0;
unsigned long long int msgCountComplete = 0;

void msg::init()
{
	pool.init();
}

void msg::in(json js) {
	message inMsg;
	msg::decode(js, &inMsg);
	msgCount++;
	if (inMsg.msg == "" || (inMsg.flags & 0x02) || inMsg.user_id<0)return;
	if (!msg::toMe(&inMsg))return;
	if (!inMsg.words.size())
		inMsg.words.push_back("help");
	pool.submit(msg::treatment, inMsg);
}

void msg::treatment(message inMsg)
{
	cout << other::getRealTime() + ": start(" + to_string(inMsg.msg_id) + ", " +  to_string(inMsg.user_id) + "/" + to_string(inMsg.chat_id) + "): " + inMsg.words[0] << endl;
	table outMsg = {};
	string id;
	if (inMsg.chat_id)
		id = to_string(inMsg.chat_id + 2000000000);
	else
		id = to_string(inMsg.user_id);

	msg::func(&inMsg, &outMsg);
	msgLock.lock();
	msgCountComplete++;
	msgLock.unlock();
	msg::send(outMsg);
	cout << other::getRealTime() + ": done(" + to_string(inMsg.msg_id) + "): " + inMsg.words[0]  << endl;
}

void msg::decode(json js, message *inMsg)
{
	inMsg->msg_id = js[1];
	inMsg->flags = (int)js[2];
	if (!js[5].is_null())inMsg->msg = js[5];
	if (js[3] > 2000000000)
	{
		inMsg->chat_id = (int)js[3] - 2000000000;
		if (!js[6].is_null())inMsg->user_id = str::fromString(js[6]["from"]);
	}
	else
	{
		inMsg->chat_id = 0;
		inMsg->user_id = js[3];
	}
	inMsg->js = js;
	if (!module::user::get(inMsg))
		inMsg->msg = "";
	if (inMsg->msg == "")return;
	inMsg->msg = str::replase(inMsg->msg, "<br>", " \n");
	inMsg->words = str::words(inMsg->msg, ' ');
}

void msg::func(message *inMsg, table *outMsg)
{
	//outMsg["message"]=inMsg.js.dump(4);
	if (inMsg->chat_id)
		(*outMsg)["peer_id"] = to_string(inMsg->chat_id + 2000000000);
	else
		(*outMsg)["peer_id"] = to_string(inMsg->user_id);
	cmd::start(inMsg, outMsg, inMsg->words[0]);
    if(forwardmessages)
        if (inMsg->chat_id)
            (*outMsg)["forward_messages"] += to_string(inMsg->msg_id);
}

void msg::send(table outMsg)
{
	if (outMsg["peer_id"] == "")return;
	vk::send("messages.send", outMsg);
}

bool msg::toMe(message *inMsg)
{
	for (auto n : botname)
		if (str::low(n) == str::low(inMsg->words[0]))
		{
			inMsg->words.erase(inMsg->words.begin());
			return true;
		}
	if (cmd::easyGet(to_string(inMsg->chat_id) + "_" + to_string(inMsg->user_id)) != "")
		return true;
	if (!inMsg->chat_id)
		return true;
	return false;
}

void msg::setTyping(string id)
{
	vk::send("messages.setActivity", {
		{"peer_id", id},
		{"type", "typing"}
	});
}

unsigned long long int msg::Count()
{
	msgLock.lock();
	unsigned long long int temp = msgCount;
	msgLock.unlock();
	return temp;
}
unsigned long long int msg::CountComplete()
{
	msgLock.lock();
	unsigned long long int temp = msgCountComplete;
	msgLock.unlock();
	return temp;
}
