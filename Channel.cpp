#include "Channel.hpp"

//-------------------------------Copelien form--------------------------------//
Channel::Channel()
{
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
