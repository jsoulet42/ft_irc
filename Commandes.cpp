
# include <ft_irc.hpp>
# include <Server.hpp>
# include <User.hpp>
# include <Channel.hpp>

extern bool errorCmd;
void normKey(std::string &key, User &user, Server &server);
void ircJoin(std::string &msg, User &user, Server &Server);
void parseCmd(std::string &cmd, User &user, Server &Server);
void parseCmdWithNoKey(std::string &cmd, User &user, Server &server);
void normNameChannel(std::string &channel, User &user, Server &server);
void msgError(std::string const &code, User &user, std::string const &msg);
void protocolForJoinChannel(Channel *channel, User &user, std::string &key);
void messageToAllUsersInChannel(Channel *channel, User &user, int createOrJoin);
void joinOrCreatChannel(std::string &cmd, User &user, Server &Server, std::string &key);
void sendForCreate(std::vector<std::string> &channels, User &user, Server &server, std::vector<std::string> &keys);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  TOOLS                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

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
	errorCmd = true;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              interpretCmd                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void interpretCommand(Server &server, std::string strmess, int const &id)
{
	User *user = findUserById(server.users, id);
	errorCmd = false;
	if(strmess.compare(0, 5, "JOIN ") == 0)
	{
		ircJoin(strmess, *user, server);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  JOIN                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//fait par julien le 02/12/2023
void ircJoin(std::string &msg, User &user, Server &Server)
{
	std::string cmd = strtok((char *)msg.c_str() + 5, "\r\n");
	if (cmd.size() == 0)
		msgError("461", user, ERRORJ461);
	if (errorCmd == true)
		throw joinException();
	parseCmd(cmd, user, Server);
}


//fait par julien le 02/12/2023
void parseCmdWithNoKey(std::string &cmd, User &user, Server &server)
{
	std::string channel;
	std::string key;

	if (cmd.find(',') == std::string::npos)
	{
		channel = cmd.substr(0, cmd.find('\r'));
		normNameChannel(channel, user, server);
		joinOrCreatChannel(channel, user, server, key);
	}
	if (cmd.find(' ') == std::string::npos)
	{
		while (cmd.find(',') != std::string::npos)
		{
			channel = cmd.substr(0, cmd.find(','));
			normNameChannel(channel, user, server);
			joinOrCreatChannel(channel, user, server, key);
			cmd.erase(0, cmd.find(',') + 1);
		}
		channel = cmd.substr(0, cmd.find('\r'));
		normNameChannel(channel, user, server);
		joinOrCreatChannel(channel, user, server, key);
	}
}

//fait par julien le 03/12/2023
void normKey(std::string &key, User &user, Server &server)
{
	if (key.size() > 10 || key.size() < 1)
		msgError("475", user, ERRORJ475);
	for (size_t i = 0; i < key.size(); ++i)
	{
		char c = key[i];
		if (c < 32 || c == 127 || c == 9 || c == 10 || c == 13)
			msgError("475", user, ERRORJ475);
	}
	if (errorCmd == true)
		throw keyException();
}

//fait par julien le 03/12/2023
void normNameChannel(std::string &channel, User &user, Server &server)
{
	if (channel.compare(0, 1, "#") != 0 || channel.compare(0, 1, "&") != 0)
			msgError("403", user, ERRORJ403);
	else if (channel.size() < 2)
		msgError("461", user, ERRORJ461);
	else if (channel.size() > 10)
		msgError("476", user, ERRORJ476);
	if (errorCmd == true)
			throw joinException();
	channel.erase(0, 1);
}


//fait par julien le 02/12/2023
void parseCmd(std::string &cmd, User &user, Server &server)
{
	std::vector<std::string> channels;
	std::vector<std::string> keys;
	std::string channel;
	std::string key;
	std::string buffer;

	parseCmdWithNoKey(cmd, user, server);
	channel = cmd.substr(0, cmd.find(' '));
	cmd.erase(0, cmd.find(' ') + 1);
	key = cmd.substr(0, cmd.find('\r'));
	while (channel.find(',') != std::string::npos)
	{
		buffer = channel.substr(0, channel.find(','));
		normNameChannel(buffer, user, server);
		channels.push_back(buffer);
		channel.erase(0, channel.find(',') + 1);
	}
	buffer = channel.substr(0, channel.find(' '));
	normNameChannel(buffer, user, server);
	channels.push_back(buffer);
	if (key.find(',') == std::string::npos)
	{
		buffer = key.substr(0, key.find('\r'));
		normKey(buffer, user, server);
		keys.push_back(buffer);
	}
	else
	{
		while (key.find(',') != std::string::npos)
		{
			buffer = key.substr(0, key.find(','));
			normKey(buffer, user, server);
			keys.push_back(buffer);
			key.erase(0, key.find(',') + 1);
		}
		buffer = key.substr(0, key.find('\r'));
		normKey(buffer, user, server);
		keys.push_back(buffer);
	}
	sendForCreate(channels, user, server, keys);
	// #channel1,chanel2,chanel3 key1,key2,key3\r\n
	// ici on a un tableau de channels et un tableau de keys remplis
	// faire une boucle qui parcours les deux tableaux et qui fait joinOrCreatChannel
	// pour chaque channel avec la key correspondante si elle existe sinon avec une key vide
}

void sendForCreate(std::vector<std::string> &channels, User &user, Server &server, std::vector<std::string> &keys)
{
	size_t i = 0;
	std::string keyEmpty = "";

	for (i; i < keys.size(); ++i)
	{
		joinOrCreatChannel(channels[i], user, server, keys[i]);
	}
	for (i; i < channels.size(); ++i)
	{
		joinOrCreatChannel(channels[i], user, server, keyEmpty);
	}
}

void protocolForJoinChannel(Channel &channel, User &user, std::string &key)
{
	if (!checkRightsUserInChannel(&channel, &user))
			msgError("473", user, ERRORJ473);
	channel.ft_checkMode(channel, user);
	if (findUserInChannel(&channel, &user))
		throw Channel::UserIsAlredyInChannelException();
	else if (channel.addUser(&user, key) == -1)
		msgError("475", user, ERRORJ475);
	if (errorCmd == true)
		throw joinException();
}


//fait par julien le 02/12/2023
// prevoir a implementer les erreurs manquantes
void joinOrCreatChannel(std::string &cmd, User &user, Server &server, std::string &key)
{
	Channel *channel = findChannelByName(server.channels, cmd);

	if (channel)
	{
		protocolForJoinChannel(channel, user, key);
		channel->addUser(&user, key);
		messageToAllUsersInChannel(channel, user, 0);
	}
	else
	{
		channel = new Channel(&user, cmd);
		channel->password = key;
		channel->addUser(&user, key);
		server.channels.push_back(channel);
		messageToAllUsersInChannel(channel, user, 1);
	}
}

//fait par julien le 02/12/2023
void messageToAllUsersInChannel(Channel *channel, User &user, int createOrJoin)
{
	std::stringstream ss;

	if (createOrJoin == 0)
	{
		ss << IPHOST << "JOIN " << channel->name << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "332 " << user.nickname << " " << channel->name << " :" << channel->topic << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "353 " << user.nickname << " = " << channel->name << " :";
		for (std::vector<User *>::iterator it = channel->users.begin(); it != channel->users.end(); ++it)
			ss << (*it)->nickname << " ";
		ss << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "366 " << user.nickname << " " << channel->name << " :End of /NAMES list.\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	}
	else if (createOrJoin)
	{
		std::stringstream ss;
		ss << IPHOST << "JOIN " << channel->name << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
		ss << IPHOST << "332 " << user.nickname << " " << channel->name << " :" << channel->topic << "\r\n";
		send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
		ss.str("");
	}
}

const char* joinException::what() const throw()
{
	return "[Error] during JOIN command";
}

const char* keyException::what() const throw()
{
	return "[Error] during JOIN command , key is too long";
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  PART                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  KICK                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  QUIT                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  PRIVMSG                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  TOPIC                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  NICK                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  MODE                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////




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

/*void ircJoin(std::string &msg, User &user, Server &Server)
{
	if (msg.length() == 5)
	{
		msgError("461", user,ERRORJ461);
		return (throw std::exception());
	}
	if (msg.find('#') != std::string::npos) //Si la commande ne commence pas par un #
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
}*/
