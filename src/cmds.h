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

#ifndef CMDS_H_INCLUDED
#define CMDS_H_INCLUDED

namespace cmds
{
	void weather(message *inMsg, table *outMsg);
	void con(message *inMsg, table *outMsg);
	#ifndef NO_LIBGD
	void upload(message *inMsg, table *outMsg);
	#endif
	void video(message *inMsg, table *outMsg);
	void f(message *inMsg, table *outMsg);
	void doc(message *inMsg, table *outMsg);
	void set(message *inMsg, table *outMsg);
	void execute(message *inMsg, table *outMsg);
	void info(message *inMsg, table *outMsg);
	void who(message *inMsg, table *outMsg);
	void when(message *inMsg, table *outMsg);
	void botinfo(message *inMsg, table *outMsg);
	void stat(message *inMsg, table *outMsg);
	void square(message *inMsg, table *outMsg);
#ifndef NO_PYTHON
	void py(message *inMsg, table *outMsg);
	void pyinit(message *inMsg, table *outMsg);
#endif
}

#endif
