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
	other::startTime();
	other::cleancmds();
	module::start();
	cmd::init();
        thread friends(vk::friends);
	msg::init();
	lp::loop();
    if(friendsadd)
        friends.join();
	return 0;
}
