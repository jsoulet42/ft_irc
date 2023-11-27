
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



