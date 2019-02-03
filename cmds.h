#ifndef CMDS_H_INCLUDED
#define CMDS_H_INCLUDED

namespace cmds
{
	void weather(message *inMsg, table *outMsg);
	void con(message *inMsg, table *outMsg);
	void upload(message *inMsg, table *outMsg);
	void video(message *inMsg, table *outMsg);
	void f(message *inMsg, table *outMsg);
	void doc(message *inMsg, table *outMsg);
	void set(message *inMsg, table *outMsg);
	void execute(message *inMsg, table *outMsg);
	void info(message *inMsg, table *outMsg);
	void who(message *inMsg, table *outMsg);
	void when(message *inMsg, table *outMsg);
#ifndef NO_PYTHON
	void py(message *inMsg, table *outMsg);
#endif
	void ip(message *inMsg, table *outMsg);
#ifndef NO_PYTHON
	void pyinit(message *inMsg, table *outMsg);
#endif
}

#endif
