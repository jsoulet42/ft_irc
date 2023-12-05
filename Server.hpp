#pragma once

# include "ft_irc.hpp"
# include "User.hpp"
# include "Channel.hpp"

class Channel;

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
		std::vector<Channel*>			channels;
		int								fdNb;
		bool							ERROR;

		Server(const int port, const std::string& password);
		~Server();


		int								getServerSocket() const;
		std::vector<User *>::iterator	getUser(int fd);


		bool haveN(const std::string& str);
		void protocolNewUser(int newFd);
		void connexionParse(std::string buffer);
		void passProtocol(std::string buffer, User *newuser);
		void CapProtocol(std::string buffer, User *newuser);
		void NickProtocol(int newFd, std::string buffer, User *newuser);
		void UserProtocol(std::string buffer, User *newuser);
		int checkNick(int & fd, std::string nickname);
		void sendError(int fd, std::string error);
		void deleteUser(int fd);

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
