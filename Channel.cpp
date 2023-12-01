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
void Channel::addUser(User *user, std::string password)
{
	if (this->password == password)
	{
		this->users.push_back(user);
		this->nbUsers++;
	}
	else
		throw std::exception();
}
//------------------------------Ostream overload------------------------------//
std::ostream &	operator<<(std::ostream & o, Channel const &rSym)
{
	(void)rSym;
	o << "nothing";
	return o;
}
