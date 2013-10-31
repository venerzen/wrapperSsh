#include "WrapperSsh.h"
namespace	wrapperSsh	{
void wrapperSsh::kbd_callback( const char *name, int name_len, const char *instruction, int instruction_len, int num_prompts, const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts, LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, void **abstract)	{
	std::string password	= "null";
	password	= wrapperSsh::setPassword(password);
    (void)name;
    (void)name_len;
    (void)instruction;
    (void)instruction_len;
    if (num_prompts == 1) {
        responses[0].text	= strdup(password.c_str());
        responses[0].length	= strlen(password.c_str());
	}
    (void)prompts;
    (void)abstract;
} /* kbd_callback */ 
	/*	--------------------------------------------------
		Partie concernant les constructeurs
	--------------------------------------------------	*/
	wrapperSsh::wrapperSsh()	{
		this->host	= "";
		this->returnCode	= EXIT_SUCCESS;
	}
	wrapperSsh::wrapperSsh(const std::string& host)	:
		host(host){
		this->hostAddr		= inet_addr(this->getIp(this->host).c_str());
		this->returnCode	= EXIT_SUCCESS;
	}
	wrapperSsh::wrapperSsh(const wrapperSsh& w):
			host(w.host)	{
		this->hostAddr		= inet_addr(this->getIp(this->host).c_str());
		this->returnCode	= EXIT_SUCCESS;
	}
	wrapperSsh::~wrapperSsh()	{
		this->portDisConnect();
	}
	/*	--------------------------------------------------
		Partie concernant l'établissement d'une connexion
	--------------------------------------------------	*/
	std::string	wrapperSsh::getIp(const std::string& host)	throw(wrapperSshException)	{
		struct	addrinfo	hints, *res, *p; 
		int	status;
		char	ipStr[INET6_ADDRSTRLEN];
		memset(&hints, 0, sizeof hints);
		hints.ai_family		= AF_UNSPEC; // AF_INET or AF_INET6 to force version
		hints.ai_socktype	= SOCK_STREAM;
		if((status = getaddrinfo(host.c_str(), NULL, &hints, &res)) != 0)	throw	wrapperSshException(16,"GettaddrInfo failed.");
		for(p = res;p != NULL; p = p->ai_next)	{
			void	*addr;

				// get the pointer to the address itself,
				// different fields in IPv4 and IPv6:
			if (p->ai_family == AF_INET) { // IPv4
				struct sockaddr_in *ipv4	= (struct sockaddr_in *)p->ai_addr;
				addr	= &(ipv4->sin_addr);
			} else { // IPv6
				struct sockaddr_in6 *ipv6	= (struct sockaddr_in6 *)p->ai_addr;
				addr	= &(ipv6->sin6_addr);
			} 

			// convert the IP to a string and print it:
			inet_ntop(p->ai_family, addr, ipStr, sizeof ipStr);
		}
		freeaddrinfo(res); // free the linked list

		return	std::string(ipStr);
	}
	void	wrapperSsh::sshInit(const std::string& user, const std::string& password)	throw(wrapperSshException)	{
			//	Initialisation des variables 
		this->exitSignal	= (char *)"none";
		this->byteCount		= 0;
			//	Connection au port 22
		this->portConnect();
			//	Négociation ssh
		this->sshConnect(user,password);
	}
	void	wrapperSsh::sshStop()	throw(wrapperSshException)	{
		this->portDisConnect();
		this->sshDisconnect();
	}
	std::string	wrapperSsh::setPassword(std::string passwd)	{
		static  std::string password;
		if(password.empty())    {   
			password    = passwd;
		}   
		return  password;
	}
	void	wrapperSsh::sshConnect(const std::string& user, const std::string& password)	throw(wrapperSshException)	{
				//	ouverture de la session
		this->session	= libssh2_session_init();	// on ouvre une session ssh
		if(!this->session)	throw	wrapperSshException(2,"Failed to initiate ssh session " + this->host);
		while ((this->returnCode = libssh2_session_startup(this->session, this->sock)) == LIBSSH2_ERROR_EAGAIN) ;
		if (this->returnCode) throw	wrapperSshException(3,"Failure establishing SSH session" + this->host);
				//	récupération des types de connections possible 
		this->setUserAuthList(user);
				//	et on choisi le mode de connexion utilisé
		char	delim	= ',';
		std::vector<std::string> elems	= string::split(this->userAuthList,delim);
		if(std::find(elems.begin(), elems.end(), "password") != elems.end()) {								//	password connexion
			if ( password.length() != 0 ) {
				while ((this->returnCode = libssh2_userauth_password(this->session, user.c_str(), password.c_str())) == LIBSSH2_ERROR_EAGAIN) ;
				if (this->returnCode) 	throw	wrapperSshException(5,"Authentification by password failed on : " + this->host);
			}
			else	throw	wrapperSshException(6,"Authentification password is required." + this->host);
		}	else	if(std::find(elems.begin(), elems.end(), "keyboard-interactive") != elems.end()) {		//	keyboord-interactive
			std::string tmp = 	wrapperSsh::setPassword(password);	// c'est crade, mais ça fonctionne…
				if (libssh2_userauth_keyboard_interactive(this->session, user.c_str(), &kbd_callback) ) {
					std::cerr << "\tAuthentication by keyboard-interactive failed!\n" << std::endl;
					throw	wrapperSshException(4,"Authentification method not allowed." + this->host);
				} 
		}	else	if(std::find(elems.begin(), elems.end(), "publickey") != elems.end()) {					// publickey
			std::cout << "je m'y connecte via une autentification par « publickey »" << std::endl;
			throw	wrapperSshException(4,"Authentification method not allowed." + this->host);
		}	else	{
			std::cout << "Mode connexion non pris en charge " << std::endl;
			std::cout << "« elems » contains :" << std::endl;
			for (unsigned i=0; i<elems.size(); i++)	std::cout << "\t" << elems.at(i) << std::endl;
			throw	wrapperSshException(4,"Authentification method not allowed." + this->host);
		}
		
			//	ajout des infos complémentaires en cas d'erreur
		//libssh2_trace(this->session,LIBSSH2_TRACE_ERROR);
			//	on passe en non bloquant
	
	}
	void	wrapperSsh::sshDisconnect()	throw(wrapperSshException)	{
		if(this->channel)	libssh2_channel_free(this->channel);
		libssh2_session_disconnect(this->session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(this->session);
	}
	void	wrapperSsh::portConnect()	throw(wrapperSshException)	{
			// on va ouvrir un simple socket à destination du port 22
		this->sock	= socket(AF_INET, SOCK_STREAM, 0);
		this->sin.sin_family		= AF_INET;
		this->sin.sin_port		= htons(22);
		this->sin.sin_addr.s_addr	= this->hostAddr;
		if (connect(this->sock, (struct sockaddr*)(&this->sin), sizeof(struct sockaddr_in)) != 0)	throw	wrapperSshException(1,"Failed to connect port 22 on " + this->host);
	}
	void	wrapperSsh::portDisConnect()	throw(wrapperSshException)	{
		close(this->sock);
	}
	int	wrapperSsh::waitsocket(int socket_fd, LIBSSH2_SESSION *session)	{
		struct	timeval timeout;
		int	rc;
		fd_set	fd;
		fd_set	*writefd	= NULL;
		fd_set	*readfd		= NULL;
		int	dir;
 
		timeout.tv_sec	= 10;
		timeout.tv_usec	= 0;
 
		FD_ZERO(&fd);
		FD_SET(socket_fd, &fd);
 
		/* now make sure we wait in the correct direction */ 
		dir	= libssh2_session_block_directions(session);
		if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)	readfd	= &fd;

		if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)	writefd	= &fd;
 
		rc	= select(sock + 1, readfd, writefd, NULL, &timeout);
 
		return	rc;
	}
	/*	--------------------------------------------------
		Partie concernant les différents types de comportement que l'on souhaite (ouverture d'un shell, interface pour simplement faire du scp, etc…)
	--------------------------------------------------	*/
	void	wrapperSsh::setUserAuthList(const std::string& user)	throw(wrapperSshException)	{
		char	*userauthlist	= libssh2_userauth_list(this->session, user.c_str(), user.length());
		this->userAuthList	= std::string(userauthlist);
	}
	void	wrapperSsh::sshRequestShell() throw(wrapperSshException)	{
			//	le but maintenant est d'obtenir un shell
		if(!(this->channel = libssh2_channel_open_session(this->session)))	throw	wrapperSshException(8,"Failed opening channel session" + this->host);
			//	émulation du terminal demandé	
			//	/etc/termcap pour plus d'option
		if(libssh2_channel_request_pty(this->channel, "vanilla"))	throw	wrapperSshException(9,"Failed requesting pty" + this->host);
			//	ouverture du shell
		if(libssh2_channel_shell(this->channel))	throw	wrapperSshException(10,"Unable to request shell on allocated pty." + this->host);
			//	on passe en mode sans bloquage
		libssh2_session_set_blocking(this->session,0);
	}	
	void	wrapperSsh::sshRequestChannelExecMode() throw(wrapperSshException)	{
			//	le but maintenant est d'obtenir un shell
		if(!(this->channel = libssh2_channel_open_session(this->session)))	throw	wrapperSshException(8,"Failed opening channel session" + this->host);
			//	émulation du terminal demandé	
			//	/etc/termcap pour plus d'option
		if(libssh2_channel_request_pty(this->channel, "vanilla"))	throw	wrapperSshException(9,"Failed requesting pty" + this->host);
			//	on passe en mode sans bloquage
		libssh2_session_set_blocking(this->session,0);
	}	
	/*	--------------------------------------------------
		Partie concernant les différents commandes qu'il possible d'effectuer
	--------------------------------------------------	*/
		sshExecResult	wrapperSsh::sshExec(const std::string& command)	throw(wrapperSshException)	{
		sshExecResult	result;
			//	Execution de la commande
		while( (this->returnCode = libssh2_channel_exec(this->channel, command.c_str())) == LIBSSH2_ERROR_EAGAIN )	{
			this->waitsocket(this->sock,this->session);
		}
		if( this->returnCode != 0 )	throw	wrapperSshException(11,"Execution of the command FAILED " + this->host);
			//	Récupération du résultat dans une string
		for( ;; )	{
				/* loop until we block */ 
			int	rc;
			int	bytecount	= 0;
			do	{
				char buffer[0x4000];
				rc	= libssh2_channel_read( this->channel, buffer, sizeof(buffer) );
					// sortie standard
				if( rc > 0 )	{
					bytecount	+= rc;
					result.result	+= std::string(buffer).substr(0,rc);
				}
			}
			while( rc > 0 );
			rc	= 0;
			do	{
				char buffer[0x4000];
					// sortie d'erreur
				rc	= libssh2_channel_read_stderr( this->channel, buffer, sizeof(buffer) );
				if( rc > 0 )	{
					bytecount	+= rc;
					result.error	+= std::string(buffer).substr(0,rc);
				}
			}
			while( rc > 0 );
 
				/* this is due to blocking that would occur otherwise so we loop on this condition */ 
			if( rc == LIBSSH2_ERROR_EAGAIN )	this->waitsocket(this->sock, this->session);
			else					break;
		}
		return	result;
	}
	void	wrapperSsh::scpGet(const std::string& fileSrc,const std::string& fileDest)	throw(wrapperSshException)	{
		struct	stat	fileinfo;
		off_t	got	= 0;
		int		initSize;
		
		if(!(this->channel = libssh2_scp_recv(this->session, fileSrc.c_str(), &fileinfo)))	throw	wrapperSshException(12,"SCP get FAILED (file not exist ?)");

		std::ofstream	file(fileDest.c_str(), std::ios::out | std::ios::trunc | std::ofstream::binary);	//	déclaration du flux et ouverture du fichier
		if(!file)	throw	wrapperSshException(12,"Temporary file could not be opened");
		initSize	= fileinfo.st_size;
		while(got < fileinfo.st_size)	{
			char	buffer[1024];
			int	amount	= sizeof(buffer);
			int	rc;
			if((fileinfo.st_size - got) < amount)	{
				amount	= fileinfo.st_size -got;
			}

			rc	= libssh2_channel_read(this->channel, buffer, amount );
			if(rc > 0)	{	
				file.write(buffer,rc);
			}
			else if(rc < 0)	{
				throw	wrapperSshException(13,"scpGet Channel read error ");
			}
			got	+= rc;
		}
                file.close();  // on referme le fichier
	}
	void	wrapperSsh::scpPut(const std::string& fileSrc,const std::string& fileDest)	throw(wrapperSshException)	{
		struct	stat	fileinfo;
		off_t	got	= 0;
			//	 on commence par tripoter le fichier source. Il faut en connaitre la taille
		std::ifstream	file(fileSrc.c_str(), std::ios::in | std::ofstream::binary);
		if(!file)	throw	wrapperSshException(14,"Source file could not be opened");
		else	{
				//	il est ouvert, on en extrait les stats
			if (stat(fileSrc.c_str(), &fileinfo) != 0)	throw	wrapperSshException(15,"Stat problem with the source file");
				//	puis il faut ouvrir le channel
			if(!(this->channel	= libssh2_scp_send(this->session, fileDest.c_str(), fileinfo.st_mode & 0777,(unsigned long)fileinfo.st_size)))	{
				throw	wrapperSshException(16,"Send channel opening error");
			};
				//	et au final, il ne reste plus qu'à lire le contenu et à l'écrire dans le channel ouvert
			while(got < fileinfo.st_size)	{
				char	buffer[1024];
				int	amount	= sizeof(buffer);
				int	rc;

				if((fileinfo.st_size - got) < amount)	amount	= fileinfo.st_size -got;	// permet de ne pas dépasser la taille du fichier et remplir le buffer avec n'importe quoi
				file.read (buffer,amount);
					// et maintenant j'écris dans le fichier cible
				rc = libssh2_channel_write(channel,buffer,amount);
				if( rc < 0 )	throw	wrapperSshException(16,"Scp write failed.");
				
				got	+= amount;
			}
				//	clôture du fichier
			file.close();
				//	clôture du channel
			libssh2_channel_send_eof(this->channel);
			libssh2_channel_wait_eof(this->channel);
			libssh2_channel_wait_closed(this->channel);
		}
		
	}
}
