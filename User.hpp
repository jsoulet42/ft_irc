
#pragma once

#include "ft_irc.hpp"
#include "Utils.hpp"


class User
{
	public :
			int 			_fdUser;
			int				_forNcProtocol;
			std::string		nickname;
			std::string		username;
			std::string		realname;

			User(int &fdUser);
			~User();


	private :

};
