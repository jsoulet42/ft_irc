# include "./includes/ft_irc.hpp"
# include "./includes/Server.hpp"
# include "./includes/User.hpp"
# include "./includes/Channel.hpp"


// fait par le Baron Sanglant le 11 aout de l'an de grâce 1943 ich bin ein berliner
std::vector<std::string> splitString(const std::string &input, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string token;

	while (std::getline(ss, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}

// fonction pour enlever les \r\n des messages a partir du charactere n
// s'il ne trouve pas \r\n renvoie la phrase d'origine
std::string	extractSubstring(std::string const &msg, int n)
{
	if (n < 0)
		n = 0;
    ssize_t startPos = msg.find("\r\n", n); // Trouve la position du prochain retour à la ligne après le nième caractère
	if (startPos == -1) // = j'ai pas trouve le \r\n
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