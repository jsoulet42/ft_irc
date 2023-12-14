# include "./includes/ft_irc.hpp"


void	irc_kick(std::string &message, User &user, Server &server)
{

	//std::cout << YELLOW <<  ON_BLACK << "KICK : " << message << user.nickname << server.fdNb << RESET << std::endl;

	std::istringstream iss(message);
	std::string word;
	std::getline(iss, word, ' '); // Read until the first space
	
	std::string rpl_kick = word;
	bool hasReason = (rpl_kick.find(':') != std::string::npos);

	std::getline(iss, word, ' '); // Read until the next space
	std::string channelName = word.substr(1);

	std::getline(iss, word, ' '); // Read until the next space
	std::string kickedUser = word;

	std::string reason = "Kicked by operator";
	if (channelName.empty() || channelName == kickedUser)
	{
		std::string rpl_error = ":127.0.0.1 461 " + user.nickname + " KICK :Not enough parameters\r\n";
		send(user._fdUser, rpl_error.c_str(), rpl_error.length(), 0);
		printMessageSendToClient("IRC_KICK - ERR - Not enougth parameters", user, rpl_error);
		return;
	}
	std::vector<Channel *>::iterator it = server.getChannelByName(channelName);
	if (it == server.channels.end())
	{
		std::string rpl_error = ":127.0.0.1 403 " + user.nickname + " #" + channelName + " :No such channel\r\n";
		send(user._fdUser, rpl_error.c_str(), rpl_error.length(), 0);
		printMessageSendToClient("IRC_KICK - ERR - No such channel", user, rpl_error);
		return;
	}// le channel existe
	User *kickedUserPointeur = findUserByName((*it)->users, kickedUser);
	if (kickedUserPointeur != NULL)
	{
		// la cible existe sur ce channel
		if (checkRightsUserInChannel(*it, &user, OPERATOR) == false && findUserInChannel(*it, &user) == false)
		{
			std::string rpl_error = ":127.0.0.1 442 " + user.nickname + " #" + channelName + " :You're not on that channel\r\n";
			send(user._fdUser, rpl_error.c_str(), rpl_error.length(), 0);
			printMessageSendToClient("IRC_KICK - ERR - You're not on that channel", user, rpl_error);
			return;
		}// le demandeur existe sur le channel
		if (checkRightsUserInChannel(*it, &user, OPERATOR) == false)
		{
			std::string rpl_error = ":127.0.0.1 482 " + user.nickname + " #" + channelName + " :You're not channel operator\r\n";
			send(user._fdUser, rpl_error.c_str(), rpl_error.length(), 0);
			printMessageSendToClient("IRC_KICK - ERR - You're not channel operator", user, rpl_error);
			return;
		} // le demandeur est op
		if (hasReason)
		{
			std::getline(iss, reason);
		}
		
		rpl_kick = ":" + user.nickname + " KICK #" + channelName + " " + kickedUser + " :" + reason + "\r\n";
		(*it)->channelSendLoop(rpl_kick, user._fdUser);
		(*it)->deleteUserInChannel(kickedUserPointeur);
		//std::cout << RED << ON_BLACK << "[ERROR] : " << kickedUser << " hasn't be deleted for reasons" << RESET << std::endl;
	}
	else
	{
		std::string rpl_error = ":127.0.0.1 403 " + user.nickname + " " + kickedUser + " #" + channelName + " :They aren't on that channel\r\n";
		send(user._fdUser, rpl_error.c_str(), rpl_error.length(), 0);
		printMessageSendToClient("IRC_KICK - ERR - They aren't on that channel", user, rpl_error);
		return;
	}
}