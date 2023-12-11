
#include "includes/ft_irc.hpp"

class Irc_part_error : public std::exception
{
	public:
		virtual const char* what() const throw();
};

void irc_part(std::string strmess, User &user, Server server)
{
	std::string message = strtok((char *)strmess.c_str() + 5, "\r\n");
	std::string chan;
	std::vector<std::string> channel;
	std::string reason;
	std::stringstream err_part;

	if (message.compare(0, 1, "#") == 0)
		message.erase(0, 1);
	else
	{
		err_part << IPHOST << " 461 " << user.nickname << " PART :Not enough parameters\r\n";
		send(user._fdUser, err_part.str().c_str(), err_part.str().length(), 0);
		printMessageSendToClient("IRC_PART - err_part", user, err_part.str());
		err_part.str("");
		throw Irc_part_error();
	}
	if (message.empty())
	{
		err_part << IPHOST << " 461 " << user.nickname << " PART :Not enough parameters\r\n";
		send(user._fdUser, err_part.str().c_str(), err_part.str().length(), 0);
		printMessageSendToClient("IRC_PART - err_part", user, err_part.str());
		err_part.str("");
		throw Irc_part_error();
	}
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
	sendPartToAllUsersInChannel(channel, &user, reason, server);
}

void sendPartToAllUsersInChannel(std::vector<std::string> channel, User *user, std::string reason, Server server)
{
	std::stringstream rpl_part;
	std::stringstream err_part;

	for (std::vector<std::string>::iterator it = channel.begin(); it != channel.end(); ++it)
	{
		Channel *chan = findChannelByName(server.channels, *it);
		if (chan == NULL)
		{
			err_part << IPHOST << " 403 " << user->nickname << " " << *it << " :No such channel\r\n";
			send(user->_fdUser, err_part.str().c_str(), err_part.str().length(), 0);
			printMessageSendToClient("IRC_PART - err_part", *user, err_part.str());
			err_part.str("");
			continue;
		}
		if (findUserInChannel(chan, user) == true)
		{
			for (std::vector<User *>::iterator cuser = chan->users.begin(); cuser != chan->users.end(); cuser++)
			{
				if ((*cuser)->_fdUser != user->_fdUser)
				{
					rpl_part << IPHOST << user->nickname << " PART " << "#" << chan->name << " :" << reason << "\r\n";
					send((*cuser)->_fdUser, rpl_part.str().c_str(), rpl_part.str().length(), 0);
					printMessageSendToClient("IRC_PART - message sur #chan avec user", (*(*cuser)), rpl_part.str());
					rpl_part.str("");
					chan->users.erase(cuser);
					if (checkRightsUserInChannel(chan, *cuser, OPERATOR) == true)
						inheritanceOperator(chan, *cuser);
					else
						chan->operators.erase(*cuser);
				}
			}
		}
		else
		{
			err_part << IPHOST << " 442 " << user->nickname << " " << *it << " :You're not on that channel\r\n";
			send(user->_fdUser, err_part.str().c_str(), err_part.str().length(), 0);
			printMessageSendToClient("IRC_PART - err_part", *user, err_part.str());
			err_part.str("");
			continue;
		}
	}
}

void inheritanceOperator(Channel *chan, User *user)
{
	chan->operators.erase(user);
	std::map<User *, bool>::iterator it = chan->operators.begin();
	if (it == chan->operators.end())
		return;
	for (; it->second == false && it != chan->operators.end(); it++)
		;
	if (it == chan->operators.end())
		chan->operators.begin()->second = true;
	else
		return;
}


const char* Irc_part_error::what() const throw()
{
	return ("IRC_PART - error");
}
