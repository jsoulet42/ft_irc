# include "./includes/ft_irc.hpp"

void irc_quit(std::string &message, User &user, Server &server)
{
	std::cout << "on est dans irc_quit" << std::endl;
	std::string mess;
	if (message.size() > 4 && message[4] == ':')
		mess = message.substr(5, message.size());
	else
		mess = message.substr(4, message.size());
	std::string msg;
	if (mess.empty())
		msg = "";
	else
		msg = message;
	std::string rpl_quit2 = "ERROR :Connection timeout\r\n";
	std::string rpl_quit = ":" + user.nickname + "-!" + user.nickname[0] + "@" + user.nickname + " QUIT :" + msg + "\r\n";
	send(user._fdUser, rpl_quit2.c_str(), rpl_quit2.size(), 0);
	for (std::vector<Channel *>::iterator it = server.channels.begin(); it != server.channels.end(); ++it)
	{
		if (findUserInChannel(*it, &user) == true)
		{
			std::cout << "on y est " << std::endl;
			(*it)->channelSendLoop(rpl_quit, user._fdUser);
			(*it)->nbUsers--;
			(*it)->deleteUserInChannel(user);
			(*it)->deleteUserInOperator(&user);
		}
	}
	deleteChannelIfEmpty(server);
	for (std::vector<struct pollfd>::iterator it2 = server.fdP.begin(); it2 != server.fdP.end(); ++it2)
	{
		if ((*it2).fd == user._fdUser)
		{
			close((*it2).fd);
			(*it2).fd = -1;
		}
	}
	server.deleteUserByNick(user.nickname);
}
