#include	"string.h"
std::vector<std::string>    string::split(std::string s, char delim)	{
	std::vector<std::string> elems;
	string::split(s, delim, elems);
	return	elems;
}

std::vector<std::string> &string::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
