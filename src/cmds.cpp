// commands here

#include "common.h"
#include <curl/curl.h>
#include <cmath>

#define max_msg_size 4000

void cmds::weather(message *inMsg, table *outMsg)
{
	if (inMsg->words.size() < 2)
	{
		(*outMsg)["message"] += "Введи город или село";
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

#ifndef NO_LIBGD
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
#endif

void cmds::square(message *inMsg, table *outMsg)
{
	if(inMsg->words.size() < 1)
	{
		(*outMsg)["message"] += "...";
		return;
	}

	string words = str::summ(inMsg->words, 1);

	int wordsint = atof(words.c_str());
	(*outMsg)["message"] += std::string("Результат: ") + std::to_string(sqrt(wordsint)) + "\n";
	return;
}

void cmds::botinfo(message *inMsg, table *outMsg)
{
	(*outMsg)["message"] += "vk-cpp-bot-lite сборка " + std::string(__DATE__) + " " + std::string(__TIME__) + "\n";
	(*outMsg)["message"] += "Версия бота: " + std::string(BOT_VERSION) + "\n";
	(*outMsg)["message"] += "Версия компилятора: " + std::string(CXX) + " " + std::string(__VERSION__) + "\n";
	(*outMsg)["message"] += "Максимальное количество потоков: " + std::to_string(MAXTHREADS) + "\n";
	#ifdef NO_PYTHON
	(*outMsg)["message"] += "Python в сборке отсутствует\n";
	#endif // NO_PYTHON
	#ifdef NO_LIBGD
	(*outMsg)["message"] += "LibGD в сборке отсутствует\n";
	#endif // NO_LIBGD
	#ifdef DEBUG
	(*outMsg)["message"] += "Дебаг билд\n";
	#endif // DEBUG
	(*outMsg)["message"] += "Версия nlohmann json: " + std::to_string(NLOHMANN_JSON_VERSION_MAJOR) + "." + std::to_string(NLOHMANN_JSON_VERSION_MINOR) + "." + std::to_string(NLOHMANN_JSON_VERSION_PATCH) + "\n";
	(*outMsg)["message"] += "О libcurl: " + std::string(curl_version()) + "\n";
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
	(*outMsg)["message"] += "Пользователю " + std::string("@id") + inMsg->words[1] + " был изменен уровень доступа на " + inMsg->words[2];
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
	if (inMsg->words.size() < 2 || inMsg->words.size() > 50)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	if (!inMsg->chat_id)
	{
		(*outMsg)["message"] += "Ты не в чате...";
		return;
	}
	table params =
	{
		{"chat_id", to_string(inMsg->chat_id)},
		{"fields", "photo"}
	};
	json res = vk::send("messages.getChatUsers", params)["response"];
	unsigned int i = rand() % res.size();
	(*outMsg)["message"] += "Я считаю, что это [id" + to_string((int)res[i]["id"]) + "|" + res[i]["first_name"].get<string>() + "]";
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
	if (inMsg->words.size() < 2 || inMsg->words.size() > 50)
	{
		(*outMsg)["message"] += "...";
		return;
	}
	unsigned int i = rand() % 100;
	(*outMsg)["message"] += "Вероятность этого равна " + to_string(i) + "%";
}

void cmds::stat(message *inMsg, table *outMsg)
{
	std::chrono::time_point<std::chrono::system_clock> begin, end;
	begin = std::chrono::system_clock::now();
	net::send("http://api.vk.com", "");
	end = std::chrono::system_clock::now();
	unsigned int t = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	(*outMsg)["message"] += "Обработка VK API за: " + std::to_string(t) + "мс\n";
	string myMem = to_string((int)((float)str::fromString(other::getParamOfPath("/proc/self/status", "VmRSS")) / 1024));

	auto net_info = str::words(net::getInfo(), ' ');

	(*outMsg)["message"] += "CPU:" + other::getParamOfPath("/proc/cpuinfo", "model name") + "\n";
	(*outMsg)["message"] += "Потоков занято: " + other::getParamOfPath("/proc/self/status", "Threads") + "\n";
	(*outMsg)["message"] += "Я сожрал оперативы: " + myMem + " Мб\n";
	(*outMsg)["message"] += "Запущен: " + other::getTime() + "\n";
 	(*outMsg)["message"] += "\nТрафик:\n";
	(*outMsg)["message"] += "Запросы: ↑" + net_info[1] + "B ↓" + net_info[0] + "B\n";
	(*outMsg)["message"] += "Выгрузка: ↑" + net_info[3] + "B ↓" + net_info[2] + "B\n";
	(*outMsg)["message"] += "Закачка: ↑" + net_info[5] + "B ↓" + net_info[4] + "B\n";
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

void cmds::pyinit(message *inMsg, table *outMsg)
{
    cmd::init();
    (*outMsg)["message"] = "done";
}
#endif
