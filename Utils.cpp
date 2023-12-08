# include "includes/ft_irc.hpp"



/// @brief renvoie un vecteur de string contenant les mots separes par le charactere delimiter, s'il y a des virgules dans les mots, ils sont separes en plusieurs mots
std::vector<std::string> splitString(const std::string &input, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string token;

	while (std::getline(ss, token, delimiter))
	{
		if (token.find(",") != std::string::npos)
		{
			std::vector<std::string> tokens2 = splitString(token, ',');
			for (std::vector<std::string>::iterator it = tokens2.begin(); it != tokens2.end(); ++it)
			{
				std::string temp = *it;
				if (temp.find("\r\n") != std::string::npos)
				{
					temp = temp.substr(0, temp.size() - 2);
					tokens.push_back(temp);
					return tokens;
				}
				tokens.push_back(temp);
			}
		}
		else
		{
			if (!token.empty() && token[token.size() - 1] == '\r')
				token = token.substr(0, token.size() - 1);
			tokens.push_back(token);
		}
	}
	return tokens;
}

// fonction pour enlever les \r\n des messages a partir du charactere n
// s'il ne trouve pas \r\n renvoie la phrase d'origine
std::string extractSubstring(std::string const &msg, int n)
{
	if (n < 0)
		n = 0;
	ssize_t startPos = msg.find("\r\n", n); // Trouve la position du prochain retour à la ligne après le nième caractère
	if (startPos == -1)						// = j'ai pas trouve le \r\n
		return (msg);
	if (startPos >= n)
	{
		std::string extracted = msg.substr(n, startPos - n); // Extraire la sous-chaîne à partir du nième caractère
		return (extracted);
	}
	else
	{
		return (msg);
	}
}

template <typename K, typename T>
typename std::map<K, T>::const_iterator userInMap(const std::map<K, T> &inputMap, const User *userPtr)
{
	typename std::map<K, T>::const_iterator it;
	for (it = inputMap.begin(); it != inputMap.end(); ++it)
	{
		if (it->first == userPtr)
		{
			return it;
		}
	}
	return inputMap.end(); // Retourne la fin de la map si l'utilisateur n'est pas trouvé
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
