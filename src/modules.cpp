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
#include <cstdlib>
#include <ctime>
#include <mutex>

void module::start()
{
	srand(time(NULL));
	module::user::read();
}
// user system
#define users_path "users.json"
json users;
mutex uLock;
void module::user::read()
{
	if (fs::exists(users_path))
	{
		users = json::parse(fs::readData(users_path));
	}
	else
	{
		users["default"] = 1;
		users["214707034"] = 5;
		module::user::save();
	}
}
void module::user::save()
{
	fs::writeData(users_path, users.dump(4));
}
int module::user::get(message *inMsg)
{
	int user = 0;
	int chat = 0;
	uLock.lock();
	if (users.find(to_string(inMsg->user_id)) == users.end())
		user = users["default"];
	else
		user = users[to_string(inMsg->user_id)];

	if (users.find(to_string(-inMsg->chat_id)) == users.end())
		chat = users["default"];
	else
		chat = users[to_string(-inMsg->chat_id)];
	uLock.unlock();
	if (chat + user <= 1)
		return 0;
	if (chat > user)
		return chat;
	else
		return user;
}
void module::user::set(string id, int acess)
{
	uLock.lock();
	users[id] = acess;
	if (acess == users["default"])
		users.erase(id);
	module::user::save();
	uLock.unlock();
}
