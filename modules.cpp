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
