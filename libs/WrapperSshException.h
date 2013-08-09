#ifndef WRAPPERSSHPEXCEPTION
#define WRAPPERSSHEXCEPTION
#include <iostream>
#include <string>
#include <exception>

class	wrapperSshException	: public	std::exception	{
	public :
		wrapperSshException(int n=0, const std::string& e="") throw(); 	// Constructeur
		~wrapperSshException() throw();					// Destructeur
		wrapperSshException(const wrapperSshException& c)	;		// Constructeur de copie
		virtual const char* what() const throw();				// Méthode retournant le texte descriptif de l'erreur
		const	int	getNumber() const throw();				// Méthode retournant le code de l'erreur
	protected :
		int		number;		// Numéro de l'erreur
		std::string	error;		// Description de l'erreur retournée
};
#endif
