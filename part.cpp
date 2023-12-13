
#include "includes/ft_irc.hpp"

class Irc_part_error : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class irc_part_rpl : public std::exception
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
			reason = message.substr(message.find(":") + 1, message.size());
		else
			reason = user.nickname + " has left the channel";
	}
	else
	{
		channel.push_back(message);
		reason = user.nickname + " has left the channel";
	}
	if (chan.find(",") != std::string::npos)
	{
		while (chan.find(",") != std::string::npos)
		{
			channel.push_back(chan.substr(0, chan.find(",")));
			chan.erase(0, chan.find(",") + 1);
			std::cout << channel.back() << std::endl;
		}
	}
	else
		channel.push_back(chan);
	(void)server;
	/*
	sendPartToAllUsersInChannel(channel, &user, reason, server);
	throw irc_part_rpl();*/
}

void sendPartToAllUsersInChannel(std::vector<std::string> channel, User *user, std::string reason, Server server)
{
	std::stringstream rpl_part;
	std::stringstream err_part;

	for (std::vector<std::string>::iterator it = channel.begin(); it != channel.end(); it++)
	{
		Channel *chan = findChannelByName(server.channels, *it);
		if (chan == NULL)
		{
			err_part << IPHOST << " 403 " << user->nickname << " " << *it << " :No such channel\r\n";
			send(user->_fdUser, err_part.str().c_str(), err_part.str().length(), 0);
			err_part.str("");
			continue;
		}
		if (findUserInChannel(chan, user) == false)
		{
			err_part << IPHOST << " 442 " << user->nickname << " " << *it << " :You're not on that channel\r\n";
			send(user->_fdUser, err_part.str().c_str(), err_part.str().length(), 0);
			err_part.str("");
			continue;
		}
		for (std::vector<User *>::iterator cuser = chan->users.begin(); cuser < chan->users.end(); cuser++)
		{
			if ((*cuser)->_fdUser != user->_fdUser)
			{
				std::cout << (*cuser)->_fdUser << user->_fdUser << std::endl;
				rpl_part << ":" << user->nickname << " PART #" << chan->name << " :" << reason << "\r\n";
				send((*cuser)->_fdUser, rpl_part.str().c_str(), rpl_part.str().length(), 0);
				rpl_part.str("");
				if (checkRightsUserInChannel(chan, *cuser, OPERATOR) == true)
					inheritanceOperator(chan, *cuser);
				else
					chan->operators.erase(*cuser);
			}
		}
		std::vector<User *>::iterator it2 = chan->users.begin();
		for (; it2 != chan->users.end(); it2++)
		{
			if ((*it2)->_fdUser == user->_fdUser)
				break;
		}
		chan->users.erase(it2);
		chan->nbUsers--;
		std::cout << chan->users[0]->nickname << " first client dans /PART" << std::endl;
		std::cout << chan->users[1]->nickname << " second client dans /PART" << std::endl;
		exit(0);
	}
}

void inheritanceOperator(Channel *chan, User *user)
{
	std::cout << "inheritanceOperator" << std::endl;
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

const char* irc_part_rpl::what() const throw()
{
	return ("IRC_PART - rpl");
}
