
#include "User.hpp"

User::User(int &fdUser) : _fdUser(fdUser)
{
	_forNcProtocol = 1;
}

User::~User()
{
}
