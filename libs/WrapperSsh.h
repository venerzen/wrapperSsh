#ifndef WRAPPERSSH_H
#define	WRAPPERSSH_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>		// wtf !?
#include <libssh2.h>
#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include "string.h"
#include "WrapperSshException.h"
#include <algorithm>    // std::find


/**
*       \class		wrapperSsh::wrapperSsh
*       \ingroup        sshTools
*       \brief  Contrôleur de connexion.
*
*       Cette classe est un wrapper de la libssh2. Elle comporte un ensemble de méthodes permettant de manipuler plus simplement cette dernière.
*
*       \namespace      wrapperSsh
*
*       \brief  Wrapper de la libssh2.
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
namespace	wrapperSsh	{
	/*!
	*	Structure contenant l'équivalent des std::stderr et std::out lors de l'exécution d'une commande.
	*/
	struct	sshExecResult	{
		/*!
		*	Correspond à std::cout.
		*/
		std::string	result;
		/*!
		*	Correspond à std::stderr.
		*/
		std::string	error;
	};
	class	wrapperSsh	{
		public:
			static	std::string	setPassword(std::string passwd);
				/*!
				*	\brief  Constructeur par défaut. 
				*	\return void
				*
				*	Constructeur par défaut appelé à chaque création d'une instance de ce contrôleur.
				*
				*/
			wrapperSsh();
				/*!
				*	\brief  Constructeur.
				*	\param	const std::string \b host : nom dns ou ip de l'hôte. 
				*	\return void
				*
				*	Constructeur avec le nom dns ou l'ip de l'hôte en plus.
				*
				*/
			wrapperSsh(const std::string& host);
				/*!
				*	\brief  Constructeur de copie par défaut. 
				*	\param	const wrapperSsh& \b host : wrapperSsh dont on va faire la copie.
				*	\return void
				*
				*	Constructeur par de copie par défaut.
				*
				*/
			wrapperSsh(const wrapperSsh& w);
				/*!
				*	\brief  Destructeur par défaut.
				*	\return void
				*
				*	Destructeur par défaut.
				*
				*/
			~wrapperSsh();

				/*!
				*	\brief	Méthode d'affectation d'un hôte.
				*	\param	const std:string& \b host : nom/ip de l'hôte.
				*	\return	void
				*
				*	Cette méthode permet d'affecter un nouvel hôte à l'objet.
				*
				*/
			void	setHost(const std::string& host) throw(wrapperSshException);
				/*!
				*	\brief	Méthode d'initialisation de la connexion ssh.
				*	\param	const std:string& \b user : login de l'utilisateur
				*	\param	const std:string& \b password : mot de passe de l'utilisateur
				*	\return	void
				*
				*	Cette méthode permet d'initialiser la connexion ssh à l'hôte via le couple login/passwd.
				*
				*/
			void	sshInit(const std::string& user, const std::string& password) throw(wrapperSshException);
				/*!
				*	\brief	Méthode de clôture d'une session ssh.
				*	\return	void
				*
				*	Cette méthode permet de clore la session ssh ouverte la connexion ssh à l'hôte.
				*
				*/
			void	sshStop() throw(wrapperSshException);
				/*!
				*	\brief	Méthode permettant d'exécuter une commande sur un serveur distant.
				*	\param	const std:string& \b command : commande à exécuter
				*	\return	wrapperSsh::sshExecResult : structure contenant les erreurs ainsi que le résultat renvoyé par la sortie standard.
				*
				*	Cette méthode permet d'exécuter une commande et retourne le résultat dans une sturcture de type wrapperSsh::sshExecResult.
				*
				*/
			struct	sshExecResult	sshExec(const std::string& command)	throw(wrapperSshException);
				/*!
				*	\brief	Méthode permettant récupérer un fichier sur une machine distante via scp
				*	\param	const std:string& \b fileSrc : chemin vers le fichier à récupérer.
				*	\param	const std:string& \b fileDst : chemin local où le fichier doit être enregistré.
				*	\return	void
				*
				*	Cette méthode permet de récupérer un fichier via scp.
				*
				*/
			void	scpGet(const std::string& fileSrc,const std::string& fileDest)	throw(wrapperSshException);
				/*!
				*	\brief	Méthode permettant déposer un fichier sur une machine distante via scp
				*	\param	const std:string& \b fileSrc : chemin local vers le fichier à récupérer.
				*	\param	const std:string& \b fileDst : chemin où le fichier doit être enregistré.
				*	\return	void
				*
				*	Cette méthode permet de déposer un fichier via scp.
				*
				*/
			void	scpPut(const std::string& fileSrc,const std::string& fileDest)	throw(wrapperSshException);	// permet de déposer un fichier via scp
				/*!
				*	\brief	Méthode permettant de demander un « shell »
				*	\return	void
				*
				*	Cette méthode permet d'obtenir un shell.
				*
				*/
			void	sshRequestShell() throw(wrapperSshException);
				/*!
				*	\brief	Méthode permettant de paramètrer le channel pour l'exécution de commandes
				*	\return	void
				*
				*	Cette méthode permet de paramètrer le channel pour l'exécution de commandes.
				*
				*/
			void	sshRequestChannelExecMode() throw(wrapperSshException);
				/*!
				*	\brief	Méthode permettant de résoudre un nom dns.
				*	\param	coost std::string& \b host : nom de l'hôte
				*
				*	Cette méthode permet de résoudre un nom dns et donc d'en récupérer l'ip (v4 ou v6).
				*
				*/
			std::string	getIp(const std::string& host) throw(wrapperSshException);
			
			/*	Liste des champs	*/
				/*!  Nom dns ou adresse ip de l'hôte auquel on veut se connecter.  */
			std::string	host;
				/*!  Nom dns ou adresse ip de l'hôte auquel on veut se connecter mais au format binaire.  */
			unsigned long	hostAddr;
				/*! Socket se connectant au port 22 du serveur */
			int		sock;
			struct		sockaddr_in sin;
			const char	*fingerPrint;
				/*! Utilisé un peu partout comme élément retourné par les différentes fonctions et méthodes. */
			int	returnCode;
			int	exitCode;
			char	*exitSignal;
			int	byteCount;
			size_t	len;
			int	type;
				/*! Va contenir la liste des méthodes d'indentification. */
			std::string	userAuthList;
			LIBSSH2_KNOWNHOSTS	*nh;
			LIBSSH2_SESSION		*session;
			LIBSSH2_CHANNEL 	*channel;
		
		private:
		protected:
			void	sshConnect(const std::string& user, const std::string& password)	throw(wrapperSshException);
			void	sshDisconnect()	throw(wrapperSshException);
			void	portConnect()	throw(wrapperSshException);			// permet d'ouvrir une connection sur le port 22 du serveur
			void	portDisConnect()	throw(wrapperSshException);		// permet de clore la connection
			void	setUserAuthList(const std::string& user)	throw(wrapperSshException);
			int	waitsocket(int socket_fd, LIBSSH2_SESSION *session);
			static	void kbd_callback( const char *name, int name_len, const char *instruction, int instruction_len, int num_prompts, const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts, LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, void **abstract);
			
	};
}

#endif
/**
*	Fermeture du groupe Doxygen
*	@}
*/
