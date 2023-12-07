
# include "./includes/ft_irc.hpp"

//Commande: PRIVMSG
//Paramètres: <destinataire>{,<destinataire>} <texte à envoyer >

void ircPrivmsg(std::string &msg, User &user, Server &Server)
{
	ssize_t i = 0;
	std::cout << YELLOW <<  ON_BLACK << "[COMMAND]\t[PRIVMSG]" << msg << RESET << std::endl;
	msg = msg.substr(8); // Remove the command "PRIVMSG "
	if (msg[0] == '\0')
	{
		// message vide
		msgError("461", user, ERRORJ461);
		return;
	}
	i = msg.find(' ');
	if (i == -1) // n'a pas trouve d'espace et donc de message a envoyer
	{
		std::cout << RED << ON_BLACK << "Usage: /PRIVMSG destinataire(un ou plusieurs separes par une virgule sans espace) message" << RESET << std::endl;
		return;
	}
	std::string messageToSend = msg.substr(i + 1, msg.size());
	std::string listOfUser = msg.substr(0, i);
	i = listOfUser.find(',');
	if (i == -1) // un seul user a qui envoye le message
	{
		for (std::vector<User *>::iterator it = Server.users.begin(); it != Server.users.end(); it++)
		{
			if (listOfUser == (*it)->nickname)
			{
				//le user existe je lui envoie le msg
				std::string rpl_privmsg = ":" + user.nickname + " PRIVMSG " + (*it)->nickname + " :" + messageToSend + "\r\n";
				send(user._fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
				//send_log((*it)->fd, rpl_privmsg, Server);
				return;
			}
		}
		// je previens l'envoyeur que le msg n'est pas parvenu parce que le pseudo n'existe pas
		std::string err_no_such_nick = "127.0.0.1 401 " + user.nickname + " " + listOfUser + " :No such nickname\r\n";
		send(user._fdUser, err_no_such_nick.c_str(), err_no_such_nick.length(), 0);
		//send_log(user._fdUser, err_no_such_nick, Server);
		return;
	}
	else
	{
		std::string reste;
		std::string destinator;
		std::vector<User *>::iterator it;
		while (i != -1)
		{
			reste = listOfUser.substr(i + 1, listOfUser.size());
			destinator = listOfUser.substr(0, i);
			for (it = Server.users.begin(); it != Server.users.end(); it++)
			{
				if (destinator == (*it)->nickname)
				{
					//le destinator existe je lui envoie le msg
					std::string rpl_privmsg = ":" + user.nickname + " PRIVMSG " + (*it)->nickname + " :" + messageToSend + "\r\n";
					send(user._fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
					//send_log((*it)->fd, rpl_privmsg, Server);
					return;
				}
			}
			// je previens l'envoyeur que le msg n'est pas parvenu parce que le pseudo n'existe pas
			std::string err_no_such_nick = "127.0.0.1 401 " + user.nickname + " " + listOfUser + " :No such nickname\r\n";
			send(user._fdUser, err_no_such_nick.c_str(), err_no_such_nick.length(), 0);
			listOfUser = reste;
			i = listOfUser.find(',');
		} // je refait le message pour le dernier user
		for (it = Server.users.begin(); it != Server.users.end(); it++)
		{
			if (listOfUser == (*it)->nickname)
			{
				//le user existe je lui envoie le msg
				std::string rpl_privmsg = ":" + user.nickname + " PRIVMSG " + (*it)->nickname + " :" + messageToSend + "\r\n";
				send(user._fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
				//send_log((*it)->fd, rpl_privmsg, Server);
				return;
			}
		}
	}
}
