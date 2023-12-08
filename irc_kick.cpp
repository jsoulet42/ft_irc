# include "./includes/ft_irc.hpp"


void	irc_kick(std::string message, User * user, Server * server)
{

	std::cout << YELLOW <<  ON_BLACK << "KICK : " << message << user->nickname << server->fdNb << RESET << std::endl;

	//std::istringstream iss(message);
	//std::string word;
	//std::getline(iss, word, ' '); // Read until the first space

	//std::string rpl_kick = word;
	//bool hasReason = (rpl_kick.find(':') != std::string::npos);

	//std::getline(iss, word, ' '); // Read until the next space
	//std::string channelName = word.substr(1);

	//std::getline(iss, word, ' '); // Read until the next space
	//std::string kickedUser = word;

	//std::string reason = "Kicked by operator";
	//if (channelName.empty() || channelName == kickedUser)
	//{
	//	std::string rpl_error = ":127.0.0.1 461 " + user->nickname + " KICK :Not enough parameters\r\n";
	//	send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
	//	send_log(user->fd, rpl_error, server);
	//	return;
	//}
	//std::vector<Channel *>::iterator it = server->getChannelByName(channelName);
	//if (it == server->channels.end())
	//{
	//	std::string rpl_error = ":127.0.0.1 403 " + user->nickname + " #" + channelName + " :No such channel\r\n";
	//	send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
	//	send_log(user->fd, rpl_error, server);
	//	return;
	//}// le channel existe
	//if (!(*server->getChannelByName(channelName))->isOpInChannel((*server->getUserByName(kickedUser))) && !(*server->getChannelByName(channelName))->isInChannel((*server->getUserByName(kickedUser))))
	//{
	//	std::string rpl_error = ":127.0.0.1 403 " + user->nickname + " " + kickedUser + " #" + channelName + " :They aren't on that channel\r\n";
	//	send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
	//	send_log(user->fd, rpl_error, server);
	//	return;
	//}// la cible existe sur ce channel
	//if (!(*server->getChannelByName(channelName))->isOpInChannel(user) && !(*server->getChannelByName(channelName))->isInChannel(user))
	//{
	//	std::string rpl_error = ":127.0.0.1 442 " + user->nickname + " #" + channelName + " :You're not on that channel\r\n";
	//	send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
	//	send_log(user->fd, rpl_error, server);
	//	return;
	//}// le demandeur existe sur le channel
	//if (!(*it)->isOpInChannel(user) || !(*it)->isBestOp(user, (*it)->getUserByNick(kickedUser)))
	//{
	//	std::cout << ERROR << user->nickname << " is not operator or have less privileges than " << kickedUser << ", can't kick it." << RESET << std::endl;
	//	std::string rpl_error = ":127.0.0.1 482 " + user->nickname + " #" + channelName + " :You're not channel operator\r\n";
	//	send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
	//	send_log(user->fd, rpl_error, server);
	//	return;
	//} // le demandeur est op et a plus de droit que la cible
	//if (hasReason)
	//{
	//	std::getline(iss, reason);
	//}

	//rpl_kick = ":" + user->nickname + " KICK #" + channelName + " " + kickedUser + " :" + reason + "\r\n";
	//(*it)->channelSendLoop(rpl_kick, user->fd, server, 1);
	//int rc = (*it)->deleteChannelUser((*it)->getUserByNick(kickedUser), server);
	//if (rc == 1)
	//{
	//	std::string name = (*it)->name;
	//	delete (*it);
	//	std::cout << BRIGHT_YELLOW << ON_BLACK << "[DELETE] : Succesfully deleted " << kickedUser << " by " << user->nickname << RESET << std::endl;
	//}
	//else
	//{
	//	std::cout << RED << ON_BLACK << "[ERROR] : " << kickedUser << " hasn't be deleted for reasons" << RESET << std::endl;
	//}
}
