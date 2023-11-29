
#include "Server.hpp"

Server::Server(const int port, const std::string& password) : _port(port), _password(password)
{
	this->_initServerSocket();

	fdNb = 0;
	fdP.push_back(pollfd());
	fdP[0].fd = this->_serverSocket;
	fdP[0].events = POLLIN;
}

Server::~Server()
{
	shutdown(this->_serverSocket, SHUT_RDWR);
}


void Server::_initServerSocket()
{
	int opt = 1;
	//int addrlen = sizeof(this->_adress);
	if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("[ERROR] : can't create a socket, please check your permissions or cpu arch.");
	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[ERROR] : can't set options of socket, please check your permissions or cpu arch.");
	int flags = fcntl(this->_serverSocket, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("[ERROR] : can't get socket flags, please check your permissions or cpu arch.");
	if (fcntl(this->_serverSocket, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("[ERROR] : can't set socket to non-blocking mode, please check your permissions or cpu arch.");
	std::memset(&this->_adress, 0, sizeof(this->_adress));
	this->_adress.sin_family = AF_INET;
	this->_adress.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->_adress.sin_port = htons(this->_port);
	if (bind(this->_serverSocket, (struct sockaddr*)&this->_adress, sizeof(this->_adress)) < 0)
		throw std::runtime_error("[ERROR] : can't get local address information, bind failed, please check your permissions or cpu arch.");
	if (listen(this->_serverSocket, 3) < 0)
		throw std::runtime_error("[ERROR] : can't listen on the address, please check your permissions or cpu arch.");
}

int Server::getServerSocket() const
{
	return (this->_serverSocket);
}

bool Server::haveN(const std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '\n')
			return (true);
	}
	return (false);
}

void Server::protocolNewUser(int newFd)
{
	int rc;
	//int i = 5;
	char buffer[BUFFSIZE + 1];
	User *newuser = new User(newFd);
	this->users.push_back(newuser);

	rc = read(newFd, buffer, BUFFSIZE + 1);
	buffer[rc] = '\0';
	if (newuser->_forNcProtocol == 1)
		this->passProtocol(newFd, buffer, newuser);

	rc = read(newFd, buffer, BUFFSIZE + 1);
	buffer[rc] = '\0';
	if (newuser->_forNcProtocol == 2)
		this->CapProtocol(newFd, buffer, newuser);

	rc = read(newFd, buffer, BUFFSIZE + 1);
	buffer[rc] = '\0';
	if (newuser->_forNcProtocol == 3)
		this->NickProtocol(newFd, buffer, newuser);

	rc = read(newFd, buffer, BUFFSIZE + 1);
	buffer[rc] = '\0';
	if (newuser->_forNcProtocol == 4)
		this->UserProtocol(newFd, buffer, newuser);
}

void Server::passProtocol(int newFd, const char *buffer, User *newUser)
{
	std::stringstream ERR_error;
	std::string pass = buffer;
	pass = pass.c_str() + 5;

	if (strncmp(buffer, "PASS", 4) != 0)
	{
		ERR_error << ":" << IPHOST << " 421 PASS :Unknown command\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	else if (pass.size() == 0)
	{
		ERR_error << ":" << IPHOST << " 461 PASS :Not enough parameters\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	else if (pass != this->_password)
	{
		ERR_error << ":" << IPHOST << " 464 PASS :Password incorrect\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	if (ERR_error.str().size() != 0)
		throw PassException();
	newUser->_forNcProtocol++;
}

void Server::CapProtocol(int newFd, const char *buffer, User *newUser)
{
	std::stringstream ERR_error;

	if (strncmp(buffer, "CAP", 3) != 0)
	{
		ERR_error << ":" << IPHOST << " 421 CAP :Unknown command\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	if (ERR_error.str().size() != 0)
		throw CapException();
	newUser->_forNcProtocol++;
}

void Server::NickProtocol(int newFd, const char *buffer, User *newUser)
{
	std::stringstream ERR_error;
	std::string nick = buffer;
	nick = nick.c_str() + 5;

	if (strncmp(buffer, "NICK", 4) != 0)
	{
		ERR_error << ":" << IPHOST << " 421 NICK :Unknown command\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	else if (nick.size() == 0)
	{
		ERR_error << ":" << IPHOST << " 431 NICK :No nickname given\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	else if (this->checkNick(newFd, nick) == 432)
	{
		ERR_error << ":" << IPHOST << " 432 NICK :Erroneous nickname\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	else if (this->checkNick(newFd, nick) == 433)
	{
		ERR_error << ":" << IPHOST << " 433 NICK :Nickname is already in use\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	if (ERR_error.str().size() != 0)
		throw NickException();
	newUser->_forNcProtocol++;
	newUser->nickname = nick;
}

int		Server::checkNick(int & fd, std::string nickname)
{
	if (nickname.find('#') != std::string::npos || nickname.find(' ') != std::string::npos || nickname.find(':') != std::string::npos)
		return 432;
	std::vector<User *>::iterator	it = this->users.begin();
	while (it != this->users.end())
	{
		if ((*it)->_fdUser != fd && (*it)->nickname == nickname)
			return 433;
		it++;
	}
	return 0;
}

void Server::UserProtocol(int newFd, const char *buffer, User *newUser)
{
	std::stringstream ERR_error;
	std::string uname = buffer;
	uname = uname.c_str() + 5;

	if (strncmp(buffer, "USER", 4) != 0)
	{
		ERR_error << ":" << IPHOST << " 421 USER :Unknown command\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	else if (uname.size() == 0)
	{
		ERR_error << ":" << IPHOST << " 461 USER :Not enough parameters\r\n";
		send(newFd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	}
	if (ERR_error.str().size() != 0)
		throw UserException();
	newUser->_forNcProtocol++;
	newUser->username = uname;
}

const char* Server::PassException::what() const throw()
{
	return "Error during PASS command";
}


const char* Server::CapException::what() const throw()
{
	return "Error during CAP command";
}

const char* Server::NickException::what() const throw()
{
	return "Error during NICK command";
}

const char* Server::UserException::what() const throw()
{
	return "Error during USER command";
}
