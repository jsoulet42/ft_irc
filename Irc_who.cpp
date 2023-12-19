# include "./includes/ft_irc.hpp"
/*
void	irc_who(std::string &message, User &user, Server &server)
{
	//std::cout << message << user.CAP_OK << server.current_size << std::endl;
	//	Who query
	//	Command: WHO
	//	Parameters: [ <mask> [ "o" ] ]

	//	The WHO command is used by a client to generate a query which returns
	//	a list of information which 'matches' the <mask> parameter given by
	//	the client.  In the absence of the <mask> parameter, all visible
	//	(users who aren't invisible (user mode +i) and who don't have a
	//	common channel with the requesting client) are listed.  The same
	//	result can be achieved by using a <mask> of "0" or any wildcard which
	//	will end up matching every visible user.

	//	The <mask> passed to WHO is matched against users' host, server, real
	//	name and nickname if the channel <mask> cannot be found.

	//	If the "o" parameter is passed only operators are returned according
	//	to the <mask> supplied.

	//	Numeric Replies:

	//	        ERR_NOSUCHSERVER
	//	        RPL_WHOREPLY                  RPL_ENDOFWHO

	//	Examples:

	//	WHO *.fi                        ; Command to list all users who match
	//	                                against "*.fi".

	//	WHO jto* o                      ; Command to list all users with a
	//	                                match against "jto*" if they are an
	//	                                operator.

	// Parsing the message
	ssize_t pos = message.find(' ');
	std::string rpl_who;

	if (pos == -1) //je ne trouve pas d'espace, WHO est vide ou mal formate
	{
		//WHO mal formate
		std::string err_unknow_command = message.substr(0, message.size() - 1) + " :Unknow command\r\n";
		send(user._fdUser, err_unknow_command.c_str(), err_unknow_command.length(), 0);
		printMessageSendToClient("IRC_WHO - Unfomarted command or empty", user, err_unknow_command);
	}
	else
	{
		message = message.substr(4); // j'enleve de WHO et l'espace.
		pos = message.find('*'); //je cherche si wildcard
		if (pos >= 0)
		{
			std::string err_no_such_service = "408 WHO with wildcard :no such service\r\n";
			send(user._fdUser, err_no_such_service.c_str(), err_no_such_service.length(), 0);
			printMessageSendToClient("IRC_WHO - No wild card service", user, err_no_such_service);
			return;
		}
		pos = message.find(" o\n");
		int findOnce = 0;
		if (pos >= 0)
		{
			//recherche d'un operateur nomme xxx ou xxx = message.substr(0, pos);
			//doit verif que la personne est sur le chan ET quelle est OP.
			std::cout << "le WHO d'operator n'est pas encore implante" << std::endl;
		}
		else
		{
			//recherche standard d'un simple user
			std::vector<Channel *>::iterator it2 = server.channels.begin();

			message = message.substr(0, message.size() - 1);
			while(it2 != server.channels.end())
			{
				std::vector<User *>::iterator it = (*it2)->users.begin();
				while (it != (*it2)->users.end())
				{
					if ((*it)->nickname == message)
					{
						findOnce = 1;
						rpl_who = "352 " + (*it2)->name + " " + (*it)->username + " ircserv 127.0.0.1 " + (*it)->nickname + "\r\n";
						send(user._fdUser, rpl_who.c_str(), rpl_who.length(), 0);
						printMessageSendToClient("IRC_WHO - find", user, rpl_who);
					}
					it++;
				}
				it2++;
			}
			if (findOnce == 1)
			{
				rpl_who = "315 " + user.nickname + " " + message + " :End of WHO list\r\n";
				send(user._fdUser, rpl_who.c_str(), rpl_who.length(), 0);
				printMessageSendToClient("IRC_WHO - end of who", user, rpl_who);
			}
			else
			{
				std::string err_was_no_such_nick = "406 " + message + " :There was no such nickname\r\n";
				send(user._fdUser, err_was_no_such_nick.c_str(), err_was_no_such_nick.length(), 0);
				printMessageSendToClient("IRC_WHO - end of who - attention renvoi 406 si aucun channel ouverts", user, err_was_no_such_nick);
			}
			return;
		}
	}
}*/

void	irc_who(std::string &message, User &user, Server &server)
{
	(void)user;
	(void)server;
	std::cout << message << std::endl;
}
