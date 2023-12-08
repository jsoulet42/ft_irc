////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   INVITE                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"

// codé par Matthieu Regis Diamant-Berger anciennement Descamps née a Paris 11 le 29/12/1995 vers 6h du matin, le 04/12/2023 du calendrier grégorien et année chinoise Gui-Mao (40), année 2023, mois 10, jour 22
// au lieu de me faire rire avec des blagues tu devrai tapercevoir que la fonction msgError est a moi et que tu devrai faire la tienne xD !
// blablabla
void msgError461(User const &user);
void msgError403(User const &user, std::string const &channel);
void msgError442(User const &user, std::string const &channel);
void msgError443(User const &user, std::string const &userInvited, Channel const &channel);
void msgError482(User const &user, std::string const &channel);



void ircInvite(std::string &msg, User &user, Server &server)
{
	std::vector<std::string> msgVec;

	msgVec = splitString(msg, ' '); // on split le message format : INVITE <nickname> <channel>
	if (msgVec.size() <= 2)
		msgError("461", user, ERRORI461);
	if (errorCmd == true)
		throw std::runtime_error("ERROR : No enough parameters");
	// dans un premier temps on verifie que le channel existe
	Channel *channel = findChanelbyNameMatt(msgVec[2], server.channels);
	User *userInvited = findUserByName(server.users, msgVec[1]);
	// error 401 a faire si user no existe
	if (channel == NULL)
		msgError403(user, msgVec[2]);
	// On verifie que celui qui invite est bien dans le channel "et qu'il a les droits"
	if(!channel->modeI) // si le channel est en mode invitation seulement
	{
		if (findUser(user, channel->users) == false)
			msgError442(user, msgVec[2]);
	}
	else
	{
		if (findUser(user, channel->operators) == false)
			msgError482(user, msgVec[2]);
	}
	// On verifie que l'utilisateur n'est pas deja invité dans le channel
	if (findUser(*userInvited, channel->invitedUsers) == true)
		msgError443(user, msgVec[1], *channel);
	// si tout est ok on ajoute l'utilisateur dans la liste des invités du channel
	channel->invitedUsers.push_back(userInvited);
	// on envoie le message d'invitation a l'utilisateur
	std::stringstream ss;
	ss << IPHOST << " INVITE " << userInvited->nickname << " " << channel->name << "\r\n";
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
}
/// @brief Erreur qui se produit quand il n'y a pas assez de paramètres
void msgError461(User const &user)
{
	std::stringstream ss;
	ss << IPHOST << " 461 " << user.nickname << " " << ERRORI461;
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	errorCmd = true;
	throw std::runtime_error(ERRORI461);
}
/// @brief Erreur qui se produit quand le channel n'existe pas
void msgError403(User const &user, std::string const &channel)
{
	std::stringstream ss;
	ss << IPHOST << " 403 " << user.nickname << " " << channel << " " << ERRORI403;
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	errorCmd = true;
	throw std::runtime_error(ERRORI403);
}
/// @brief Erreur qui se produit quand celui qui invite n'est pas dans le channel
void msgError442(User const &user, std::string const &channel)
{
	std::stringstream ss;
	ss << IPHOST << " 442 " << user.nickname << " " << channel << " " << ERRORI442;
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	errorCmd = true;
	throw std::runtime_error(ERRORI442);
}
/// @brief Returned when a client tries to invite <nick> to a channel they’re already joined to.
void msgError443(User const &user, std::string const &userInvited, Channel const &channel)
{
	std::stringstream ss;
	ss << IPHOST << " 443 " << userInvited << " " << channel.name << " " << ERRORI443;
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	errorCmd = true;
	throw std::runtime_error(ERRORI443);
}
/// @brief Le client ne possède pas les privilèges appropriés pour le canal
void msgError482(User const &user, std::string const &channel)
{
	std::stringstream ss;
	ss << IPHOST << " 482 " << user.nickname << " " << channel << " " << ERRORI482;
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
	errorCmd = true;
	throw std::runtime_error(ERRORI482);
}

// TRADUCTION IRC : Message INVITE
//     Commande : INVITE
//  Paramètres : <pseudo> <canal>
// La commande INVITE est utilisée pour inviter un utilisateur dans un canal.
// Le paramètre <pseudo> est le pseudonyme de la personne à inviter dans le canal
// cible <canal>.
// Le canal cible DEVRAIT exister (au moins un utilisateur y est présent).
// Sinon, le serveur DEVRAIT rejeter la commande avec le numéro ERR_NOSUCHCHANNEL.

// Seuls les membres du canal sont autorisés à inviter d'autres utilisateurs.
// Sinon, le serveur DOIT rejeter la commande avec le numéro ERR_NOTONCHANNEL.

// Les serveurs PEUVENT rejeter la commande avec le numéro ERR_CHANOPRIVSNEEDED.
// En particulier, ils DEVRAIENT la rejeter lorsque le mode "invitation seulement"
// est activé sur le canal et que l'utilisateur n'est pas un opérateur de canal.

// Si l'utilisateur est déjà sur le canal cible, le serveur DOIT rejeter la
// commande avec le numéro ERR_USERONCHANNEL.

// Lorsque l'invitation est réussie, le serveur DOIT envoyer un numéro RPL_INVITING
// à l'émetteur de la commande et un message INVITE, avec l'émetteur en tant que
// <source>, à l'utilisateur cible. Les autres membres du canal NE DOIVENT PAS être
// notifiés
