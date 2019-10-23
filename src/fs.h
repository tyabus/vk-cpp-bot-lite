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

#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED
#include <fstream>

#define FS_READ ios::in
#define FS_WRITE ios::out
#define FS_BINARY ios::binary

namespace fs
{
	class file
	{
	private:
		fstream _stream;
	public:
		fstream* getStream();
		void write(string data);
		void writeBinary(const char *data, size_t size);
		string read();
		void readBinary(char *data, size_t size);
		void close();
	};

	class exception
	{
		string what_str;
	public:
		exception(string what);
		string what();
	};

	bool exists(std::string path);

	file *open(std::string path, int flags = FS_READ | FS_WRITE);

	void writeData(string path, string data);
	void safeWriteData(string path, string data);
	string readData(string path);
	string getRootPath();
	void rename(string oldname, string newname);
}

#endif
