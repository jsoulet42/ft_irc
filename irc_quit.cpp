# include "includes/ft_irc.hpp"

void irc_quit(std::string &message, User &user, Server &server)
{
	std::cout << "on est dans irc_quit" << std::endl;
	std::string mess;
	std::string str;
	if (message.size() > 4 && message[4] == ':')
		mess = message.substr(5, message.size());
	else
		mess = message.substr(4, message.size());
	std::string msg;
	if (mess.empty())
		msg = "Quit";
	else
		msg = message;
	checkIFUserIsInChannelAndSendQuitMessage(user, server);
	for (std::vector<struct pollfd>::iterator it2 = server.fdP.begin(); it2 != server.fdP.end(); ++it2)
	{
		if ((*it2).fd == user._fdUser)
		{
			close((*it2).fd);
			(*it2).fd = -1;
		}
	}
}

void checkIFUserIsInChannelAndSendQuitMessage(User &user, Server &server)
{
	std::string rpl_part;
	std::string rpl_quit;

	for (std::vector<Channel *>::iterator it = server.channels.begin(); it != server.channels.end(); ++it)
	{
		if (findUserInChannel(*it, &user) == true)
		{
			rpl_part = "PART #" + (*it)->name + "\r\n";
			ircPart(rpl_part, user, server);
			rpl_part = ("");
			rpl_quit = ":" + user.nickname + " QUIT :Quit: " + "\r\n";
			(*it)->channelSendLoop(rpl_quit, user._fdUser);
			rpl_quit = ("");
		}
	}
}
