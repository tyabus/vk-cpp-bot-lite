#ifndef OTHER_H_INCLUDED
#define OTHER_H_INCLUDED

namespace other
{
	void sleep(int ms);
	void startTime();
	void cleancmds();
	string getTime();
	string getRealTime();
	string getTime(time_t temp);
	string getDate(time_t temp);
	json jsonDifferenceArr(json j1, json j2);
	void fwds(json *in, json *out, unsigned int lvl=0);
	string getParamOfPath(string path, string p);
	long long int getFileSize(const char * fileName);
	args msgPhotos(message *inMsg);
}

#endif
