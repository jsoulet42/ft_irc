# include <ft_irc.hpp>
# include <Server.hpp>
# include <User.hpp>
# include <Channel.hpp>

User *findUserById(std::vector<User> &users, int const &id)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); it++)
	{
		if ((it)->_fdUser == id)
			return (it);
	}
	return NULL;
}

Channel *isChannelExist(std::vector<Channel *> &channels, std::string const &name)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end; it++)
	{
		if ((*it)->name == name)
			return (*it)
	}
	return NULL;
}

void interpretCommand(Server &server, std::string strmess, int const &id)
{
	User *user = findUserById(server._users, id);
	if(strmess.compare(0, 5, "JOIN ") == 0)
	{
		std::string msg = strmess.substr(5); // prend le message apres le premier espace
		ircJoin(msg, user, server);
	}

}
/*
** Cette fonction est appelée lorsqu'un client envoie JOIN.
** Elle va interpréter le message et appeler la fonction correspondante.
**Exemple de message : JOIN #chan
**Autre exemple : JOIN #chan password
*/
void ircJoin(std::string &msg, User &user, Server &Server)
{
	std::stringstream msgError;
	if(msg == "")
	{
		msgError << IPHOST << " " << user->nickname << ERR_NEEDMOREPARAMS;
		send (msgError.str().c_str(), msgError.str().size(), 0);
	}
	else if (msg[0] != '#')
	{
		msgError << IPHOST << " " << user->nickname << ERR_NEEDMOREPARAMS;
		send (msgError.str().c_str(), msgError.str().size(), 0);
	}
	else
	{
		std::string channelName = msg.substr(0, msg.find_first_of(' '));
		Channel *channel = isChannelExist(Server._channels, channelName);
		if (channel)
		{
			channel->addUser(user, msg);
		}
		else
		{
			Channel *newChannel = new Channel(user, msg);
			Server._channels.push_back(newChannel);
		}
	}
}


// Cette fonction `irc_join` semble être liée à la commande JOIN d'un serveur IRC. Voici une explication détaillée de son fonctionnement :

// 1. **Extraction des paramètres** :
//    - La fonction commence par récupérer les paramètres envoyés à la commande JOIN en avançant de 5 caractères (supposant que la commande est de la forme "JOIN ").
//    - Si aucun paramètre n'est fourni, elle renvoie un message d'erreur indiquant qu'il n'y a pas suffisamment de paramètres.

// 2. **Analyse et traitement des paramètres** :
//    - Elle analyse les paramètres et les traite en conséquence.
//    - Si le premier caractère après la commande JOIN n'est pas '#' (le symbole indiquant un canal), la fonction appelle `irc_join_cases` pour traiter d'autres cas.
//    - Si le premier caractère est '#', la fonction recherche parmi les canaux existants dans le serveur.
//    - Si un canal correspondant est trouvé :
//      - Vérifie les modes du canal et l'accès du client pour rejoindre le canal.
//      - Si les conditions pour rejoindre le canal ne sont pas remplies, elle envoie un message d'erreur approprié au client et arrête le processus.
//      - Si les conditions sont remplies, elle ajoute l'utilisateur au canal, envoie des messages de confirmation aux utilisateurs du canal et effectue des actions de suivi.
//    - Si aucun canal correspondant n'est trouvé :
//      - Crée un nouveau canal avec le nom du paramètre.
//      - Ajoute l'utilisateur comme premier membre du canal.
//      - Envoie des messages de confirmation.

// 3. **Traitements additionnels** :
//    - Elle envoie des messages relatifs à l'entrée et la sortie d'utilisateurs sur les canaux (`JOIN`, `PART`, `KICK`).
//    - Actualise les données liées aux canaux et utilisateurs dans le serveur.

// En résumé, cette fonction prend en charge le traitement des commandes JOIN des utilisateurs IRC, vérifiant les droits d'accès aux canaux, ajoutant les utilisateurs aux canaux existants ou créant de nouveaux canaux si nécessaire, et informant tous les utilisateurs concernés des changements dans les canaux.
