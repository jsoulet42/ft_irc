#pragma once

#include "ft_irc.hpp"

class Server
{
	private:

		void							_initServerSocket();
		int								_serverSocket;
		struct sockaddr_in				_adress;
		const int						_port;
		std::string						_password;

	// Vous pouvez ajouter d'autres variables membres nécessaires

	public:

		std::vector<struct pollfd>		fdP;
		int								fdNb;

		Server(const int port, const std::string& password);
		~Server();
		bool haveN(const std::string& str);

		int								getServerSocket() const;

};
