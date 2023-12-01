#include "Channel.hpp"

//-------------------------------Copelien form--------------------------------//
Channel::Channel(User *user, std::string name)
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
	}
	return *this;
}
//------------------------------Getter & Setter-------------------------------//
//-------------------------------Other function-------------------------------//
//------------------------------Ostream overload------------------------------//
std::ostream &	operator<<(std::ostream & o, Channel const &rSym)
{
	(void)rSym;
	o << "nothing";
	return o;
}

void Channel::ft_kick(std::string buffer, User *user)
{

}
