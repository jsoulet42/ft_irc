#include "includes/ft_irc.hpp"

void replaceChars(std::string &input, char target, char dest)
{
	for (size_t i = 0; i < input.size(); ++i)
	{
		if (input[i] == target)
		{
			input[i] = dest;
		}
	}
}

/// @brief renvoie un vecteur de string contenant les mots separes par le charactere delimiter, s'il y a des virgules dans les mots, ils sont separes en plusieurs mots
std::vector<std::string> splitString(const std::string &input, char delimiter)
{
	std::vector<std::string> tokens;
	std::string output = input;
	replaceChars(output, ',', ' ');
	std::stringstream ss(output);
	std::string token;

	while (std::getline(ss, token, delimiter))
	{
		token = strtok((char *)token.c_str(), "\r\n");
		std::cout << "token : " << token << "|" << std::endl;
		tokens.push_back(token);
	}
	return tokens;
}

bool findElement(User const &user, std::vector<User *> &userList)
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

int countSpaces(const std::string &str, const char &delimiter)
{
	int count = 0;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == delimiter)
		{
			count++;
		}
	}
	return count;
}

void printMessageSendToClient(std::string fonction, User &user, std::string message)
{
	std::cout << "J'ai envoye au client le message : |" << message << "| de |" << user.nickname << "| pour la fonction |" << fonction << "|" << std::endl;
}
