# include "./includes/ft_irc.hpp"
# include "./includes/Server.hpp"
# include "./includes/User.hpp"
# include "./includes/Channel.hpp"


// KICK <channel> <nick> [<reason>]

void ircKick(std::string &msg, User &user, Server &server)
{
	(void)server;
	std::vector<std::string> msgSplit = splitString(msg, ' ');

	for(std::vector<std::string>::iterator it = msgSplit.begin(); it != msgSplit.end(); ++it)	// DEBUG
		std::cout << *it << std::endl;
	if (msgSplit.size() < 3)
	{
		msgError461(user);
		return ;
	}
	if (countSpaces(msg, ' ') < 2)												//ajout du message de kick s'il n'y en a pas
		msgSplit.push_back(":" + user.nickname);
	if (msgSplit[2][0] != '#')													// on verifie que le channel commence bien par #
		msgError403(user, msgSplit[2]);
	//Channel *channel = findChanelbyNameMatt(ici le nom du channel apres parsing, server.channels);
	//if (channel == NULL)
		//erreur channel inexistant
	//if (checkRightsUserInChannel(channel, &user) != OPERATOR)							//on verifie que l'utilisateur est bien operateur du channel
		//erreur pas les droits
}

// La commande KICK permet à un opérateur de canal de retirer un utilisateur de son canal.

// KICK <canal> <pseudo> [<raison>]
// <canal> : Canal à partir duquel l'utilisateur doit être expulsé.
// <pseudo> : Pseudo de l'utilisateur à expulser.
// <raison> : Une raison facultative à donner à l'utilisateur (et à tout le monde
// dans le canal).

// Certains logiciels serveur permettent de spécifier plusieurs canaux ou
// pseudonymes en les séparant par des virgules (par exemple, KICK #v3 tom,jerry).
// Cependant, ce n'est pas universellement supporté, donc limitez-vous à un canal
// et un pseudo par commande d'expulsion.

// Un message KICK envoyé par le serveur indique qu'une expulsion a eu lieu. Dans
// ce cas, le <préfixe> est l'utilisateur ayant effectué l'expulsion. Si la
// commande KICK d'un utilisateur est réussie, ils reçoivent l'un de ces messages.
// De plus, l'utilisateur expulsé et tous les autres clients du canal reçoivent
// également un message KICK. Par exemple, si dan, alice et matthew sont sur le
// canal #v4, et que dan expulse matthew, les trois clients recevront un message
// KICK indiquant que dan a retiré matthew du canal.

// Il est possible de spécifier plusieurs canaux ou pseudonymes, et les clients
// exécutent les demandes une par une (en ne retournant qu'un canal/pseudo dans
// chaque message KICK qu'ils envoient). Cela est affiché dans les exemples
// ci-dessous.

// La commande KICK est réservée aux opérateurs de canal, parfois avec des niveaux
// de permission alternatifs ajoutés au serveur (tels que le halfop).

// Si l'utilisateur qui effectue l'expulsion n'a pas les autorisations
// appropriées, le serveur renvoie ERR_CHANOPRIVSNEEDED et continue. Si le nom du
// canal est entièrement invalide (ne commence pas par #, etc.), le serveur peut
// renvoyer ERR_BADCHANMASK (ou plus probablement ERR_NOSUCHCHANNEL). Si
// l'utilisateur à expulser n'est pas sur le réseau, l'opérateur du canal envoie
// ERR_NOSUCHNICK. Si l'utilisateur à expulser existe mais n'est pas sur le canal,
// l'opérateur du canal envoie ERR_USERNOTINCHANNEL. Et si le canal n'existe pas,
// ERR_NOSUCHCHANNEL est retourné.
