#pragma once

#include "ft_irc.hpp"
#include "User.hpp"

class Server
{
	private:

		void							_initServerSocket();
		int								_serverSocket;
		struct sockaddr_in				_adress;
		const int						_port;
		std::string						_password;
		bool							Error;

	public:

		std::vector<struct pollfd>		fdP;
		std::vector<User*>				users;
		int								fdNb;

		Server(const int port, const std::string& password);
		~Server();


		int								getServerSocket() const;


		bool haveN(const std::string& str);
		void protocolNewUser(int newFd);
		void passProtocol(int newfd, const char *buffer, User *newuser);
		void CapProtocol(int newFd, const char *buffer, User *newuser);
		void NickProtocol(int newFd, const char *buffer, User *newuser);
		void UserProtocol(int newFd, const char *buffer, User *newuser);
		int checkNick(int & fd, std::string nickname);




		class PassException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		class UserException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		class NickException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		class CapException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};
