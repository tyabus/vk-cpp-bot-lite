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
#include <iostream>
#include <ctime>
#include <mutex>

extern json botname;
extern bool friendsadd;
extern bool forwardmessages;

#define vk_version  "5.69"

string vk_token = "";

/* Add stats and check token */
void vk::init()
{
	if (vk_token != "")return;
	json data_temp;
	if (fs::exists(CONFIG))
	{
		data_temp = json::parse(fs::readData(CONFIG));
		vk_token = data_temp["token"];
	}
	auto data = vk::send("stats.trackVisitor");
	if (!data["error"].is_null())
	{
		cout << "invalid vk_token" << endl;
		data_temp["token"] = "put vk token here (vkhost.github.io)";
	}
	if(data_temp["names"].is_null())
        data_temp["names"] =  {"кот", "котейка", ";"};
    botname = data_temp["names"];

    if(data_temp["friendsadd"].is_null())
        data_temp["friendsadd"] =  true;
    friendsadd = data_temp["friendsadd"];

    if(data_temp["forwardmessages"].is_null())
        data_temp["forwardmessages"] =  true;
    forwardmessages = data_temp["forwardmessages"];

    fs::writeData(CONFIG, data_temp.dump(4));
}

mutex l;

/* Send vk request and get response */
nlohmann::json vk::send(string method, table params, bool sendtoken)
{
	l.lock();
	if (sendtoken) {
		params["access_token"] = vk_token;
	}
	if(params.find("v") == params.cend())
		params["v"] = vk_version;
	nlohmann::json request = nlohmann::json::parse(net::send("https://api.vk.com/method/" + method, params));
	unsigned int sleept=0;
	while (!request["error"].is_null() && (request["error"]["error_code"] == 14 || request["error"]["error_code"] == 10))
	{
		sleept+=60000;
		cout << other::getRealTime() + ": VK ERROR: capcha! wait " << sleept << "ms" << endl;
		other::sleep(sleept);
		request = nlohmann::json::parse(net::send("https://api.vk.com/method/" + method, params));
	}
	while (!request["error"].is_null() && request["error"]["error_code"] == 6)
	{
		other::sleep(500);
		request = nlohmann::json::parse(net::send("https://api.vk.com/method/" + method, params));
	}
	if (!request["error"].is_null()) cout << other::getRealTime() + ": VK ERROR:" << endl << request/*.dump(4)*/ << endl;
	l.unlock();
	return request;
}

string vk::upload(string path, string peer_id, string type)
{
	if (!other::getFileSize(path.c_str()))
		return "";
	json res;
	table params =
	{
		{"peer_id", peer_id},
		{"type", type}
	};
	if (type != "audio_message")
		res = vk::send(type + "s.getMessagesUploadServer", params)["response"];
	else
		res = vk::send("docs.getMessagesUploadServer", params)["response"];
	string tmp = net::upload(res["upload_url"], path);
	if (tmp == "" || str::at(tmp, "504 Gateway Time-out"))
	{
		return "";
	}
	res = json::parse(tmp);
	params = {};
	if (type == "photo") {
		params["server"] = to_string(res["server"].get<int>());
		params["photo"] = res["photo"];
		params["hash"] = res["hash"];
		res = vk::send("photos.saveMessagesPhoto", params);
	}
	else
		if (type == "doc" || type == "audio_message") {
			params["file"] = res["file"];
			res = vk::send("docs.save", params)/*["response"]*/;
		}
	if (res["response"].is_null())
	{
		return "";
	}
	if (type == "audio_message")
		type = "doc";
	return type + to_string((int)res["response"][0]["owner_id"]) + "_" + to_string((int)res["response"][0]["id"]);
}

void vk::friends()
{
	while (true)
	{
		json list = vk::send("friends.getRequests", {
			{"need_viewed", "1"}
		})["response"]["items"];
		for (unsigned int i = 0; i < list.size(); i++)
		{
			vk::send("friends.add", {
				{"user_id", to_string((int)list[i])}
			});
		}
		for (int i = 0; i < 10; i++)
		{
			vk::send("account.setOnline");
			other::sleep(60000);
		}
	}
}
