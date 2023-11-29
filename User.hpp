
#pragma once

#include "ft_irc.hpp"

class User
{
	public :
			int 			_fdUser;
			std::string		nickname;
			std::string		username;
			std::string		realname;
			int				_forNcProtocol;

			User(int &fdUser);
			~User();


	private :

};
