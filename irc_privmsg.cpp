
# include "./includes/ft_irc.hpp"

// a rajouter a ft_irc.hpp
#define ERROR403 " :No such channel\r\n"
#define ERROR461 " JOIN :Not enough parameters\r\n"
Channel	*findChannelByName(std::vector<Channel *> &channels, std::string const &cmd);

void printMessageSendToClientPrivMsg(std::string fonction, User &user, std::string message)
{
	std::cout << "J'ai envoye au client le message : |" << message << "| de |" << user.nickname << "| pour la fonction |" << fonction << "|" << std::endl;
}

class Irc_privmsg_error : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class Irc_privmsg_rpl : public std::exception
{
	public:
		virtual const char* what() const throw();
};

const char* Irc_privmsg_error::what() const throw()
{
	return "Error during IRC_PRIVMSG command";
}

const char* Irc_privmsg_rpl::what() const throw()
{
	return "RPL during IRC_PRIVMSG command";
}

void ircPrivmsg(std::string &msg, User &user, Server &Server)
{
	ssize_t i = 0;
	std::cout << "message recu dans IRC_PRIVMSG |" << msg << "|" << std::endl;
	msg = msg.substr(8); // Remove the command "PRIVMSG "
	if (msg[0] == '\0')
	{
		// message vide
		msgError("461", user, ERROR461);
		throw Irc_privmsg_error();
	}
	i = msg.find("\r\n");
	i = msg.find(" :");
	if (i == -1) // n'a pas trouve d'espace et donc de message a envoyer
	{
		std::cout << RED << ON_BLACK << "Usage: /PRIVMSG destinataire(un ou plusieurs separes par une virgule sans espace) :message (ne pas oublier les :)" << RESET << std::endl;
		throw Irc_privmsg_error();
	}
	std::string messageToSend = msg.substr(i + 2, msg.size());
	std::string rpl_privmsg;
	std::string listOfUser = msg.substr(0, i);
	i = listOfUser.find(',');
	if (i == -1) // un seul user a qui envoye le message
	{
		if (listOfUser[0] == '#')
		{
			// le message est destine aux utilisateurs d'un channel
			// je verifie que le channel existe
			Channel *chan = findChannelByName(Server.channels, listOfUser.substr(1, listOfUser.size()));
			if (chan)
			{
				// je modifie le message pour la norme
				rpl_privmsg = ":" + user.nickname + " PRIVMSG #" + chan->name + " :" + messageToSend;
				// envoi a tous les users (sauf la personne qui envoie le message)
				printUsersOfAChannel(chan);
				printOperatorsOfAChannel(chan);
				printInvitedUsersOfAChannel(chan);
				if (findUserInChannel(chan, &user) == true)
				{
					for (std::vector<User *>::iterator it2 = chan->users.begin(); it2 != chan->users.end(); it2++)
					{
						std::cout << "envoye : |" << rpl_privmsg  << "| sur : |" << (*it2)->_fdUser << "|" << std::endl;
						if ((*it2)->_fdUser != user._fdUser)
						{
							//std::cout << "envoye dans if : |" << rpl_privmsg  << "| sur : |" << (*it2)->_fdUser << "|" << std::endl;
							send((*it2)->_fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
							printMessageSendToClient("IRC_PRIVMSG - message sur #chan avec user", (*(*it2)), rpl_privmsg);
						}
					}
					// envoi a tous les operators (sauf la personne qui envoie le message)
					for (std::map<User *, bool>::iterator it3 = chan->operators.begin(); it3 != chan->operators.end(); it3++)
					{
						//std::cout << "22" << std::endl;
						if (checkRightsUserInChannel(chan, &user, OPERATOR) == true && it3->first->_fdUser != user._fdUser)
						{
							send(it3->first->_fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
							printMessageSendToClient("IRC_PRIVMSG - message sur #chan avec operator", (*(it3->first)), rpl_privmsg);
						}
					}
					// envoi a tous les users invites (sauf la personne qui envoie le message)
					throw Irc_privmsg_rpl();
				}
				// je previens l'envoyeur que le msg n'est pas parvenu parce que le pseudo n'existe pas
				std::string err_cannot_send_to_chan = "127.0.0.1 404 " + chan->name + " :Cannot send to channel\r\n";
				send(user._fdUser, err_cannot_send_to_chan.c_str(), err_cannot_send_to_chan.length(), 0);
				printMessageSendToClient("IRC_PRIVMSG - err le user n'est pas dans le chan avec lequel il souhaite communiquer", user, err_cannot_send_to_chan);
			}
			else // il n'a pas trouve le channel
			{
				//std::cout << "3" << std::endl;
				std::string err_no_such_channel = listOfUser + ERROR403;
				send(user._fdUser, err_no_such_channel.c_str(), err_no_such_channel.length(), 0);
				throw Irc_privmsg_error();
			}
		}
		else // c'est un PRIVMSG a un seul user
		{
			std::cout << "4 : |" << listOfUser << "|" << std::endl;
			for (std::vector<User *>::iterator it = Server.users.begin(); it != Server.users.end(); it++)
			{
				std::cout << "41 : |" << (*it)->nickname << "|" << std::endl;
				if (listOfUser == (*it)->nickname)
				{
					//le user existe je lui envoie le msg norme
					rpl_privmsg = ":" + user.nickname + " PRIVMSG " + (*it)->nickname + " :" + messageToSend;
					std::cout << "rpl_privmsg " << rpl_privmsg << std::endl;
					send((*it)->_fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
					printMessageSendToClientPrivMsg("PRIVMSG - section mp vers un seul user", user, rpl_privmsg);
					return;
				}
			}
			// je previens l'envoyeur que le msg n'est pas parvenu parce que le pseudo n'existe pas
			std::string err_no_such_nick = "127.0.0.1 401 " + listOfUser + " :No such nickname\r\n";
			send(user._fdUser, err_no_such_nick.c_str(), err_no_such_nick.length(), 0);
			//send_log(user._fdUser, err_no_such_nick, Server);
			//throw Irc_privmsg_error();
		}
	}
	else // c'est plusieurs users
	{
		std::cout << "5" << std::endl;
		std::string reste;
		std::string destinator;
		while (i != -1)
		{
			reste = listOfUser.substr(i + 1, listOfUser.size());
			destinator = listOfUser.substr(0, i);
			for (std::vector<User *>::iterator it = Server.users.begin(); it != Server.users.end(); it++)
			{
				if (destinator == (*it)->nickname)
				{
					//le destinator existe je lui envoie le msg
					rpl_privmsg = ":" + user.nickname + " PRIVMSG " + (*it)->nickname + " :" + messageToSend;
					send(user._fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
					//send_log((*it)->fd, rpl_privmsg, Server);
					throw Irc_privmsg_rpl();
				}
			}
			// je previens l'envoyeur que le msg n'est pas parvenu parce que le pseudo n'existe pas
			std::string err_no_such_nick = "127.0.0.1 401 " + listOfUser + " :No such nickname\r\n";
			send(user._fdUser, err_no_such_nick.c_str(), err_no_such_nick.length(), 0);
			listOfUser = reste;
			i = listOfUser.find(',');
		} // je refait le message pour le dernier user
		for (std::vector<User *>::iterator it = Server.users.begin(); it != Server.users.end(); it++)
		{
			if (listOfUser == (*it)->nickname)
			{
				//le user existe je lui envoie le msg
				rpl_privmsg = ":" + user.nickname + " PRIVMSG " + (*it)->nickname + " :" + messageToSend;
				send(user._fdUser, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
				//send_log((*it)->fd, rpl_privmsg, Server);
				throw Irc_privmsg_rpl();
			}
		}
	}
}
