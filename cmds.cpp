// commands here

#include "common.h"
#include <gd.h>
#include <ctime>
#include <random>
#include <mutex>

#define max_msg_size 4000

void cmds::weather(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "Введи город/село";
		return;
	}
	table params =
	{
		{"lang", "ru"},
		{"units", "metric"},
		{"APPID", "ef23e5397af13d705cfb244b33d04561"},
		{"q", str::summ(inMsg->words, 1)}
	};
	json weather = json::parse(net::send("http://api.openweathermap.org/data/2.5/weather", params, false));
	if (weather["main"].is_null())
	{
		(*outMsg)["message"] += "Что то пошло не так, возможно надо ввести город транслитом";
		return;
	}
	string temp = "";
	temp += "погода в " + weather["sys"]["country"].get<string>() + "/" + weather["name"].get<string>() + ":";
	temp += "\n¤" + other::getTime(weather["dt"]) + ":\n";
	temp += "\n•температура: " + to_string((int)weather["main"]["temp"]) + "°C\n•скорость ветра: " + to_string((int)weather["wind"]["speed"]) + "м/с\n•влажность: " + to_string((int)weather["main"]["humidity"]) + "%\n•описание: " + weather["weather"][0]["description"].get<string>() + "\n";
	(*outMsg)["message"] += temp;
}

void cmds::con(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	string cmd = str::summ(inMsg->words, 1);
	cmd = str::convertHtml(cmd);
	string comand = "chmod +x cmd-"+to_string(inMsg->msg_id)+".sh";
	fs::writeData(string("cmd-"+to_string(inMsg->msg_id)+".sh"), cmd);
	system(comand.c_str());
	comand = "bash ./cmd-"+to_string(inMsg->msg_id)+".sh > cmd-"+to_string(inMsg->msg_id)+" 2>&1";
	system(comand.c_str());
	cmd = fs::readData(string("cmd-"+to_string(inMsg->msg_id)));
	comand = "rm -rf cmd-"+to_string(inMsg->msg_id)+" cmd-"+to_string(inMsg->msg_id)+".sh";
	system(comand.c_str());
	string temp = "";
	args out;
	for (unsigned i = 0; i < cmd.size(); i++)
	{
		temp.push_back(cmd[i]);
		if (temp.size() > max_msg_size && (cmd.size() > i + 1 && cmd[i + 1] != '\n'))
		{
			out.push_back(temp);
			temp = "";
		}
	}
	out.push_back(temp);
	temp = "";
	for (unsigned i = 0; i < out.size(); i++)
	{
		if (out.size() > 1)
			(*outMsg)["message"] += "(" + to_string(i + 1) + "/" + to_string(out.size()) + ")\n";
		(*outMsg)["message"] += "\n" + out[i];
		if (out.size() == 1 || i == out.size() - 1)break;
		msg::send((*outMsg));
		other::sleep(10000);
		(*outMsg)["message"] = "";
	}
}

void cmds::upload(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	string url = str::summ(inMsg->words, 2);
	string name = inMsg->words[1];
	if (inMsg->words.size() > 2)
		net::download(url, name);
	(*outMsg)["attachment"] += "," + vk::upload(name, (*outMsg)["peer_id"], "doc");
	if (inMsg->words.size() > 2) {
		name = "rm -f " + name;
		system(name.c_str());
	}
	return;
}

void cmds::video(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "Я что Ванга?";
		return;
	}
	table params =
	{
		{"q", str::summ(inMsg->words, 1)},
		{"adult", "0"},
		{"count", "200"},
		{"hd", "1"},
		{"sort", "2"}
	};
	json res = vk::send("video.search", params)["response"]["items"];
	args videos;
	for (unsigned int i = 0; i < res.size(); i++)
	{
		if (res[i]["id"].is_null()) continue;
		string temp = "";
		temp += "video";
		temp += to_string((int)res[i]["owner_id"]);
		temp += "_";
		temp += to_string((int)res[i]["id"]);
		videos.push_back(temp);
	}
	if (videos.size() == 0)
	{
		(*outMsg)["message"] += "нету такого";
		return;
	}
	(*outMsg)["attachment"] = "";
	unsigned int index = 0;
	if (videos.size() > 10)
		index = rand() % (videos.size() - 10);
	for (unsigned int i = index; i < videos.size(); i++)
	{
		(*outMsg)["attachment"] += videos[i];
		(*outMsg)["attachment"] += ",";
	}
	(*outMsg)["message"] += "вот\nвсего:";
	(*outMsg)["message"] += to_string(videos.size());
}

void cmds::f(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "Я что Ванга?";
		return;
	}
	table params =
	{
		{"q", str::summ(inMsg->words, 1)},
		{"adult", "1"},
		{"count", "100"},
		{"offset", "0"},
		{"hd", "1"},
		{"sort", "2"}
	};
	json res1 = vk::send("video.search", params)["response"]["items"];
	params["adult"] = "0";
	json res2;
	for (int i = 0; i < 1000; i += 200)
	{
		params["offset"] = to_string(i);
		json temp = vk::send("video.search", params)["response"]["items"];
		for (unsigned int t = 0; t < temp.size(); t++)
			res2.push_back(temp[i]);
	}
	json res = other::jsonDifferenceArr(res1, res2);
	args videos;
	for (unsigned int i = 0; i < res.size(); i++)
	{
		if (res[i]["id"].is_null()) continue;
		string temp = "";
		temp += "video";
		temp += to_string((int)res[i]["owner_id"]);
		temp += "_";
		temp += to_string((int)res[i]["id"]);
		videos.push_back(temp);
	}
	if (videos.size() == 0)
	{
		(*outMsg)["message"] += "нету такого";
		return;
	}
	(*outMsg)["attachment"] = "";
	unsigned int index = 0;
	if (videos.size() > 10)
		index = rand() % (videos.size() - 10);
	for (unsigned int i = index; i < videos.size(); i++)
	{
		(*outMsg)["attachment"] += videos[i];
		(*outMsg)["attachment"] += ",";
	}
	(*outMsg)["message"] += "вот\nвсего:";
	(*outMsg)["message"] += to_string(videos.size());
}

void cmds::doc(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "Я что Ванга?";
		return;
	}
	table params =
	{
		{"q", str::summ(inMsg->words, 1)},
		{"count", "1000"}
	};
	json res = vk::send("docs.search", params)["response"]["items"];
	args docs;
	for (unsigned int i = 0; i < res.size(); i++)
	{
		if (res[i]["id"].is_null()) continue;
		string temp = "";
		temp += "doc";
		temp += to_string((int)res[i]["owner_id"]);
		temp += "_";
		temp += to_string((int)res[i]["id"]);
		docs.push_back(temp);
	}
	if (docs.size() == 0)
	{
		(*outMsg)["message"] += "нету такого";
		return;
	}
	(*outMsg)["attachment"] = "";
	unsigned int index = 0;
	if (docs.size() > 10)
		index = rand() % (docs.size() - 10);
	for (unsigned int i = index; i < docs.size(); i++)
	{
		(*outMsg)["attachment"] += docs[i];
		(*outMsg)["attachment"] += ",";
	}
	(*outMsg)["message"] += "воть\nвсего:";
	(*outMsg)["message"] += to_string(docs.size());
}

void cmds::set(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 3)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	module::user::set(inMsg->words[1], str::fromString(inMsg->words[2]));
}

void cmds::execute(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	string cmd = str::summ(inMsg->words, 1);
	cmd = str::convertHtml(cmd);
	table params =
	{
		{"code", cmd}
	};
	json res = vk::send("execute", params);
	string temp = "";
	string resp = res.dump(4);
	args out;
	for (unsigned i = 0; i < resp.size(); i++)
	{
		temp.push_back(resp[i]);
		if (temp.size() > max_msg_size && (resp.size() > i + 1 && resp[i + 1] != '\n'))
		{
			out.push_back(temp);
			temp = "";
		}
	}
	out.push_back(temp);
	temp = "";
	for (unsigned i = 0; i < out.size(); i++)
	{
		if (out.size() > 1)
			(*outMsg)["message"] += "(" + to_string(i + 1) + "/" + to_string(out.size()) + ")\n";
		(*outMsg)["message"] += "\n" + out[i];
		if (out.size() == 1 || i == out.size() - 1)break;
		msg::send((*outMsg));
		(*outMsg)["message"] = "";
	}
}

void cmds::who(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	if (!inMsg->chat_id)
	{
		(*outMsg)["message"] += "ты не в чате...";
		return;
	}
	table params =
	{
		{"chat_id", to_string(inMsg->chat_id)},
		{"fields", "photo"}
	};
	json res = vk::send("messages.getChatUsers", params)["response"];
	unsigned int i = rand() % res.size();
	string who = str::summ(inMsg->words, 1);
	if (who[who.size() - 1] == '?')
		who.resize(who.size() - 1);
	(*outMsg)["message"] += "Я считаю, что " + who + " - [id" + to_string((int)res[i]["id"]) + "|" + res[i]["first_name"].get<string>() + "]";
}

void cmds::when(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	string when = str::summ(inMsg->words, 1);
	if (when[when.size() - 1] == '?')
		when.resize(when.size() - 1);
	(*outMsg)["message"] += "Я считаю, что " + when + " произойдёт " + other::getDate(time(NULL) + rand() % 100000000);
}

void cmds::info(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	unsigned int i = rand() % 130;
	if (i > 100)
		i = (i - 100) * 10;
	string info = str::summ(inMsg->words, 1);
	(*outMsg)["message"] += "Вероятность того, что " + info + " - " + to_string(i) + "%";
}

#ifndef NO_PYTHON
#include "py.h"
void cmds::py(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	string cmd = str::summ(inMsg->words, 1);
	cmd = str::convertHtml(cmd);
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
	py::exec("import sys", main_namespace);
	py::exec("from io import StringIO", main_namespace); //python3
	py::exec("sys.stdout = mystdout = StringIO()", main_namespace);
	try
	{
		fs::writeData(to_string(inMsg->msg_id)+".py", cmd);
		py::exec_file(py::str(to_string(inMsg->msg_id)+".py"), main_namespace);
		*outMsg = pyF::toTable(py::extract<py::dict>(main_module.attr("outMsg")));
		py::exec("output = str(mystdout.getvalue())", main_namespace);
		cmd = py::extract<string>(main_module.attr("output"));
	}
	catch(py::error_already_set&)
	{
		string err = pyF::error();
		py::exec("output = str(mystdout.getvalue())", main_namespace);
		cmd = py::extract<string>(main_module.attr("output"));
		cmd += "\n" + err;
	}
	system(string("rm -rf "+to_string(inMsg->msg_id)+".py").c_str());

	string temp = "";
	args out;
	for (unsigned i = 0; i < cmd.size(); i++)
	{
		temp.push_back(cmd[i]);
		if (temp.size() > max_msg_size && (cmd.size() > i + 1 && cmd[i + 1] != '\n'))
		{
			out.push_back(temp);
			temp = "";
		}
	}
	out.push_back(temp);
	temp = "";
	for (unsigned i = 0; i < out.size(); i++)
	{
		if (out.size() > 1)
			(*outMsg)["message"] += "(" + to_string(i + 1) + "/" + to_string(out.size()) + ")\n";
		(*outMsg)["message"] += "\n" + out[i];
		if (out.size() == 1 || i == out.size() - 1)break;
		msg::send((*outMsg));
		(*outMsg)["message"] = "";
	}
}
#endif

int delta(gdImagePtr im, int x, int y, int r)
{
	int rr = r*r;
	long long summ[3] = { 0, 0, 0 };
	int colors[3];
	unsigned int c = 0;
	for (int xc = x - r; xc <= x + r; xc++)
	{
		for (int yc = y - r; yc <= y + r; yc++)
		{
			if (rr >= (xc - x)*(xc - x) + (yc - y)*(yc - y) && xc >= 0 && yc >= 0 && xc < im->sx&&yc < im->sy)
			{
				int color = gdImageGetTrueColorPixel(im, xc, yc);
				summ[0] += gdTrueColorGetRed(color);
				summ[1] += gdTrueColorGetGreen(color);
				summ[2] += gdTrueColorGetBlue(color);
				c++;
			}
		}
	}
	int color = gdImageGetTrueColorPixel(im, x, y);
	colors[0] = gdTrueColorGetRed(color) - summ[0] / c;
	colors[1] = gdTrueColorGetGreen(color) - summ[1] / c;
	colors[2] = gdTrueColorGetBlue(color) - summ[2] / c;
	for (int i = 0; i < 3; i++)
		if (colors[i] < 0)colors[i] = 0;
	return gdImageColorClosest(im, colors[0], colors[1], colors[2]);
}

#ifndef NO_PYTHON
void cmds::pyinit(message *inMsg, table *outMsg)
{
    cmd::init();
    (*outMsg)["message"] = "done";
}
#endif
