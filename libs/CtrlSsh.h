#ifndef	CTRLSSH_H
#define	CTRLSSH_H
#include <iostream>
#include <string>
#include "CtrlSshException.h"
#include "WrapperSsh.h"
/*!	\mainpage Wrapper SSH en c++ de la libssh2
*
*	\section intro_sec Introduction
*
*	Cette librairie à simplement pour but de permettre d'utiliser simplement des fonctionnalitées comme le scp, l'exécution d'une commande via ssh sur un serveur distant, etc…
*
*	\section install_sec Installation
*
*	Nécessite gcc et la libssh2.
*
*	\subsection step1 Step 1: Compiler 

*	Compiler simplement via make. Il y a normalement un makefile pour mac osx, linux et freebsd.  
*
*/
/**
*       \class		ctrlSsh::ctrlSsh
*       \ingroup        sshTools
*       \brief  Contrôleur de connexion.
*
*       Cette classe est le contrôleur permettant d'effectuer des actions via le la wrapper ssh.
*
*       \namespace      ctrlSsh
*
*       \brief  Groupe de classes permettant de gérer les actions ssh via un wrapper de la libSsh2.
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

namespace	ctrlSsh	{
	class	ctrlSsh	{
		public:
				/*!
				*	\brief  Constructeur par défaut. 
				*	\return void
				*
				*	Constructeur par défaut appelé à chaque création d'une instance de ce contrôleur.
				*
				*/
			ctrlSsh();
				/*!
				*	\brief  Constructeur de copie.
				*	\return void
				*
				*	Constructeur de copie.
				*
				*/
			ctrlSsh(const ctrlSsh& r);	
				/*!
				*	\brief  Destructeur par défaut. 
				*	\return void
				*
				*	Destructeur par défaut appelé à chaque destruction d'une instance de ce contrôleur.
				*
				*/
			~ctrlSsh();					
				/*!
				*	\brief  Méthode permettant d'exécuter une commande via une connexion ssh.
				*	\param	const std::string& \b host : adresse ip ou le nom dns de l'hôte.
				*	\param	const std::string& \b user : login de l'utilisateur utilisé pour se connecter à la machine en ssh.
				*	\param	const std::string& \b password : mot de passe utilisé pour authentifier l'utilisateur.
				*	\param	const std::string& \b command : commande a éxécuter sur le serveur distant.
				*	\return wrapperSsh::sshExecResult : Structure permettant de récupérer la sortie standard et la sortie d'erreur.
				*
				*	Cette méthode permet de se connecter à un serveur distant, d'exécuter une commande et de récupérer un contenu dans une structure.
				*
				*/
			wrapperSsh::sshExecResult	execTask(	const std::string& host,
									const std::string& user,
									const std::string& password,
									const std::string& command) throw(sshException);
				/*!
				*	\brief  Méthode permettant de récupérer un fichier sur le serveur distant
				*	\param	const std::string& \b host : adresse ip de l'hôte, je n'ai pas réussi à faire fonctionner la résolution des noms encore.
				*	\param	const std::string& \b user : login de l'utilisateur utilisé pour se connecter à la machine en ssh.
				*	\param	const std::string& \b password : mot de passe utilisé pour authentifier l'utilisateur.
				*	\param	const std::string& \b fileSrc : chemin complet vers le fichier se trouvant sur le serveur.
				*	\param	const std::string& \b fileDest : chemin complet vers le fichier allant être stocké sur la machine éxécutant le script.
				*	\return int : code d'erreur ou de réussite sous la forme d'un int.
				*
				*	Cette méthode permet de se connecter à un serveur distant, d'y récupérer un fichier et de le déposer à l'endroit désiré.
				*
				*/
			int	scpGet(	const std::string& host,
					const std::string& user,
					const std::string& password,
					const std::string& fileSrc,
					const std::string& fileDest) throw(sshException);
				/*!
				*	\brief  Méthode permettant de déposer un fichier sur le serveur distant
				*	\param	const std::string& \b host : adresse ip de l'hôte, je n'ai pas réussi à faire fonctionner la résolution des noms encore.
				*	\param	const std::string& \b user : login de l'utilisateur utilisé pour se connecter à la machine en ssh.
				*	\param	const std::string& \b password : mot de passe utilisé pour authentifier l'utilisateur.
				*	\param	const std::string& \b fileSrc : chemin complet vers le fichier se trouvant sur la machine éxécutant le script.
				*	\param	const std::string& \b fileDest : chemin complet vers le fichier allant être stocké sur la machine distante.
				*	\return int : code d'erreur ou de réussite sous la forme d'un int.
				*
				*	Cette méthode permet de se connecter à un serveur distant et d'y déposer un fichier à l'endroit désiré.
				*
				*/
			int	scpPut(	const std::string& host,
					const std::string& user,
					const std::string& password,
					const std::string& fileSrc,
					const std::string& fileDest) throw(sshException);
		private:
		protected:
	};
}
#endif
/**
*	Fermeture du groupe Doxygen
*	@}
*/
