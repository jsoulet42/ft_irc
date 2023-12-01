
# include <ft_irc.hpp>
# include <Server.hpp>
# include <User.hpp>
# include <Channel.hpp>

void ircJoin(std::string &msg, User &user, Server &Server);
void msgError(std::string const &code, User &user, std::string const &msg);


User *findUserById(std::vector<User *> &users, int const &id)
{
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->_fdUser == id)
			return (*it);
	}
	return NULL;
}

Channel *findChannelByName(std::vector<Channel *> &channels, std::string const &cmd)
{
	std::string name = cmd.find(' ') != std::string::npos ? cmd.substr(5, cmd.find(' ') - 5) : cmd.substr(5);
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->name == name)
			return (*it);
	}
	return NULL;
}

void interpretCommand(Server &server, std::string strmess, int const &id)
{
	User *user = findUserById(server.users, id);
	if(strmess.compare(0, 5, "JOIN ") == 0)
	{
		ircJoin(strmess, *user, server);
	}
}

void ircJoin(std::string &msg, User &user, Server &Server)
{
	int pos;
	if (msg.length() == 5)
	{
		msgError("461", user,ERRORJ461);
		return (throw std::exception());
	}
	pos = msg.find('#');
	if (pos != std::string::npos) //Si la commande ne commence pas par un #
	{
		msgError("403", user, ERRORJ403);
		return ;
	}//la commande est correctement formaté à partir d'ici
	msg += pos;
	Channel *channel = findChannelByName(Server.channels, msg);
	if (channel) // si le channel existe deja
	{
		// if (findUserInChannel(channel, &user)) // vérifie si l'utilisateur est déja dans le channel
		// {
		// 	throw Channel::UserIsAlredyInChannelException();
		// }
		if (!checkRightsUserInChannel(channel, &user)) // vérifie si l'utilisateur a les droits pour rejoindre le channel
		{
			msgError("403", user, ERRORJ403);
			return ;
		}

		// vérifie si le mot de passe du channel est correct
		// envoyer les messages de confirmation
	}
	else
	{
		// créer le channel
		// ajouter l'utilisateur au channel
		// envoyer les messages de confirmation
	}
}

bool findUserInChannel(Channel *channel, User *user)
{
	for (std::vector<User *>::iterator it = channel->users.begin(); it != channel->users.end(); ++it)
	{
		if ((*it)->_fdUser == user->_fdUser)
			return true;
	}
	return false;
}
bool checkRightsUserInChannel(Channel *channel, User *user)
{
	for(std::vector<User *>::iterator it = channel->invitedUsers.begin(); it != channel->invitedUsers.end(); ++it)
	{
		if ((*it)->_fdUser == user->_fdUser)
			return true;
	}
}

void msgError(std::string const &code, User &user, std::string const &msg)
{
	std::stringstream ss;
	ss << IPHOST << code << " " << user.nickname << msg;
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
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
