# include "./includes/ft_irc.hpp"

void	irc_whois(std::string message, User *user, Server *server)
{
	//	Commande: WHOIS
	//
	//	Paramètres: [<serveur>] <masque de pseudo>[,<masque de pseudo>[,...]]
	//
	//	Ce message est utilisé pour obtenir des informations sur un utilisateur
	//	donné. Le serveur répondra à ce message avec des messages numériques
	//	indiquant les différents statuts de chacun des utilisateurs qui
	//	correspondent au <masque de pseudo> (si vous pouvez les voir).
	//	
	//	un /WHOIS sans masque renvoi tout les pseudos sur le channel.

	// Parsing the message
	std::string copy = message.substr(6); // j'enleve de WHOIS et l'espace.
	
	//A FAIRE car utile pour la prog et la recherche de bugs
	std::cout << message << user->username << server->fdNb << std::endl;


}