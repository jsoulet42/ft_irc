# include "ft_irc.hpp"
# include "Server.hpp"
# include "User.hpp"
# include "Channel.hpp"


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
