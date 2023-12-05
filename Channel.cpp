#include "Channel.hpp"
void msgError(std::string const &code, User &user, std::string const &msg);

//-------------------------------Copelien form--------------------------------//
Channel::Channel(User *user, std::string &name)
{
	this->maxUsers = 10;
	this->nbUsers = 1;
	this->name = name;
	this->users.push_back(user);
	this->operators.push_back(user);
	this->invitedUsers.push_back(user);
}
Channel::Channel(Channel const &src)
{
	*this = src;
}
Channel::~Channel()
{
}
Channel &	Channel::operator=(Channel const &rSym)
{
	if (this != &rSym)
	{
		this->maxUsers = rSym.maxUsers;
		this->nbUsers = rSym.nbUsers;
		this->name = rSym.name;
		this->users = rSym.users;
		this->operators = rSym.operators;
		this->invitedUsers = rSym.invitedUsers;
	}
	return *this;
}
//------------------------------Getter & Setter-------------------------------//
//-------------------------------Other function-------------------------------//

//modifié par julien le 02/12/2023
int Channel::addUser(User *user, std::string &password)
{
	if (this->password.compare(password) == 0)
	{
		this->users.push_back(user);
		this->nbUsers++;
		return 0;
	}
	else
		return -1;
}

/* return 1 si une erreur a eter trouver dans un checkMode()*/
void Channel::ft_checkMode(Channel *channel, User &user)
{
	int i = 0;
	channel->ft_fillPtrCheckMode();
	std::map<std::string, bool>::iterator it = channel->modeTab.begin();
	while (it != channel->modeTab.end())
	{
		if (it->second == true)
			(channel->*ftPtr[i])(user);
		++it;
		++i;
	}
}

void Channel::checkModeI(User &user)
{
	if (this->users.size() >= this->modeLMaxUser)
		msgError("471", user, ERRORJ471);
}

void Channel::checkModeK(User &user)
{
	(void)user;
}

void Channel::checkModeL(User &user)
{
	(void)user;

}

void Channel::checkModeO(User &user)
{
	(void)user;

}

void Channel::checkModeT(User &user)
{
	(void)user;

}

void Channel::ft_fillPtrCheckMode()
{
	this->ftPtr[0] = &Channel::checkModeI;
	this->ftPtr[1] = &Channel::checkModeK;
	this->ftPtr[2] = &Channel::checkModeL;
	this->ftPtr[3] = &Channel::checkModeO;
	this->ftPtr[4] = &Channel::checkModeT;
}

//------------------------------Ostream overload------------------------------//
std::ostream &	operator<<(std::ostream & o, Channel const &rSym)
{
	(void)rSym;
	o << "nothing";
	return o;
}

Channel *findChanelbyNameMatt(std::string name, std::vector<Channel *> &chanelList)
{
	if (name.compare(0, 1, "#") == 0 || name.compare(0, 1, "&") == 0)
		name.erase(0, 1);
	//else
		//on peux décider de renvoyer une erreur ou de ne rien faire
	for (std::vector<Channel *>::iterator it = chanelList.begin(); it != chanelList.end(); it++)
	{
		if ((*it)->name == name)
			return (*it);
	}
	return NULL;
}
