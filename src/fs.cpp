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

#include "common.h"
#include <stdio.h>

bool fs::exists(string path)
{
	std::ifstream fin;
	fin.open(path);
	if (fin.is_open())
	{
		fin.close();
		return true;
	}
	return false;
}

/* --------- FILE CLASS ---------*/

fs::file *fs::open(std::string path, int flags)
{
	file *_file = new file();
	_file->getStream()->open(path, (std::ios_base::openmode)flags);
	return _file;
}

void fs::file::close()
{
	this->_stream.close();
	delete this;
}

string fs::file::read()
{
	string data;
	string line;
	while (std::getline(this->_stream, line))
	{
		data += line;
		data += "\n";
	}
	if (data.size())
		data.resize(data.size() - 1);
	return data;
}

void fs::file::readBinary(char *data, size_t size)
{
	this->_stream.read(data, size);
}

fstream * fs::file::getStream()
{
	return &this->_stream;
}

void fs::file::write(string data)
{
	this->_stream << data;
}

void fs::file::writeBinary(const char *data, size_t size)
{
	this->_stream.write(data, size);
}

/* --------- /FILE CLASS/ ---------*/

void fs::writeData(string path, string data)
{
	if (fs::file *file = fs::open(path, FS_WRITE))
	{
		file->write(data);
		file->close();
	}
}

void fs::safeWriteData(string path, string data)
{
	fs::writeData(path + ".safe", data);
	remove(path.c_str());
	fs::rename(path + ".safe", path);
}

string fs::readData(string path)
{
	string data;
	fs::file *in = fs::open(path);
	data = in->read();
	in->close();
	return data;
}

void fs::rename(string oldname, string newname)
{
	std::rename(oldname.c_str(), newname.c_str());
}

/* CLASS EXCEPTION */
fs::exception::exception(string what)
{
	this->what_str = what;
}

string fs::exception::what()
{
	return this->what_str;
}
