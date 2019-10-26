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

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include <map>
#include <vector>
#include <string>
#include <thread>

using namespace std;

#ifdef _WIN32
#define NO_PYTHON // If someone try to compile this using mingw
#define BOT_VERSION "unknown-win32"
#include "./3rd_party/json.hpp"
#include "../win32deps/include/curl/curl.h"
#else
#include <curl/curl.h>
#include "json.hpp"
#endif

using json = nlohmann::json;

typedef vector<string> args;
typedef map<string, string> table;
typedef struct
{
	json js;
	char flags;
	int msg_id;
	int user_id;
	string msg;
	int chat_id;
	args words;
}message;

#include "fs.h"
#include "net.h"
#include "vk.h"
#include "lp.h"
#include "msg.h"
#include "str.h"
#include "other.h"
#include "modules.h"
#include "cmd.h"
#include "cmds.h"

#define MAXTHREADS 5
#endif
