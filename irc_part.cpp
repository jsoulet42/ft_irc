# include "includes/ft_irc.hpp"
//# include "./includes/User.hpp"

int	deleteChannelUser(Channel *channel, User *user, Server *server); // a mettre dans ft_irc.hpp

void irc_part(std::string &message, User &user, Server &server)
{

	//std::cout << YELLOW <<  ON_BLACK << "PART : " << message << user.nickname << server.fdNb << RESET << std::endl;
	//exemple : "PART #nom_du_channel :message"
	int rc;
	std::string channelName;
	std::string reason;
	std::cout << YELLOW <<  ON_BLACK << "[COMMAND]\tPART" << RESET << std::endl;

	message = message.substr(6); // Remove the command "PART #"
	std::string rpl_part;
	ssize_t pos = message.find(" :");
	if (pos == -1)
	{
		reason = "No reason but " + user.nickname + " have quit this channel";
		channelName = message;
	}
	else
	{
		reason = message.substr(pos + 2, message.size());
		channelName = message.substr(0, pos);
	}
	rpl_part = ":" + user.nickname + " PART #" + channelName + " :" + reason + "\r\n";
	std::vector<Channel *>::iterator it = server.channels.begin();
	while (it != server.channels.end())
	{
		if ((*it)->name == channelName)
		{
			//(*it)->channelSendLoop(rpl_part, user->fd, server, 1);
			for (std::vector<User *>::iterator it2 = (*it)->users.begin(); it2 != (*it)->users.end(); it2++)
				{
					if ((*it2)->_fdUser != user._fdUser)
						send((*it2)->_fdUser, rpl_part.c_str(), rpl_part.length(), 0);
				}
			(*it)->nbUsers--;
			//irc_names((*it), user, server);
			messageToAllUsersInChannel((*it), user, 0);
			rc = deleteChannelUser((*it), &user, &server);
			if (rc == 1)
			{
				delete (*it);
				std::cout << BRIGHT_YELLOW << ON_BLACK << "[DELETE] : Succesfully deleted channel #" << channelName << RESET << std::endl;
				return;
			}
		}
		++it;
	}
}

// a ajouter a utils

int	deleteChannelUser(Channel *channel, User *user, Server *server)
{
	(void)server;
	std::vector<User *>::iterator	it = channel->users.begin();
	std::vector<User *>::iterator	ite = channel->users.end();

	// ce if passe l'operateur quittant le channel en non operateur
	if (checkRightsUserInChannel(channel, user) == OPERATOR)
		channel->operators[user] = false;

	while (it != ite)
	{
		if (user == (*it))
		{
			std::cout << "il faut faire une fonction pour delete quand find user sera modifiee" << std::endl;
			//if (findUser(*user, channel->users))
			//{
			//	channel->users.erase(it);
			//	std::cout << BRIGHT_YELLOW << ON_BLACK << "[DELETE] : Erasing user " << user->nickname << " from #" << channel->name << RESET << std::endl;
			//	break;
			//}
			//else if (channel->isOpInChannel(user))
			//{
			//	channel->operators.erase(it);
			//	std::cout << BRIGHT_YELLOW << ON_BLACK << "[DELETE] : Erasing operator " << user->nickname << " from #" << channel->name << RESET << std::endl;
			//	break;
			//}
		}
		it++;
	}
	//if (channel->operators.empty() && !channel->users.empty())
	//	channel->allocNewOp(server);
	//else if (channel->operators.empty() && channel->users.empty())
	//{
	//	std::cout << BRIGHT_YELLOW << ON_BLACK << "[DELETE] : Channel " << channel->name << " is empty, erasing it from server..." << RESET << std::endl;
	//	server->channels.erase(server->getChannelByName(channel->name));
	//	return 1;
	//}
	return 0;
}
