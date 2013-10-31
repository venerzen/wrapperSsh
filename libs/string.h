#ifndef	STRING_H
#define	STRING_H
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <sstream>


class	string	{
	public:
		static	std::vector<std::string>    split(std::string s, char delim);
	protected:
	private:
		static	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
};
#endif
