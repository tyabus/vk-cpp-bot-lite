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
#ifndef NO_PYTHON
#include "py.h"
#endif
cmd::cmd_table cmd_d;

/*
права доступа:
0 в бане
1 пользователь
2 доверенный
3 кодер
4
5 админ
*/

void help(message *inMsg, table *outMsg)
{
	(*outMsg)["message"] += "Твой уровень доступа: " + to_string(module::user::get(inMsg)) + "\n";
	(*outMsg)["message"] += "Список команд:\n" + cmd::helpList(inMsg) + "\n";
}

void cmd::init()
{
	cmd_d = {};

	cmd::add("help", &help, false, "help", 0, 1);
	cmd::add("погода", &cmds::weather, true, "погода в указанном городе/селе", 0, 1);
	cmd::add("con", &cmds::con, true, "системная консоль", 0, 5);
	cmd::add("u", &cmds::upload, true, "выгрузить в аттачмент", 0, 3);
	cmd::add("видео", &cmds::video, false, "видео по запросу", 0, 1);
	cmd::add("f", &cmds::f, false, "видосы с правками)", 0, 2);
	cmd::add("доки", &cmds::doc, true, "поиск документов", 0, 2);
	cmd::add("set", &cmds::set, true, "access set", 0, 5);
	cmd::add("exe", &cmds::execute, true, "vk api", 0, 5);
	cmd::add("оботе", &cmds::botinfo, true, "о боте", 0, 1);
	cmd::add("корень", &cmds::square, true, "корень указанного числа", 0, 1);
	cmd::add("кто", &cmds::who, true, "рандом в чате", 0, 1);
	cmd::add("когда", &cmds::when, true, "когда что либо произойдёт", 0, 1);
	cmd::add("инфа", &cmds::info, true, "вероятность чего либо", 0, 1);
	cmd::add("i", &cmds::stat, true, "статистика бота", 0, 1);

#ifndef NO_PYTHON
	cmd::add("pyinit", &cmds::pyinit, true, "re init py cmds", 0, 5);
	cmd::add("py", &cmds::py, true, "python 3", 0, 3);
	//py init
	PySubThread sub;
	try
	{
		py::object main_module = py::import("__main__");
		py::object main_namespace = main_module.attr("__dict__");
		main_module.attr("init") = cmd::pyAdd;
		py::exec(py::str(fs::readData("py/init.py")), main_namespace);
	}
	catch(py::error_already_set const &)
	{
		PyErr_Print();
	}
#endif
}

void cmd::add(string command, cmd::msg_func func, bool disp, string info, int cost, int acess)
{
	cmd_d[str::low(command)].ex.func = func;
	cmd_d[str::low(command)].ex.pyPath = "";
	cmd_d[str::low(command)].disp = disp;
	cmd_d[str::low(command)].info = info;
	cmd_d[str::low(command)].acess = acess;
}

#ifndef NO_PYTHON
void cmd::pyAdd(string command, string pyPath, bool disp, string info, int cost, int acess)
{
	cmd_d[str::low(command)].ex.func = NULL;
	cmd_d[str::low(command)].ex.pyPath = pyPath;
	cmd_d[str::low(command)].disp = disp;
	cmd_d[str::low(command)].info = info;
	cmd_d[str::low(command)].acess = acess;
}
#endif

void cmd::start(message *inMsg, table *outMsg, string command)
{
	if (str::low(command) == "idkfa" || str::low(command) == "idfa" )
	{
		(*outMsg)["message"] += "very happy ammo added";
		return;
	}

	if (module::user::get(inMsg) < 2)
	{
		inMsg->msg = str::replase(inMsg->msg, ". ", "@#$%&");
		inMsg->msg = str::replase(str::replase(inMsg->msg, "&#", "-"), ".", "-");
		inMsg->msg = str::replase(inMsg->msg, "@#$%&", ". ");
	}

	string t = cmd::easyGet(to_string(inMsg->chat_id) + "_" + to_string(inMsg->user_id));
	if (t != "" && cmd_d.find(command) == cmd_d.cend())
	{
		command = t;
		args temp;
		temp.push_back(t);
		for (auto ar : inMsg->words)
			temp.push_back(ar);
		inMsg->words = temp;
	}
	else
		command = str::low(command);
	if (cmd_d.find(command) != cmd_d.cend())
	{
		if (module::user::get(inMsg) < cmd_d[command].acess)
		{
			(*outMsg)["message"] += "Ваш уровень доступа не позволяет выполнить данную команду";
			return;
		}
		if(cmd_d[command].ex.func!=NULL)
			cmd_d[command].ex.func(inMsg, outMsg);
#ifndef NO_PYTHON
		else
		{
			//py execute script
			PySubThread sub;
			py::object main_module = py::import("__main__");
			py::object main_namespace = main_module.attr("__dict__");
			main_module.attr("outMsg") = pyF::toPythonDict(*outMsg);
			main_module.attr("chat_id") = inMsg->chat_id;
			main_module.attr("user_id") = inMsg->user_id;
			main_module.attr("msg_id") = inMsg->msg_id;
			main_module.attr("msg_flags") = inMsg->flags;
			main_module.attr("msg") = str::summ(inMsg->words, 1);
			main_module.attr("lp_msg") = inMsg->js.dump(4);
			main_module.attr("user_set") = module::user::set;
			main_module.attr("user_get") = pyF::user_get;
			main_module.attr("msg_count") = msg::Count;
			main_module.attr("msg_countComplete") = msg::CountComplete;
			main_module.attr("getStartTime") = pyF::getTime;
			main_module.attr("vk_upload") = vk::upload;
			main_module.attr("vk_send") = pyF::vk_send;
			main_module.attr("net_send") = pyF::net_send;
			main_module.attr("net_upload") = net::upload;
			main_module.attr("net_download") = net::download;
			main_module.attr("net_info") = net::getInfo();
			main_module.attr("msg_imgs") = pyF::toPythonList(other::msgPhotos(inMsg));
			try
			{
				py::exec(py::str(fs::readData("py/" + cmd_d[command].ex.pyPath)), main_namespace);
				*outMsg = pyF::toTable(py::extract<py::dict>(main_module.attr("outMsg")));
			}
			catch(py::error_already_set&)
			{
				(*outMsg)["message"]+=pyF::error();
			}
		}
#endif
	}
	else
	{
		(*outMsg)["message"] = (*outMsg)["message"] += "нет такой команды, введите команду help и уточните";
	}
	return;
}

string cmd::helpList(message *inMsg)
{
	string out = "";
	for (auto cmds : cmd_d)
	{
		if (!cmds.second.disp&&module::user::get(inMsg) != 5)continue;
		if (module::user::get(inMsg) < cmds.second.acess) continue;
		out += "\n - \"";
		out += str::low(cmds.first);
		out += "\" - ";
		out += cmds.second.info;
		if (cmds.second.cost)
		{
			out += " - ";
			out += to_string(cmds.second.cost);
			out += "$";
		}
	}
	return out;
}

map<string, string> easyCmd;
mutex easyLock;

void cmd::easySet(string id, string cmd)
{
	easyLock.lock();
	if (cmd == "")
		easyCmd.erase(id);
	else
		easyCmd[id] = cmd;
	easyLock.unlock();
}

string cmd::easyGet(string id)
{
	string t;
	easyLock.lock();
	if (easyCmd.find(id)->first != "")
		t = easyCmd[id];
	else
		t = "";
	easyLock.unlock();
	return t;
}
