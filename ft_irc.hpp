
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

//---------Join---------//
#define ERR_NEEDMOREPARAMS " 461 JOIN :Not enough parameters\r\n"
#define ERR_NOSUCHCHANNEL " 403 JOIN :No such channel\r\n"
#define ERR_TOOMANYCHANNELS " 405 JOIN :You have joined too many channels\r\n"
#define ERR_BADCHANNELKEY " 475 JOIN :Cannot join channel (+k)\r\n"
#define ERR_BANNEDFROMCHAN " 474 JOIN :Cannot join channel (+b)\r\n"
#define ERR_CHANNELISFULL " 471 JOIN :Cannot join channel (+l)\r\n"
#define ERR_INVITEONLYCHAN " 473 JOIN :Cannot join channel (+i)\r\n"
#define ERR_BADCHANMASK " 476 JOIN :Bad Channel Mask\r\n"
#define RPL_TOPIC " 332 JOIN :<channel> :<topic>\r\n"
#define RPL_TOPICWHOTIME " 333 JOIN :<channel> <nickname> <time>\r\n"
#define RPL_NAMREPLY " 353 JOIN :<channel> :<nicknames>\r\n"
#define RPL_ENDOFNAMES " 366 JOIN :<channel> :End of /NAMES list\r\n"
