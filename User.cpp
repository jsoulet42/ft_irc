
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
