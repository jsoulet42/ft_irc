
# include "ft_irc.hpp"
# include "Server.hpp"
# include "User.hpp"
# include "Channel.hpp"

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

class joinacceptedException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

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

bool	checkRightsUserInChannel(Channel *channel, User *user)
{
	for(std::vector<User *>::iterator it = channel->invitedUsers.begin(); it != channel->invitedUsers.end(); ++it)
	{
		if ((*it)->_fdUser == user->_fdUser)
			return true;
	}
	return false;
}

void	msgError(std::string const &code, User &user, std::string const &msg)
{
	std::stringstream ss;
	ss << IPHOST << code << " " << user.nickname << msg;
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
	if(strmess.compare(0, 5, "JOIN ") == 0)
	{
		ircJoin(strmess, *user, server);
	}
	/*if (strmess.compare(0, 6, "INVITE") == 0)
	{
		ircInvite(strmess, *user, server);
	}*/

}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  JOIN                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//fait par julien le 02/12/2023
void ircJoin(std::string &msg, User &user, Server &Server)
{
	std::string cmd = strtok((char *)msg.c_str() + 5, "\r\n");
	if (cmd.size() == 0)
		msgError("461", user, ERRORJ461);
	if (errorCmd == true)
		throw joinException();
	parseCmd(cmd, user, Server);
}

//fait par julien le 02/12/2023
void parseCmdWithNoKey(std::string &cmd, User &user, Server &server)
{
	std::string channel;
	std::string key;

	if (cmd.find(',') == std::string::npos)
	{
		channel = cmd.substr(0, cmd.find('\r'));
		normNameChannel(channel, user, server);
		joinOrCreatChannel(channel, user, server, key);
		throw joinacceptedException();

	}
	if (cmd.find(' ') == std::string::npos)
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
	}
}

//fait par julien le 03/12/2023
void normKey(std::string &key, User &user, Server &server)
{
	(void)server;
	if (key.size() > 10 || key.size() < 1)
		msgError("475", user, ERRORJ475);
	for (size_t i = 0; i < key.size(); ++i)
	{
		char c = key[i];
		if (c < 32 || c == 127 || c == 9 || c == 10 || c == 13)
			msgError("475", user, ERRORJ475);
	}
	if (errorCmd == true)
		throw keyException();
}

//fait par julien le 03/12/2023
void normNameChannel(std::string &channel, User &user, Server &server)
{
		(void)server;
	if (channel.compare(0, 1, "#") != 0)
			msgError("403", user, ERRORJ403);
	else if (channel.size() < 2)
		msgError("461", user, ERRORJ461);
	else if (channel.size() > 10)
		msgError("476", user, ERRORJ476);
	if (errorCmd == true)
			throw joinException();
	channel.erase(0, 1);
}

// /JOIN #channel1,chanel2,chanel3, key1\r\n
//fait par julien le 02/12/2023
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
	// #channel1,chanel2,chanel3 key1,key2,key3\r\n
	// ici on a un tableau de channels et un tableau de keys remplis
	// faire une boucle qui parcours les deux tableaux et qui fait joinOrCreatChannel
	// pour chaque channel avec la key correspondante si elle existe sinon avec une key vide
}

void sendForCreate(std::vector<std::string> &channels, User &user, Server &server, std::vector<std::string> &keys)
{
	size_t i;
	std::string keyEmpty = "";

	for (i = 0; i < keys.size() || i < channels.size(); ++i)
	{
		joinOrCreatChannel(channels[i], user, server, keys[i]);
	}
	for (; i < channels.size(); ++i)
	{
		joinOrCreatChannel(channels[i], user, server, keyEmpty);
	}
}

void protocolForJoinChannel(Channel *channel, User &user, std::string &key)
{
	if (!checkRightsUserInChannel(channel, &user))
			msgError("473", user, ERRORJ473);
	channel->ft_checkMode(channel, user);
	if (findUserInChannel(channel, &user))
		throw Channel::UserIsAlredyInChannelException();
	else if (channel->addUser(&user, key) == -1)
		msgError("475", user, ERRORJ475);
	if (errorCmd == true)
		throw joinException();
}

//fait par julien le 02/12/2023
// prevoir a implementer les erreurs manquantes
void joinOrCreatChannel(std::string &cmd, User &user, Server &server, std::string &key)
{
	Channel *channel = findChannelByName(server.channels, cmd);

	if (channel)
	{
		protocolForJoinChannel(channel, user, key);
		channel->addUser(&user, key);
		messageToAllUsersInChannel(channel, user, 0);
	}
	else
	{
		channel = new Channel(&user, cmd);
		channel->password = key;
		channel->addUser(&user, key);
		server.channels.push_back(channel);
		messageToAllUsersInChannel(channel, user, 1);
	}
}

//fait par julien le 02/12/2023
void messageToAllUsersInChannel(Channel *channel, User &user, int createOrJoin)
{
	std::stringstream ss;

	if (createOrJoin == 0)
	{
		ss << IPHOST << "JOIN " << channel->name << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "332 " << user.nickname << " " << channel->name << " :" << channel->topic << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "353 " << user.nickname << " = " << channel->name << " :";
		for (std::vector<User *>::iterator it = channel->users.begin(); it != channel->users.end(); ++it)
			ss << (*it)->nickname << " ";
		ss << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "366 " << user.nickname << " " << channel->name << " :End of /NAMES list.\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	}
	else if (createOrJoin)
	{
		ss << ":" << user.nickname << " JOIN #" << channel->name << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		/*ss << IPHOST << "332 " << user.nickname << " " << channel->name << " :" << channel->topic << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");*/
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

const char* joinacceptedException::what() const throw()
{
	return "[RPL] during JOIN command , user is accepted on channel";
}
