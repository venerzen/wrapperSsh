#include <iostream>
#include <string>
#include <sstream>
#include "CtrlSsh.h"

namespace	ctrlSsh	{
	/*	--------------------------------------------------
		Partie concernant les constructeurs
	--------------------------------------------------	*/
		//	Constructeur par défaut
	ctrlSsh::ctrlSsh()	{
	}
	ctrlSsh::~ctrlSsh()	{
	}
/*
	ctrlSsh::ctrlSsh(const ctrlSsh& r):
			host(r.host)	{
	}
*/
	/*	--------------------------------------------------
		Partie dédiée aux traitements
	--------------------------------------------------	*/
	wrapperSsh::sshExecResult	ctrlSsh::execTask(const std::string& host, const std::string& user, const std::string& password, const std::string& command)	throw(sshException)	{
		wrapperSsh::sshExecResult	result;
		wrapperSsh::wrapperSsh	* objWrapperSsh	= new wrapperSsh::wrapperSsh(host);		// objet permettant de s'interfacer avec un serveur ssh
		try	{	

			objWrapperSsh->sshInit(user,password);
			objWrapperSsh->sshRequestChannelExecMode();
			result	= objWrapperSsh->sshExec(command);
			objWrapperSsh->sshStop();

			delete(objWrapperSsh);

		}
		catch (const wrapperSshException& e)	{
			result.error	= e.what();
			result.result	= std::string("Error");
			delete(objWrapperSsh);
			throw	sshException(1, e.what());
		}
		catch(...)	{
			delete(objWrapperSsh);
			result.result	= std::string("Error");
			throw	sshException(1, "[wrapperSsh::sshExecResult]: an undefinerd error occured..");
		}
		
		return	result;
	}
	int	ctrlSsh::scpGet(	const std::string& host,
							const std::string& user,
							const std::string& password,
							const std::string& fileSrc,
							const std::string& fileDest) throw(sshException)	{
		int	returnCode	= EXIT_SUCCESS;
		wrapperSsh::wrapperSsh	* objWrapperSsh	= new wrapperSsh::wrapperSsh(host);		// objet permettant de s'interfacer avec un serveur ssh
		try	{

			objWrapperSsh->sshInit(user,password);
				// récupération du fichier
			objWrapperSsh->scpGet(fileSrc,fileDest);
			
				// copie du fichier à destination
			objWrapperSsh->sshStop();

			delete(objWrapperSsh);
		}
		catch (const wrapperSshException& e)	{
			std::cerr << "\t\t" << e.what() << std::endl;
			delete(objWrapperSsh);
			returnCode	= EXIT_FAILURE;
		}
		catch(...)	{
			delete(objWrapperSsh);
			returnCode	= EXIT_FAILURE;
		}
		
		return	returnCode;	
	}
	int	ctrlSsh::scpPut(	const std::string& host,
					const std::string& user,
					const std::string& password,
					const std::string& fileSrc,
					const std::string& fileDest) throw(sshException)	{
		int	returnCode	= EXIT_SUCCESS;
		wrapperSsh::wrapperSsh	* objWrapperSsh	= new wrapperSsh::wrapperSsh(host);		// objet permettant de s'interfacer avec un serveur ssh
		try	{

			objWrapperSsh->sshInit(user,password);
				// récupération du fichier
			objWrapperSsh->scpPut(fileSrc,fileDest);
			
				// copie du fichier à destination
			objWrapperSsh->sshStop();

			delete(objWrapperSsh);
		}
		catch (const wrapperSshException& e)	{
			std::cerr << "\t\t" << e.what() << std::endl;
			delete(objWrapperSsh);
			returnCode	= EXIT_FAILURE;
		}
		catch(...)	{
			delete(objWrapperSsh);
			returnCode	= EXIT_FAILURE;
		}
		
		return	returnCode;	
	}
}
