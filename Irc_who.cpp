# include "./includes/ft_irc.hpp"

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
	std::string copy = message.substr(4); // j'enleve de WHO et l'espace.
	ssize_t pos = copy.find(' ');
	std::string rpl_who;
	if (pos > 0 && copy[pos + 1] == 'o')
	{
		// recherche operator
		// print si operator ou non
		// return;
		pos = copy.find('*'); //je cherche si wildcard
		if (pos >= 0)
		{
			std::cout << "I'm sorry but WHO can accept wildcard *" << std::endl;
			return;
		}
		//recherche standard (sans wildcard)
		std::vector<User *>::iterator it = server.users.begin();
		while (it != server.users.end())
		{
			if ((*it)->nickname == user.nickname)
			{
				rpl_who = user.nickname + " is connected\r\n";
				
				return;
			}
			++it;
		}
		rpl_who = user.nickname + " not found\r\n";
		
		return;
	}
	if (pos == -1)
	{
		//recherche un seul user
		pos = copy.find('*'); //je cherche si wildcard
		if (pos >= 0)
		{
			std::cout << "I'm sorry but WHO can accept wildcard *" << std::endl;
			return;
		}
		//recherche standard (sans wildcard)
		std::vector<User *>::iterator it = server.users.begin();
		while (it != server.users.end())
		{
			if ((*it)->nickname == user.nickname)
			{
				rpl_who = user.nickname + " is connected\r\n";
				
				return;
			}
			++it;
		}
		rpl_who = user.nickname + " not found\r\n";
		
		return;
	}
	rpl_who = " you can't search more than one nick\r\n";
	

}