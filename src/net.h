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

#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

namespace net
{
	void init ();
	string urlEncode(string str);
	string send(string url, string params);
	string send(string url, table param = {}, bool post = true);
	string upload(string url, string filename, string params = "");
	void download(string url, string filename, string params = "");
	string getInfo();
}

#endif
