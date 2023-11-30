#ifndef _CHANNEL_HPP_
# define _CHANNEL_HPP_

# include <ft_irc.hpp>

class Channel
{
//-----------------------------------Public-----------------------------------//
	public:
//-------------------------------Copelien form--------------------------------//
		Channel();
		Channel(Channel const &src);
		~Channel();
		Channel &	operator=(Channel const &rSym);
//-----------------------------Operators overload-----------------------------//
//------------------------------Getter & Setter-------------------------------//
//-------------------------------Other function-------------------------------//
//----------------------------------Exeption----------------------------------//
//----------------------------------Private-----------------------------------//
	private:
};
//------------------------------Ostream overload------------------------------//
std::ostream &	operator<<(std::ostream & o, Channel const &rSym);

#endif
