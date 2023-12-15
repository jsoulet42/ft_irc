# include "./includes/ft_irc.hpp"

void irc_quit(std::string &message, User &user, Server &server)
{
	message = message.substr(6); // Remove the command "QUIT"
	std::string msg;
	if (message.empty())
		msg = "Quit";
	else
		msg = message;
	std::string rpl_quit = ":" + user.nickname + " QUIT :Quit: " + msg + "\r\n";
	for (std::vector<Channel *>::iterator it = server.channels.begin(); it != server.channels.end(); ++it)
	{
		if (findUserInChannel(*it, &user) == true || checkRightsUserInChannel(*it, &user, OPERATOR) == true)
		{
			(*it)->channelSendLoop(rpl_quit, user._fdUser);
			(*it)->nbUsers--;
			(*it)->deleteUserInChannel(&user);
			(*it)->deleteUserInOperator(&user);
		}
	}
	for (std::vector<struct pollfd>::iterator it2 = server.fdP.begin(); it2 != server.fdP.end(); ++it2)
	{
		if ((*it2).fd == user._fdUser)
		{
			close((*it2).fd);
			(*it2).fd = -1;
		}
	}
}
