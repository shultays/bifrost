#include "gTools.h"

#include <stdio.h>
#include <fstream>
#include <streambuf>

bool textFileRead(char *fileName, std::string &data) {
	std::ifstream t(fileName);
	if (!t.good()) {
		return false;
	}
	t.seekg(0, std::ios::end);
	data.reserve((unsigned int)t.tellg());
	t.seekg(0, std::ios::beg);

	data.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return true;
}