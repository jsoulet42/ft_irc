////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   INVITE                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

# include "./includes/ft_irc.hpp"
# include "./includes/Server.hpp"
# include "./includes/User.hpp"
# include "./includes/Channel.hpp"

// codé par Matthieu Regis Diamant-Berger anciennement Descamps née a Paris 11 le 29/12/1995 vers 6h du matin, le 04/12/2023 du calendrier grégorien et année chinoise Gui-Mao (40), année 2023, mois 10, jour 22
// au lieu de me faire rire avec des blagues tu devrai tapercevoir que la fonction msgError est a moi et que tu devrai faire la tienne xD !
// blablabla




void ircInvite(std::string &msg, User &user, Server &server)
{
	std::vector<std::string> msgVec;

	msgVec = splitString(msg, ' ');												// on split le message format : INVITE <nickname> <channel>
	if (msgVec.size() <= 2)
		msgError461(user);
	Channel *channel = findChanelbyNameMatt(msgVec[2], server.channels);
	if (channel == NULL)
	{
		std::cout << "yolo" << std::endl;														// on verifie que le channel existe
		msgError403(user, msgVec[2]);
	}
	User *userInvited = findUserByName(server.users, msgVec[1]);
	if (userInvited == NULL)													// on verifie que l'utilisateur existe
		msgError401(user, msgVec[1]);
	if(!channel->modeI)															// si le channel est public
	{
		if (findElement(user, channel->users) == false)
			msgError442(user, msgVec[2]);
	}
	// else																		// si le channel est prive
	// {
 	if (checkRightsUserInChannel(channel, &user) != 1)						//on verifie que l'utilisateur est bien operateur du channel
			msgError482(user, msgVec[2]);
	// }
	if (findElement(*userInvited, channel->invitedUsers) == true)				// on verifie que l'utilisateur n'est pas deja invité
		msgError443(user, msgVec[1], *channel);
	channel->invitedUsers.push_back(userInvited);								// on ajoute l'utilisateur a la liste des invites
	std::stringstream ss;
	ss << IPHOST << userInvited->nickname << " " << channel->name << "\r\n";
	send(user._fdUser, ss.str().c_str(), ss.str().size(), 0);
}
