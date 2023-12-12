
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
#include "Color.hpp"
#include <map>

#define BUFFSIZE 1023
#define IPHOST ":127.0.0.1 "
// ERROR P
#define ERRORP421 " 421 PASS :Unknown command\r\n"
#define ERRORP461 " 461 PASS :Not enough parameters\r\n"
#define ERRORP464 " 464 PASS :Password incorrect\r\n"
// ERROR C
#define ERRORC421 " 421 CAP :Unknown command\r\n"
// ERROR N
#define ERRORN421 " 421 NICK :Unknown command\r\n"
#define ERRORN432 " 432 NICK :Erroneus nickname\r\n"
#define ERRORN433 " 433 NICK :Nickname is already in use\r\n"
#define ERRORN431 " 431 NICK :No nickname given\r\n"
// ERROR R
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
#define INVITED 2
#define OPERATOR 1
extern bool isRunning;
extern bool errorCmd;

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

class	User;
class	Server;
class	Channel;

int		checkRightsUserInChannel(Channel *channel, User *user, int grade);
void	irc_part(std::string strmess, User &user, Server server);
void	inheritanceOperator(Channel *chan, User *user);
void	normKey(std::string &key, User &user, Server &server);
void	ircJoin(std::string &msg, User &user, Server &Server);
void	parseCmd(std::string &cmd, User &user, Server &Server);
void	ircPrivmsg(std::string &msg, User &user, Server &Server);
void	irc_who(std::string &message, User &user, Server &server);
void	parseCmdWithNoKey(std::string &cmd, User &user, Server &server);
void	normNameChannel(std::string &channel, User &user, Server &server);
void	interpretCommand(Server &server, std::string strmess, int const &id);
void	msgError(std::string const &code, User &user, std::string const &msg);
void	protocolForJoinChannel(Channel *channel, User &user, std::string &key);
User	*findUserByName(std::vector<User *> &users, std::string const &cmd);
void	interpretCommand(Server &server, std::string strmess, int const &id);
void	msgError(std::string const &code, User &user, std::string const &msg);
void	msgError(std::string const &code, User &user, std::string const &msg);
void	protocolForJoinChannel(Channel *channel, User &user, std::string &key);
void	messageToAllUsersInChannel(Channel *channel, User &user, int createOrJoin);
Channel	*findChannelByName(std::vector<Channel *> &channels, std::string const &cmd);
void	joinOrCreatChannel(std::string &cmd, User &user, Server &Server, std::string &key);
void	sendForCreate(std::vector<std::string> &channels, User &user, Server &server, std::vector<std::string> &keys);
void	sendPartToAllUsersInChannel(std::vector<std::string> channel, User *user, std::string reason, Server server);
void	ircInvite(std::string &msg, User &user, Server &server);


//bool findUser(User &user, std::vector<User *> &userList);
void	irc_userhost(std::string &message, User &user, Server &server);
void	printMessageSendToClient(std::string fonction, User &user, std::string message);
void	irc_topic(std::string &message, User &user, Server &server);
bool	findUserInChannel(Channel *channel, User *user);

void 	printUsersOfAChannel(Channel *chan);
void 	printOperatorsOfAChannel(Channel *chan);
void 	printInvitedUsersOfAChannel(Channel *chan);
bool	findUserInChannel(Channel *channel, User *user);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   UTILS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
std::vector<std::string>	splitString(const std::string &input, char delimiter);
bool						findElement(User const &user, std::vector<User *> &userList);
int							countSpaces(const std::string &str, const char &delimiter);
void						remouveUser(User &user, std::vector<User *> &userList);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   INVITE                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/*model de message d'erreur de la commande INVITE
":127.0.0.1 403 " + user->nickname + " #" + channel + " :No such channel\r\n"
*/
#define ERRORI401 " :No such nick/channel\r\n"			//"<client> <nick> :No such nick/channel"
#define ERRORI403 " :No such channel\r\n" 				//"<client> <channel> :No such channel"
#define ERRORI441 " :They aren't on that channel\r\n"	//"<client> <nick> <channel> :They aren't on that channel"
#define ERRORI442 " :You're not on that channel\r\n"	//"<client> <channel> :You're not on that channel"
#define ERRORI443 " :is already on channel\r\n"			//"<client> <nick> <channel> :is already on channel"
#define ERRORI461 " :Not enough parameters\r\n"			//"<client> :Not enough parameters"
#define ERRORI482 " :You're not channel operator\r\n"	//"<client> <channel> :You're not channel operator"

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

#define DEFAULTMAXUSER 100
#define ERRORM403 ":No such channel"
bool checkMode(Channel *channel, std::string mode);


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                msgError.cpp                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void msgError461(User const &user);
void msgError403(User const &user, std::string const &channel);
void msgError441(User const &user, std::string const &userInvited, Channel const &channel);
void msgError442(User const &user, std::string const &channel);
void msgError443(User const &user, std::string const &userInvited, Channel const &channel);
void msgError482(User const &user, std::string const &channel);
void msgError401(User const &user, std::string const &userInvited);
