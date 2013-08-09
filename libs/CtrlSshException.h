#ifndef CTRLSSHEXCEPTION
#define CTRLSSHEXCEPTION
#include <iostream>
#include <string>
#include <exception>
/**
*       \class		sshException
*       \ingroup        sshTools
*       \brief		Exceptions liées au CtrlSsh
*
*       Cette est une simple spécification de la classe std::exception. Elle permet de gérer des exceptions propre au CtrlSsh.
*
*       \author (dernier à l'avoir modifié) $Author: Kuhn Frédéric $
*
*       \version $Revision: 1 $
*
*       \date $Date: 2010/12/20 15:52:00 $
*
*       Contact: fkuhn@plurimedia.fr
*
*       Created on: 2010/12/20 15:52:00
*
* @{
*/

class	sshException	: public	std::exception	{
	public :
			/*!
			*	\brief  Constructeur de la classe.
			*	\param	int \b n : code de l'erreur jetée.
			*	\param	const std::string& \b e : message correspondant au code.
			*	\return void
			*
			*	Cette méthode est le constructeur de cette classe. C'est lui qui est directement appelé lors d'un « throw ».
			*
			*/
		sshException(int n=0, const std::string& e="") throw(); 
			/*!
			*	\brief  Destructeur par défaut.
			*	\return void
			*
			*	Cette méthode est le destructeur par défaut de cette classe.
			*
			*/
		~sshException() throw();
			/*!
			*	\brief  Constructeur de copie de la classe.
			*	\param	const sshException& \b c : Exception dont on va copier les paramètres.
			*	\return void
			*
			*	Cette méthode est le constructeur de copie de cette classe.
			*
			*/
		sshException(const sshException& c);	
			/*!
			*	\brief  Méthode d'affichage de l'erreur.
			*	\return char* : message formaté.
			*
			*	Cette méthode retourne le texte descriptif de l'erreur.
			*
			*/
		virtual const char* what() const throw();			
	protected :
		/*!
		*	Cette variable correspond au code de l'erreur. Celle-ci peut à l'heure actuelle prendre pour valeur :
		*		- 1 : description du type de l'erreur.
		*/
		int		number;		
		/*!
		*	Description de l'erreur retournée.
		*/
		std::string	error;	
};
#endif

/**
*	Fermeture du groupe Doxygen
*	@}
*/

