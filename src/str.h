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

#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

namespace str
{
	long long int fromString(std::string s);
	args words(const std::string& s, char delim = ' ');
	string summ(args words, unsigned int s);
	bool at(string str1, string str2);
	string low(string str);
	string replase(string str, string findstr, string replasestr);
	string convertHtml(string str);
}

#endif
