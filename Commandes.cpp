
# include "./includes/ft_irc.hpp"

bool errorCmd = false;

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

class joinAcceptedException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class modeException : public std::exception
{
	public:
		virtual const char *what() const throw();
};

void test(Channel channel, std::string nameMode)
{
	channel.checkRights(nameMode);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  TOOLS                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

User	*findUserById(std::vector<User *> &users, int const &id)
{
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->_fdUser == id)
			return (*it);
	}
	return NULL;
}
/// @brief Renvoie un pointeur sur l'utilisateur si il existe, sinon NULL
/// @param users : liste des utilisateurs
/// @param cmd : nom de l'utilisateur
User	*findUserByName(std::vector<User *> &users, std::string const &cmd)
{
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->nickname == cmd)
			return (*it);
	}
	return NULL;
}



Channel	*findChannelByName(std::vector<Channel *> &channels, std::string const &cmd)
{
	std::vector<Channel *>::iterator it = channels.begin();
	for (; it != channels.end(); ++it)
	{
		if ((*it)->name == cmd)
			return (*it);
	}
	return NULL;
}

bool	findUserInChannel(Channel *channel, User *user)
{
	for (std::vector<User *>::iterator it = channel->users.begin(); it != channel->users.end(); ++it)
	{
		if ((*it)->_fdUser == user->_fdUser)
			return true;
	}
	return false;
}

///@brief return the grade of the user in the channel
///@param 3 is OPERATOR or INVITED
int	checkRightsUserInChannel(Channel *channel, User *user, int grade)
{
	if (grade == OPERATOR)
	{
		std::map<User *, bool>::iterator it = channel->operators.find(user);
		if (it != channel->operators.end() && it->second == true)
			return true;
		return false;
	}
	if (grade == INVITED)
	{
		for(std::vector<User *>::iterator it = channel->invitedUsers.begin(); it != channel->invitedUsers.end(); ++it)
		{
			if ((*it)->_fdUser == user->_fdUser)
				return true;
		}
		return false;
	}
	return false;
}

/// <code> <nickname> <msg>
void	msgError(std::string const &code, User &user, std::string const &msg)
{
	std::stringstream ss;
	ss << IPHOST << code << " " << user.nickname << msg;
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	errorCmd = true;
}

void	msgError(std::string const &code, std::string &channel, User &user, std::string const &msg)
{
	std::stringstream ss;
	ss << IPHOST << code << " " << channel << msg;
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	errorCmd = true;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              interpretCmd                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void interpretCommand(Server &server, std::string strmess, int const &id)
{
	User *user = findUserById(server.users, id);
	errorCmd = false;

	if (strmess.compare(0, 5, "PART ") == 0)
		ircPart(strmess, *user, server);
	else if(strmess.compare(0, 5, "JOIN ") == 0)
		ircJoin(strmess, *user, server);
	else if(strmess.compare(0, 8, "PRIVMSG ") == 0)
		ircPrivmsg(strmess, *user, server);
	else if (strmess.compare(0, 5, "QUIT ") == 0)
		irc_quit(strmess, *user, server);
	else if (strmess.compare(0, 5, "MODE ") == 0)
		ft_launchMode(strmess, *user, server);
	else if (strmess.compare(0, 5, "TOPIC") == 0)
		irc_topic(strmess, *user, server);
	else if (strmess.compare(0, 5, "KICK ") == 0)
		ircKick(strmess, *user, server);
	else if (strmess.compare(0, 6, "INVITE") == 0)
		ircInvite(strmess, *user, server);
	else if (strmess.compare(0, 3, "WHO") == 0)
		irc_who(strmess, *user, server);
	else if (strmess.compare(0, 9, "USERHOST ") == 0)
		irc_userhost(strmess, *user, server);
	else
		return;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  JOIN                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


void ircJoin(std::string &msg, User &user, Server &Server)
{
	std::string cmd = strtok((char *)msg.c_str() + 5, "\n");

	if (cmd.size() == 0)
		msgError("461", user, ERRORJ461);
	if (errorCmd == true)
		throw joinException();
	parseCmd(cmd, user, Server);
}

void parseCmdWithNoKey(std::string &cmd, User &user, Server &server)
{
	std::string channel;
	std::string key;

	if (cmd.find(',') == std::string::npos && cmd.find(' ') == std::string:: npos)
	{
		channel = cmd.substr(0, cmd.find('\r'));
		normNameChannel(channel, user, server);
		joinOrCreatChannel(channel, user, server, key);
		throw joinAcceptedException();

	}
	else if (cmd.find(',') == std::string::npos && cmd.find(' ') != std::string:: npos)
	{
		channel = cmd.substr(0, cmd.find(' '));
		cmd.erase(0, cmd.find(' ') + 1);
		key = cmd.substr(0, cmd.find('\r'));
		normKey(key, user, server);
		normNameChannel(channel, user, server);
		joinOrCreatChannel(channel, user, server, key);
		throw joinAcceptedException();
	}
	else if (cmd.find(' ') == std::string::npos && cmd.find(',') != std::string:: npos)
	{
		while (cmd.find(',') != std::string::npos)
		{
			channel = cmd.substr(0, cmd.find(','));
			normNameChannel(channel, user, server);
			joinOrCreatChannel(channel, user, server, key);
			cmd.erase(0, cmd.find(',') + 1);
		}
		channel = cmd.substr(0, cmd.find('\r'));
		normNameChannel(channel, user, server);
		joinOrCreatChannel(channel, user, server, key);
		throw joinAcceptedException();
	}
}

void normKey(std::string &key, User &user, Server &server)
{
	(void)server;
	if (key.size() > 10 || key.size() < 1)
		msgError("475", user, ERRORJ475);
	for (size_t i = 0; i < key.size(); ++i)
	{
		char c = key[i];
		if (c <= 32 || c == 127 || c == 35)
			msgError("475", user, ERRORJ475);
	}
	if (errorCmd == true)
		throw keyException();
}

void normNameChannel(std::string &channel, User &user, Server &server)
{
	(void)server;
	if (channel.size() > 10 || channel.size() < 1)
		msgError("475", user, ERRORJ475);
	for (size_t i = 1; i < channel.size(); i++)
	{
		char c = channel[i];
		if (c <= 97 && c >= 122)
			msgError("475", user, ERRORJ475);
	}
	if (channel.compare(0, 1, "#") != 0)
		msgError("403", user, ERRORJ403);
	else if (channel.size() < 2)
		msgError("461", user, ERRORJ461);
	else if (channel.size() > 10)
		msgError("476", channel, user, ERRORJ476);
	if (errorCmd == true)
		throw joinException();
	channel.erase(0, 1);
}

// /JOIN #channel1,chanel2,chanel3, key1\r\n
void parseCmd(std::string &cmd, User &user, Server &server)
{
	std::vector<std::string> channels;
	std::vector<std::string> keys;
	std::string channel;
	std::string key;
	std::string buffer;

	parseCmdWithNoKey(cmd, user, server);
	channel = cmd.substr(0, cmd.find(' '));
	cmd.erase(0, cmd.find(' ') + 1);
	key = cmd.substr(0, cmd.find('\r'));
	while (channel.find(',') != std::string::npos)
	{
		buffer = channel.substr(0, channel.find(','));
		normNameChannel(buffer, user, server);
		channels.push_back(buffer);
		channel.erase(0, channel.find(',') + 1);
	}
	buffer = channel.substr(0, channel.find(' '));
	normNameChannel(buffer, user, server);
	channels.push_back(buffer);
	if (key.find(',') == std::string::npos)
	{
		buffer = key.substr(0, key.find('\r'));
		normKey(buffer, user, server);
		keys.push_back(buffer);
	}
	else
	{
		while (key.find(',') != std::string::npos)
		{
			buffer = key.substr(0, key.find(','));
			normKey(buffer, user, server);
			keys.push_back(buffer);
			key.erase(0, key.find(',') + 1);
		}
		buffer = key.substr(0, key.find('\r'));
		normKey(buffer, user, server);
		keys.push_back(buffer);
	}
	sendForCreate(channels, user, server, keys);
}

void sendForCreate(std::vector<std::string> &channels, User &user, Server &server, std::vector<std::string> &keys)
{
	size_t i;
	std::string keyEmpty = "";

	for (i = 0; i < keys.size() && i < channels.size(); ++i)
		joinOrCreatChannel(channels[i], user, server, keys[i]);
	for (; i < channels.size(); ++i)
		joinOrCreatChannel(channels[i], user, server, keyEmpty);
}

void protocolForJoinChannel(Channel *channel, User &user, std::string &key)
{
	if (checkMode(channel, "modeI") == true)
	{
		if (checkRightsUserInChannel(channel, &user, INVITED) == false)
		{
			msgError("473", user, ERRORJ473);
			throw joinException();
		}
	}
	if (channel->checkModeL() == false)
	{
		// send();
		throw joinException();
	}
	if (findUserInChannel(channel, &user) == true)
		throw Channel::UserIsAlredyInChannelException();
	else if (channel->addUser(&user, key) == -1)
		msgError("475", user, ERRORJ475);
	if (errorCmd == true)
		throw joinException();
}

bool checkMode(Channel *channel, std::string mode)
{
	std::map<std::string, bool>::iterator it = channel->modeTab.find(mode);
	if (it != channel->modeTab.end() && it->second == true)
		return true;
	return false;
}

void joinOrCreatChannel(std::string &cmd, User &user, Server &server, std::string &key)
{
	Channel *channel = findChannelByName(server.channels, cmd);

	if (channel)
	{
		std::cout << GREEN << ON_BLACK << " try to join channel existing " << RESET << std::endl;
		protocolForJoinChannel(channel, user, key);
		std::cout << GREEN << ON_BLACK << user.nickname << " join channel " << "[" << channel->name << "]" << RESET << std::endl;
		messageToAllUsersInChannel(channel, user, 0);
	}
	else
	{
		std::cout << GREEN << ON_BLACK << "this channel doesn't exist" << RESET << std::endl;
		std::cout << GREEN << ON_BLACK << user.nickname << " create channel " << "[" << cmd << "]" << RESET << std::endl;
		channel = new Channel(&user, cmd);
		channel->password = key;
		channel->addUser(&user, key);
		channel->operators[&user] = true;
		channel->getDateTime();
		server.channels.push_back(channel);
		messageToAllUsersInChannel(channel, user, 1);
	}
}

void messageToAllUsersInChannel(Channel *channel, User &user, int createOrJoin)
{
	std::stringstream ss;

	if (createOrJoin == 0)
	{
		channel->topic = "No topic is set";
		ss << ":" << user.nickname << "!~" << user.nickname[0] << "@" << user.nickname <<  " JOIN #" << channel->name << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "332 " << user.nickname << " #" << channel->name << " :" << channel->topic << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "333 " << user.nickname << " #" << channel->name << " " << channel->getOperator()->nickname << "!~" << channel->getOperator()->nickname[0] << "@" << channel->getOperator()->nickname << channel->creationDate << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		std::map<User *, bool>::iterator it = channel->operators.begin();
		ss << IPHOST << "353 " << user.nickname << " = #" << channel->name << " :";
		for (; it != channel->operators.end(); it++)
		{
			if (it->second == true)
				ss << "@" << it->first->nickname << " ";
			else
				ss << it->first->nickname << " ";
		}
		ss << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "366 " << user.nickname << " #" << channel->name << " :End of /NAMES list.\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		for (std::vector<User *>::iterator it2 = channel->users.begin(); it2 != channel->users.end(); it2++)
		{
			if ((*it2)->_fdUser != user._fdUser)
			{
				ss << ":" << user.nickname << "!" << user.nickname[0] << "@" << user.nickname << " JOIN :#" << channel->name << "\r\n";
				send((*it2)->_fdUser, ss.str().c_str(), ss.str().size(), 0);
				ss.str("");
			}
		}
	}
	else if (createOrJoin)
	{
		ss << ":" << user.nickname << "!~" << user.nickname[0] << "@" << user.nickname << " JOIN #" << channel->name << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "MODE #" << channel->name << " +nt" << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "353 " << user.nickname << " = #" << channel->name << " :@" << user.nickname << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "366 " << user.nickname << " #" << channel->name << " :End of /NAMES list.\r\n";
	}
}

/*void send324(Channel &chan, User user, std::string code)
{
	bool t = false;
	bool o = false;
	bool l = false;
	bool k = false;
	bool i = false;
	std::string str;
	std::stringstream rpl_mess;
	if (chan.modeTab["modeT"] == true);
		str += "T";
	if (chan.modeTab["modeO"] == true);
		str += "O";
	if (chan.modeTab["modeL"] == true);
		str += "L";
	if (chan.modeTab["modeK"] == true);
		str += "K";
	if (chan.modeTab["modeI"] == true);
		str += "I";
	rpl_mess << IPHOST << code << " #" << chan.name << " ";
	send(user._fdUser, rpl_mess.str().c_str(), rpl_mess.str().size(), 0);
}*/

/*void send329(User &user, Channel &chan, std::string timestamp, std::string code)
{

}*/

void ft_launchMode(std::string &strmess, User &user, Server &server)
{
	Channel *chan = NULL;
	std::string str = strtok((char *)strmess.c_str() + 5, "\r\n");
	errorCmd = false;
	if (str.size() == 0)
		msgError("403", user, ERRORM403);
	std::cout << str[0] << std::endl;
	if (str[0] != '#' && str[0] != '&')
  	  msgError("403", user, ERRORM403);
	if (errorCmd == true)
		throw modeException();
	str.erase(0, 1);
	if (str.find(" ") != std::string::npos)
		chan = findChannelByName(server.channels, str.substr(0, str.find(" ")));
	if (!chan)
	{
	//	send(ERRORM403); // "<client> <channel> :No such channel"
		throw modeException();
	}
	if (checkRightsUserInChannel(chan, &user, OPERATOR) == false)
	{
		std::string err_not_op = ":127.0.0.1 482 " + user.nickname + " #" + chan->name + " :You're not channel operator\r\n";
		send(user._fdUser, err_not_op.c_str(), err_not_op.length(), 0);
		throw modeException();
	}
	else
	{
		str.erase(0, (str.find(" ") + 1));
		if (str.empty() == true)
		{
		//	send324(*chan, user, "324"); //   "<client> <channel> <modestring> <mode arguments>..."
			//send329(user, *chan, "9999999999999", "329"); //   "<client> <channel> <creationtime>"
		}
		else
			chan->ft_insertChanMode(str, user, *chan);
	}
}

const char* joinException::what() const throw()
{
	return "[Error] during JOIN command";
}

const char* keyException::what() const throw()
{
	return "[Error] during JOIN command , key is too long";
}

const char* joinAcceptedException::what() const throw()
{
	return "[RPL] during JOIN command , user is accepted on channel";
}

const char* modeException::what() const throw()
{
	return "[Error] during MODE command";
}
