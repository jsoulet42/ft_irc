
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <sstream>
#include <csignal>

#define BUFFSIZE 1023
#define IPHOST ":127.0.0.1 "
#define ERRORP421 " 421 PASS :Unknown command\r\n"
#define ERRORP461 " 461 PASS :Not enough parameters\r\n"
#define ERRORP464 " 464 PASS :Password incorrect\r\n"
#define ERRORC421 " 421 CAP :Unknown command\r\n"
#define ERRORN421 " 421 NICK :Unknown command\r\n"
#define ERRORN432 " 432 NICK :Erroneus nickname\r\n"
#define ERRORN433 " 433 NICK :Nickname is already in use\r\n"
#define ERRORN431 " 431 NICK :No nickname given\r\n"
#define ERRORU421 " 421 USER :Unknown command\r\n"
#define ERRORU461 " 461 USER :Not enough parameters\r\n"


//exemple de message d'erreur <"":" + IPHOST + " " + code + " " user->nickname + " " + "JOIN " + " :Not enough parameters\r\n">
//std::string err_need_more_param = ":127.0.0.1 461 " + user->nickname + " JOIN :Not enough parameters\r\n";

#define ERRORJ461 " JOIN :Not enough parameters\r\n"
#define ERRORJ403 " JOIN :No such channel\r\n"
#define ERRORJ405 " JOIN :You have joined too many channels\r\n"
#define ERRORJ475 " JOIN :Cannot join channel (+k)\r\n"
#define ERRORJ473 " JOIN :Cannot join channel (+i)\r\n"
#define ERRORJ476 " JOIN :Bad Channel Mask\r\n"
extern bool isRunning;
extern bool errorCmd;

class joinException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class keyException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

