#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>    /* for getopt */
#include "libs/CtrlSsh.h"
#include "libs/WrapperSsh.h"

int	main (int argc, char* argv[])	{
	int	rc	= EXIT_SUCCESS;	//	return code
	int optCode;        	// code retour du système de récupération des options.
	std::string	user,host,passwd;
	std::string	cmd	= "uname -a";
	try	{
		while ((optCode = getopt (argc, argv, "h:u:p:t:c:l:g:s:b:v::")) != -1)       {
			switch(optCode) {
				case 'h' :
					host	= optarg;
				break;
				case 'u' :
					user	= optarg;
				break;
				case 'p' :
					passwd	= optarg;
				break;
			}
		}
			// Vérification de manière à savoir s'il y a assez d'infos
		if( host.empty() )	{
			rc	= EXIT_FAILURE;
			std::cerr << "Host is missing" << std::endl;
		}	
		if( user.empty() )	{	
			rc	= EXIT_FAILURE;
			std::cerr << "User is missing" << std::endl;
		}	
		if( passwd.empty() )	{	
			rc	= EXIT_FAILURE;
			std::cerr << "Password is missing" << std::endl;
		}
		if(rc == EXIT_SUCCESS)	{
			ctrlSsh::ctrlSsh *objCtrlSsh	= new ctrlSsh::ctrlSsh();
			wrapperSsh::sshExecResult		commandResult;	// résultat de l'exécution de la commande
			commandResult	= objCtrlSsh->execTask(	host,
													user,
													passwd,
													cmd);
			delete(objCtrlSsh);
			std::cout << commandResult.result << std::endl;
		}
	}	catch(const wrapperSshException& e)	{
			std::cout << "Je suis dans ce cas" << std::endl;
	}	catch(...)	{
		rc	= EXIT_FAILURE;
		std::cerr << "Something failed…" << std::endl;
	}
	
	return	rc;
}
