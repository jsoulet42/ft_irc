
#pragma once

#include "ft_irc.hpp"

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
			bool	operator==(User const &rSym);
	private :
};

bool findUser(User &user, std::vector<User *> &userList);
