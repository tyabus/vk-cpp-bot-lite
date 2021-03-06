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

#define CONFIG "config.json"

#ifndef VK_H_INCLUDED
#define VK_H_INCLUDED

namespace vk
{
	void init();
	json send(string method, table params = {}, bool sendtoken = true);
	string upload(string path, string peer_id, string type);
	void friends();
}

#endif
