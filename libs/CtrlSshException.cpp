#include "CtrlSshException.h"
#include <iostream>
#include <string>
#include <exception>

sshException::sshException(int n,const std::string& e) throw():
		number(n),
		error (e)
		{}
sshException::~sshException() throw()	{
}
sshException::sshException(const sshException& c):
		number(c.number),
		error(c.error)
		{}
const	char*	sshException::what()	const throw()	{
	return	this->error.c_str();
}

