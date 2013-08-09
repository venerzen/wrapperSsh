#include "WrapperSshException.h"
#include <iostream>
#include <string>
#include <exception>

wrapperSshException::wrapperSshException(int n,const std::string& e) throw():
		number(n),
		error (e)
		{}
wrapperSshException::~wrapperSshException() throw()	{
}
wrapperSshException::wrapperSshException(const wrapperSshException& c):
		number(c.number),
		error(c.error)
		{}
const	char*	wrapperSshException::what()	const throw()	{
	return	this->error.c_str();
}
const	int	wrapperSshException::getNumber()	const throw()	{
	return	this->number;
}
