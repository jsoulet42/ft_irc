# include "./includes/ft_irc.hpp"

void irc_part(std::string message, User *user, Server *server)
{

	std::cout << YELLOW <<  ON_BLACK << "PART : " << message << user->nickname << server->fdNb << RESET << std::endl;
	//// "PART #nom_du_channel :message"
	//int rc;
	//std::cout << YELLOW <<  ON_BLACK << "[COMMAND]PART" << RESET << std::endl;

	//message = message.substr(6); // Remove the command "PART"
	//std::string rpl_part = message;
	//std::string channelName = rpl_part.substr(0, rpl_part.find(' '));
	//channelName = channelName.substr(0, channelName.length());
	//rpl_part.erase(0, rpl_part.find(' ') + 2);
	//std::string reason = rpl_part;
	//rpl_part = ":" + user->nickname + " PART #" + channelName + " :" + reason + "\r\n";
	////std::cout << "irc_part, message : " << message << std::endl;
	////std::cout << "irc_part, channelName : " << channelName << std::endl;
	////std::cout << "irc_part, rpl_part : " << rpl_part << std::endl;
	//std::vector<Channel *>::iterator it = server->channels.begin();
	//while (it != server->channels.end())
	//{
	//	if ((*it)->name == channelName)
	//	{
	//		//std::cout << "irc_part, message : " << message << std::endl;
	//		//std::cout << "irc_part, channelName : " << channelName << std::endl;
	//		//std::cout << "irc_part, rpl_part : " << rpl_part << std::endl;
	//		(*it)->channelSendLoop(rpl_part, user->fd, server, 1);
	//		(*it)->nbUsers--;
	//		irc_names((*it), user, server);
	//		rc = (*it)->deleteChannelUser(user, server);
	//		if (rc == 1)
	//		{
	//			delete (*it);
	//			std::cout << BRIGHT_YELLOW << ON_BLACK << "[DELETE] : Succesfully deleted channel #" << channelName << RESET << std::endl;
	//			return;
	//		}
	//	}
	//	++it;
	//}
}