
#include "./includes/ft_irc.hpp"

User::User(int &fdUser) : _fdUser(fdUser)
{
	_forNcProtocol = 1;
}

User::~User()
{
}

bool	User::operator==(User const &rSym)
{
	return this->nickname == rSym.nickname;
}

/// @brief find a user in a channel
/// \param user the user we are looking for
/// \param userList the vector of user pointers in which we are searching
/// \return true if the user is found, false otherwise
bool findUser(User const &user, std::vector<User *> &userList)
{
	if (userList.empty())
		return false;
	for (std::vector<User *>::iterator it = userList.begin(); it != userList.end(); it++)
	{
		if (*(*it) == user)
			return true;
	}
	return false;
}

template <typename T, typename container>
bool findUser(T const &user, container &userList)
{
	return (std::find(userList.begin(), userList.end(), user) != userList.end());
}
