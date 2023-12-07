
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


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                    JOIN                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//exemple de message d'erreur <"":" + IPHOST + " " + code + " " user->nickname + " " + "JOIN " + " :Not enough parameters\r\n">
//std::string err_need_more_param = ":127.0.0.1 461 " + user->nickname + " JOIN :Not enough parameters\r\n";

#define ERRORJ403 " JOIN :No such channel\r\n"
#define ERRORJ405 " JOIN :You have joined too many channels\r\n"
#define ERRORJ461 " JOIN :Not enough parameters\r\n"
#define ERRORJ471 " JOIN :Cannot join channel (+l)\r\n"
#define ERRORJ473 " JOIN :Cannot join channel (+i)\r\n"
#define ERRORJ475 " JOIN :Cannot join channel (+k)\r\n"
#define ERRORJ476 " JOIN :Bad Channel Mask\r\n"
extern bool isRunning;
extern bool errorCmd;

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

class	User;
class	Server;
class	Channel;

bool	findUserInChannel(Channel *channel, User *user);
void	normKey(std::string &key, User &user, Server &server);
void	ircJoin(std::string &msg, User &user, Server &Server);
void	parseCmd(std::string &cmd, User &user, Server &Server);
bool	checkRightsUserInChannel(Channel *channel, User *user);
User	*findUserById(std::vector<User *> &users, int const &id);
void	parseCmdWithNoKey(std::string &cmd, User &user, Server &server);
void	normNameChannel(std::string &channel, User &user, Server &server);
void	interpretCommand(Server &server, std::string strmess, int const &id);
void	msgError(std::string const &code, User &user, std::string const &msg);
void	protocolForJoinChannel(Channel *channel, User &user, std::string &key);
std::vector<std::string> splitString(const std::string &input, char delimiter);
void	messageToAllUsersInChannel(Channel *channel, User &user, int createOrJoin);
Channel	*findChannelByName(std::vector<Channel *> &channels, std::string const &cmd);
void	joinOrCreatChannel(std::string &cmd, User &user, Server &Server, std::string &key);
void	sendForCreate(std::vector<std::string> &channels, User &user, Server &server, std::vector<std::string> &keys);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   UTILS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> splitString(const std::string &input, char delimiter);
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   INVITE                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/*model de message d'erreur de la commande INVITE
":127.0.0.1 403 " + user->nickname + " #" + channel + " :No such channel\r\n"
*/
#define ERRORI461 " :Not enough parameters" //"<client> :Not enough parameters"
#define ERRORI403 " :No such channel" //  "<client> <channel> :No such channel"
#define ERRORI442 " :You're not on that channel" //"<client> <channel> :You're not on that channel"
#define ERRORI482 " :You're not channel operator" //"<client> <channel> :You're not channel operator"
#define ERRORI443 " :is already on channel" //"<client> <nick> <channel> :is already on channel"

/*class inviteException : public std::exception
{
	public:
		virtual const char* what() const throw();
};
class notEnoughParamException : public std::exception
{
	public:
		virtual const char* what() const throw();
};*/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               Commandes.CPP                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  PART                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  KICK                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  QUIT                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  PRIVMSG                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  TOPIC                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  NICK                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  MODE                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
