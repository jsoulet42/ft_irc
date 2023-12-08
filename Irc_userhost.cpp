# include "./includes/ft_irc.hpp"

void	irc_userhost(std::string message, User *user, Server *server)
{
	std::string copy = message.substr(9); // j'enleve de USERHOST et l'espace.
	std::string rpl_userhost;

	std::vector<User *>::iterator it = server->users.begin();
	std::vector<User *>::iterator ite = server->users.end();
	while (it != ite)
	{
		if ((*it)->nickname == user->nickname)
		{
			rpl_userhost = user->nickname + " is connected\r\n";
			return;
		}
		++it;
	}
	rpl_userhost = user->nickname + " not found\r\n";
	return;
}