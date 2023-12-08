
#include "./includes/User.hpp"

User::User(int &fdUser) : _fdUser(fdUser)
{
	_forNcProtocol = 1;
}

User::~User()
{
}

bool User::operator==(User const &rSym)
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



/// @brief 
/// @tparam T
/// @tparam Container
/// @param element
/// @param container
/// @return
template <typename T, typename Container>
bool findElement(const T &element, const Container &container)
{
	return std::find(container.begin(), container.end(), element) != container.end();
}
