
#include "Server.hpp"

Server::Server(const int port, const std::string& password) : _port(port), _password(password)
{
	this->_initServerSocket();
	fdNb = 0;
	fdP.push_back(pollfd());
	fdP[0].fd = this->_serverSocket;
	fdP[0].events = POLLIN;
	ERROR = false;
}

Server::~Server()
{
	for (size_t i = 0; i < this->fdP.size(); ++i)
	{
		if (this->fdP[i].fd != this->_serverSocket && this->fdP[i].fd != -1)
		{
			shutdown(this->fdP[i].fd, SHUT_RDWR);
			close(this->fdP[i].fd);
		}
	}
	shutdown(this->_serverSocket, SHUT_RDWR);
	close(this->_serverSocket);
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
	std::string buffer;
	buffer.resize(BUFFSIZE + 1);
	User *newuser = new User(newFd);
	this->users.push_back(newuser);
	ERROR = false;

	read(newFd, &buffer[0], BUFFSIZE + 1);
	buffer.push_back('\0');
	if (newuser->_forNcProtocol == 1)
		this->passProtocol(buffer, newuser);

	read(newFd, &buffer[0], BUFFSIZE + 1);
	buffer.push_back('\0');
	if (newuser->_forNcProtocol == 2)
		this->CapProtocol(buffer, newuser);

	read(newFd, &buffer[0], BUFFSIZE + 1);
	buffer.push_back('\0');
	if (newuser->_forNcProtocol == 3)
		this->NickProtocol(newFd, buffer, newuser);

	read(newFd, &buffer[0], BUFFSIZE + 1);
	buffer.push_back('\0');
	if (newuser->_forNcProtocol == 4)
		this->UserProtocol(buffer, newuser);

	fdP.push_back(pollfd());
	this->fdP[this->fdNb].fd = newFd;
	this->fdP[this->fdNb].events = POLLIN;

	std::string message = IPHOST + std::string(" 001 ") + newuser->nickname + " :Welcome to the ft_irc network, " + newuser->nickname + "\r\n";
	send(newFd, message.c_str(), message.length(), 0);
	std::cout << "New user " << newuser->nickname << " succesfully registered with id " << newFd << "." << std::endl;
}

void Server::passProtocol(std::string buffer, User *newUser)
{
	std::string pass = buffer;
	pass = strtok((char *)pass.c_str() + 5, "\r\n");

	if (buffer.compare(0, 4, "PASS") != 0)
		sendError(newUser->_fdUser, ERRORP421);
	else if (pass.size() == 0)
		sendError(newUser->_fdUser, ERRORP461);
	else if (pass != this->_password)
		sendError(newUser->_fdUser, ERRORP464);
	if (ERROR == true)
		throw PassException();
	newUser->_forNcProtocol++;
}

void Server::CapProtocol(std::string buffer, User *newUser)
{
	std::cout << "buffer : " << buffer << std::endl;
	if (buffer.compare(0, 3, "CAP") != 0)
		sendError(newUser->_fdUser, ERRORC421);
	if (ERROR == true)
		throw CapException();
	newUser->_forNcProtocol++;
}

void Server::NickProtocol(int newFd, std::string buffer, User *newUser)
{
	std::string nick = buffer;
	nick = strtok((char *)nick.c_str() + 5, "\r\n");

	if (buffer.compare(0, 4, "NICK") != 0)
		sendError(newUser->_fdUser, ERRORN421);
	else if (nick.size() == 0)
		sendError(newUser->_fdUser, ERRORN431);
	else if (this->checkNick(newFd, nick) == 432)
		sendError(newUser->_fdUser, ERRORN432);
	else if (this->checkNick(newFd, nick) == 433)
		sendError(newUser->_fdUser, ERRORN433);
	if (ERROR == true)
		throw NickException();
	newUser->_forNcProtocol++;
	newUser->nickname = nick;
}

int		Server::checkNick(int & fd, std::string nickname)
{
	const std::string invalidChars = "# :";

	for (size_t i = 0; i < invalidChars.size(); ++i)
	{
		char c = invalidChars[i];
		if (nickname.find(c) != std::string::npos)
			return 432;
	}
	std::vector<User *>::iterator	it = this->users.begin();
	while (it != this->users.end())
	{
		if ((*it)->_fdUser != fd && (*it)->nickname == nickname)
			return 433;
		it++;
	}
	return 0;
}

void Server::UserProtocol(std::string buffer, User *newUser)
{
	std::string uname = buffer;

	uname = strtok((char *)uname.c_str() + 5, "\r\n");
	if (buffer.compare(0, 4, "USER") != 0)
		sendError(newUser->_fdUser, ERRORU421);
	else if (uname.size() == 0 || uname.find(':') == std::string::npos)
		sendError(newUser->_fdUser, ERRORU461);
	std::string	username = uname.substr(0, uname.find(' '));
	uname.erase(0, uname.find(' ') + 1);
	if (uname.compare(0, 3, "0 *") && ERROR == false)
		sendError(newUser->_fdUser, ERRORU461);
	uname.erase(0, 5);
	std::string	realname = uname;
	if (ERROR == true)
		throw UserException();
	newUser->username = username;
	newUser->realname = uname;
}

void Server::sendError(int fd, std::string error)
{
	std::stringstream ERR_error;
	ERR_error << IPHOST << error;
	send(fd, ERR_error.str().c_str(), ERR_error.str().size(), 0);
	ERROR = true;
}

std::vector<User *>::iterator Server::getUser(int fd)
{
	std::vector<User *>::iterator	user = this->users.begin();

	while (user != this->users.end())
	{
		if ((*user)->_fdUser == fd)
			return (user);
		user++;
	}
	return this->users.end();
}

void Server::deleteUser(int fd)
{
	std::vector<User *>::iterator	user = getUser(fd);
	delete *user;
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


