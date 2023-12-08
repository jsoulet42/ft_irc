# include "./includes/ft_irc.hpp"

void irc_quit(std::string message, User *user, Server *server)
{
	std::cout << YELLOW <<  ON_BLACK << "[COMMAND]QUIT" << RESET << std::endl;

	message = message.substr(6); // Remove the command "QUIT"
	std::string msg;
	if (message.empty())
	{
		msg = "Quit";
	}
	else
	{
		msg = message;
	}

	std::string rpl_quit = ":" + user->nickname + " QUIT :Quit: " + msg + "\r\n";

	for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
	{
		if ((*it)->isInChannel(user) || (*it)->isOpInChannel(user))
		{
			(*it)->channelSendLoop(rpl_quit, user->fd, server, 0);
			(*it)->nbUsers--;
			int rc = (*it)->deleteChannelUser(user, server);
			if (rc == 1)
			{
				std::string name = (*it)->name;
				delete (*it);
				std::cout << BRIGHT_YELLOW << ON_BLACK << "[DELETE] : Succesfully deleted channel #" << name << RESET << std::endl;
				return;
			}
			else
			{
				(*it)->printChannelUsers(QUIT);
			}
		}
	}

	for (std::vector<struct pollfd>::iterator it4 = server->fds.begin(); it4 != server->fds.end(); ++it4)
	{
		if ((*it4).fd == user->fd)
		{
			close((*it4).fd);
			(*it4).fd = -1;
		}
	}
	delete user;
}