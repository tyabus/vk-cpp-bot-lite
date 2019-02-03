#ifndef MODULES_H_INCLUDED
#define MODULES_H_INCLUDED

namespace module
{
	void start();
	
	namespace user
	{
		void read();
		void save();
		int get(message *inMsg);
		void set(string id, int acess);
	}
}

#endif
