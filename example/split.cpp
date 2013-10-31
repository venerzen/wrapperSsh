#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>    /* for getopt */
#include <vector>
#include <fstream>
#include <sstream>
#include "libs/string.h"

int	main (int argc, char* argv[])	{
	int	rc	= EXIT_SUCCESS;	//	return code
	std::string	list	= "publickey,keyboard-interactive";
	char	delim	= ',';
	std::vector<std::string> elems	= string::split(list,delim);
	std::cout << "Nb de trucs : " << elems.size() << std::endl;
	
	return	rc;
}
