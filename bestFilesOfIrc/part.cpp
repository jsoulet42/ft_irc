
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
}
