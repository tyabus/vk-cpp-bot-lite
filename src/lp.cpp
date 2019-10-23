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

// longpoll updates
#include "common.h"
#include <iostream>

string server, key, ts, old_ts;

void lp::getServer()
{
	json longpollinfo = vk::send("messages.getLongPollServer");
	server = longpollinfo["response"]["server"];
	key = longpollinfo["response"]["key"];
	if (ts == "")
		ts = to_string((int)longpollinfo["response"]["ts"]);
}

void lp::loop()
{
	getServer();
	while (true)
	{
		try
		{
			table params = {
				{ "key", key },
				{ "ts", ts },
				{ "wait", "90" },//25
				{ "mode", "2" },
				{ "version", "3" },
				{ "act", "a_check" }
			};
			json data = json::parse(net::send("https://" + server, params));
			if (!data["ts"].is_null()) ts = to_string((int)data["ts"]);
			if (!data["failed"].is_null())
			{
				if (!lp::errors(data)) break;
				continue;
			}
			else if (old_ts != ts)
			{
				lp::updates(data["updates"]);
			}
			old_ts = ts;
			other::sleep(50);
		}
		catch (...)
		{
			std::cout << "Error!" << std::endl;
		}
	}
}

bool lp::errors(json lp_data)
{
	switch ((int)lp_data["failed"])
	{
	case 1:
		cout << "LongPoll: " << "Updating ts..." << endl;
		break;
	case 2:
		cout << "LongPoll: " << "Key is out of date" << endl;
		lp::getServer();
		break;
	case 3:
		cout << "LongPoll: " << "Information about the user is lost" << endl;
		lp::getServer();
		break;
	case 4:
		cout << "LongPoll: " << "Invalid api version" << endl;
		return false;
		break;
	default:
		cout << "LongPoll: " << "Reconnect" << endl;
		lp::getServer();
		break;
	}
	return true;
}

void lp::updates(json updates)
{
	for (auto update : updates)
	{
		if (update[0].is_null() || update[1].is_null())continue;
		switch ((int)update[0])
		{
		case 4: // message
			msg::in(update);
			break;
		}
	}
}
