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

#ifndef CMD_H_INCLUDED
#define CMD_H_INCLUDED

namespace cmd
{
	typedef void(*msg_func)(message *inMsg, table *outMsg); // func(inMsg, outMsg)
	typedef struct
	{
		cmd::msg_func func;
		string pyPath;
	}ex_t;
	typedef struct
	{
		string info;
		ex_t ex;
		int cost; // nothing
		int acess;
		bool disp;
	}cmd_dat;
	typedef map<string, cmd_dat> cmd_table;

	void init();
	void add(string command, cmd::msg_func func, bool disp=false, string info="", int cost=0, int acess=1);
	#ifndef NO_PYTHON
	void pyAdd(string command, string pyPath, bool disp=false, string info="", int cost=0, int acess=1);
	#endif
	void start(message *inMsg, table *outMsg, string command);
	string helpList(message *inMsg);
    	void easySet(string id, string cmd);
    	string easyGet(string id);
}

#endif
