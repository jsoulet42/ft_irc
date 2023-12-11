
#include "includes/ft_irc.hpp"

void irc_part(std::string strmess, User *user, Server server)
{
	std::string message = strtok((char *)strmess.c_str() + 5, "\r\n");
	std::string chan;
	std::vector<std::string> channel;
	std::string reason;

	if (message.find(" ") != std::string::npos)
	{
		chan = message.substr(0, message.find(" "));
		message.erase(0, message.find(" ") + 1);
		if (message.find(":") != std::string::npos && message.find(":") < message.size())
			reason = message.substr(message.find(":"), message.size());
		else
			reason = "";
	}
	else
	{
		channel.push_back(message);
		reason = "";
	}
	if (chan.find(",") != std::string::npos)
		channel = splitString(chan, ',');
	else
		channel.push_back(chan);
	for (std::vector<std::string>::iterator it = channel.begin(); it != channel.end(); ++it)
	{
		Channel *chan = findChannelByName(server.channels, *it);
		if (findUserInChannel(chan, user) == true)
		{
			//on boucle un message de depart pour chaque channel
		}
	}
}

/*for (std::vector<User *>::iterator it2 = chan->users.begin(); it2 != chan->users.end(); it2++)
{
	std::cout << "envoye : |" << rpl_privmsg  << "| sur : |" << (*it2)->_fdUser << "|" << std::endl;
	if ((*it2)->_fdUser != user._fdUser)
	{
		//std::cout << "envoye dans if : |" << rpl_privmsg  << "| sur : |" << (*it2)->_fdUser << "|" << std::endl;
		send((*it2)->_fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
		printMessageSendToClient("IRC_PRIVMSG - message sur #chan avec user", (*(*it2)), rpl_privmsg);
	}
}*/
