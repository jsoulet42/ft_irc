#pragma once

#include "ft_irc.hpp"

class Utils
{
	public :
			std::vector<std::string> splitString(const std::string &input, char delimiter);
			std::string	extractSubstring(std::string const &msg, int n);
			void printMessageSendToClient(std::string fonction, User *user, std::string message);

	private :
};