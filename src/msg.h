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

#ifndef MSG_H_INCLUDED
#define MSG_H_INCLUDED

void new_msg(json js);

namespace msg
{
	void init();
	void in(json js);
	void treatment(message inMsg);
    	void decode(json js, message *inMsg);
    	void func(message *inMsg, table *outMsg);
    	void send(table outMsg);
    	bool toMe(message *inMsg);
    	void setTyping(string id);
    	unsigned long long int Count();
    	unsigned long long int CountComplete();
};

#endif // MSG_H_INCLUDED
