
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
#define IPHOST ":127.0.0.1"
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

extern bool isRunning;

