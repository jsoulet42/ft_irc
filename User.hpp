
#pragma once

#include "ft_irc.hpp"

class User
{
	public :
			int 			_fdUser;
			std::string		_nickname;
			std::string		_realname;

			User(int &fdUser);
			~User();


	private :

};
