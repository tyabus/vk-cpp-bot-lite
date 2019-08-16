#include "common.h"
#include <unistd.h>
#include <ctime>

void other::sleep(int ms)
{
	usleep(ms * 1000);
}

time_t td;
void other::startTime()
{
	td = time(NULL);
}

void other::cleancmds()
{
	system("rm -f cmd-*");
}

string other::getTime()
{
	string str = ctime(&td);
	str.resize(str.size() - 1);
	return str;
}

string other::getRealTime()
{
	time_t temp = time(NULL);
	string str = ctime(&temp);
	str.resize(str.size() - 1);
	return str;
}

string other::getTime(time_t temp)
{
	struct tm *timeinfo = localtime(&temp);
	char buffer[64];
	strftime(buffer, 64, "%Z %I:%M%p", timeinfo);
	return buffer;
}

string other::getDate(time_t temp)
{
	struct tm *timeinfo = localtime(&temp);
	char buffer[64];
	strftime(buffer, 64, "%A, %d %B %Y", timeinfo);
	return buffer;
}

json other::jsonDifferenceArr(json j1, json j2)
{
	json out;
	for (unsigned int i1 = 0; i1 < j1.size(); i1++)
	{
		bool f = true;
		for (unsigned int i2 = 0; i2 < j2.size(); i2++)
		{
			if (j1[i1] == j2[i2])
			{
				f = false;
				break;
			}
		}
		if (f) out.push_back(j1[i1]);
	}
	return out;
}

void other::fwds(json *in, json *out, unsigned int lvl)
{
	for (auto i : *in)
	{
		json t;
		t["msg"] = i["body"];
		t["user_id"] = i["user_id"];
		t["lvl"] = lvl;
		out->push_back(t);
		if (i["fwd_messages"].is_null())continue;
		other::fwds(&i["fwd_messages"], out, lvl + 1);
	}
}

string other::getParamOfPath(string path, string p)
{
	string dat = fs::readData(path);
	args lines = str::words(dat, '\n');
	for (auto line : lines)
	{
		args param = str::words(line, ':');
		if (str::at(param[0], p))
			return str::replase(param[1], "  ", " ");
	}
	return "";
}

#include <sys/stat.h>
long long int other::getFileSize(const char * fileName)
{
	struct stat file_stat;
	stat(fileName, &file_stat);
	return file_stat.st_size;
}
args other::msgPhotos(message *inMsg)
{
    args out;
    table params =
	{
		{"message_ids", to_string(inMsg->msg_id)}
	};
	json res = vk::send("messages.getById", params)["response"]["items"][0];
	if (res["attachments"].is_null())
	{
        string id = vk::send("users.get", {{"fields", "photo_id"}, {"user_ids", to_string(inMsg->user_id)}})["response"][0]["photo_id"];
        res = vk::send("photos.getById", {{"photos", id},{"photo_sizes", "1"},{"extended", "0"}})["response"];
        for (unsigned i = 0; i < res.size(); i++)
        {
            int maxIndex = 0;
            for (unsigned int si = 0; si < res[i]["sizes"].size(); si++)
                if (res[i]["sizes"][si]["width"] > res[i]["sizes"][maxIndex]["width"])
                    maxIndex = si;
            out.push_back(res[i]["sizes"][maxIndex]["src"]);
        }
		return out;
	}
	params = {};
	json photos;
	string p = "";
	photos =
	{
		{"photos", ""},
		{"photo_sizes", "1"},
		{"extended", "0"}
	};
	for (unsigned i = 0; i < res["attachments"].size(); i++)
	{
		if (res["attachments"][i]["type"] == "photo")
		{
			p += to_string((int)res["attachments"][i]["photo"]["owner_id"]) + "_" + to_string((int)res["attachments"][i]["photo"]["id"]);
			if (!res["attachments"][i]["photo"]["access_key"].is_null())
				p += "_" + res["attachments"][i]["photo"]["access_key"].get<string>();
			p += ",";
		}
	}
	photos["photos"] = p;
	res = vk::send("photos.getById", photos)["response"];
	for (unsigned i = 0; i < res.size(); i++)
	{
		int maxIndex = 0;
		for (unsigned int si = 0; si < res[i]["sizes"].size(); si++)
			if (res[i]["sizes"][si]["width"] > res[i]["sizes"][maxIndex]["width"])
				maxIndex = si;
        out.push_back(res[i]["sizes"][maxIndex]["src"]);
    }
    return out;
}
