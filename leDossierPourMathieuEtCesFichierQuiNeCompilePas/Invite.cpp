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
//au lieu de me faire rire avec des blagues tu devrai tapercevoir que la fonction msgError est a moi et que tu devrai faire la tienne xD !
void ircInvite(std::string &msg, User &user, Server &server)
{

	(void)server;
	(void)msg;
	(void)user;
	// INVITE bob psg
	std::vector<std::string> msgVec;

	msgVec = splitString(msg, ' ');
	if (msgVec.size() <= 2)
		msgError("461", user, ERRORI461);
	if (errorCmd == true)
		throw inviteException();
	//dans un premier temps on verifie que le channel existe
	Channel *channel = findChanelbyNameMatt(msgVec[2], server.channels);
	if (channel == NULL)
		msgError("403", user, ERRORI403);
	//Channel *channel = findChannelByName(Server.channels, msgVec[1]);
	//On verifie que celui qui invite est bien dans le channel "et qu'il a les droits"
	//On verifie si le channel est en mode invitation seulement
	//On verifie que l'utilisateur n'est pas deja dans le channel
	//On verifie que l'utilisateur n'est pas deja invité dans le channel
	//si tout est ok on ajoute l'utilisateur dans la liste des invités du channel


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
