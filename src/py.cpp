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
#include "py.h"

py::dict pyF::toPythonDict(table map) {
	typename table::iterator iter;
	py::dict dictionary;
	for (iter = map.begin(); iter != map.end(); ++iter) {
		dictionary[iter->first] = iter->second;
	}
	return dictionary;
}

table pyF::toTable(py::dict dict)
{
	table dic;
	for(unsigned int i =0; i < py::len(dict.keys()); i++)
		dic[py::extract<string>(dict.keys()[i])] = py::extract<string>(dict[dict.keys()[i]]);
	return dic;
}

py::list pyF::toPythonList(args a)
{
    py::list out;
    for(auto s: a)
        out.append(s);
    return out;
}

string pyF::getTime()
{
	return other::getTime();
}

string pyF::vk_send(string method, py::dict params, bool sendtoken)
{
	return vk::send(method, pyF::toTable(params), sendtoken).dump(4);
}

string pyF::net_send(string url, py::dict param, bool post)
{
	return net::send(url, pyF::toTable(param), post);
}

int pyF::user_get(int id)
{
	message m;
	m.user_id=id;
	m.chat_id=0;
	return module::user::get(&m);
}

#include <iostream>
// decode a Python exception into a string
string pyF::error()
{
	PyObject *exc, *val, *tb;
    PyErr_Fetch(&exc, &val, &tb);
    PyErr_NormalizeException(&exc, &val, &tb);
    py::handle<> hexc(exc), hval(py::allow_null(val)), htb(py::allow_null(tb));
    py::object format_exception = py::import("traceback").attr("format_exception");
    return py::extract<string>(py::str("").join(format_exception(hexc, hval, htb)));
}
