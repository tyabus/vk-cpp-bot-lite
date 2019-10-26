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
#include <iostream>
#ifndef NO_PYTHON
#include "py.h"
#endif

bool friendsadd;

int main()
{
#ifndef NO_PYTHON
	PyMainThread m;
#endif
	vk::init();
	other::starttime();
#ifndef _WIN32
	other::cleancmds();
#endif
	other::licensenotice();
	module::start();
	cmd::init();
        thread friends(vk::friends);
	msg::init();
	lp::loop();
    	if(friendsadd)
        	friends.join();
	return 0;
}
