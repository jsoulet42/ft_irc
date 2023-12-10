
#include "./includes/Channel.hpp"

void msgError(std::string const &code, User &user, std::string const &msg);

//-------------------------------Copelien form--------------------------------//
Channel::Channel(User *user, std::string &name)
{
	this->maxUsers = 10;
	this->nbUsers = 1;
	this->name = name;
	this->users.push_back(user);
	this->operators.push_back(user);
	this->invitedUsers.push_back(user);
	this->initModeMap();
}
Channel::Channel(Channel const &src)
{
	*this = src;
}
Channel::~Channel()
{
}
Channel &	Channel::operator=(Channel const &rSym)
{
	if (this != &rSym)
	{
		this->maxUsers = rSym.maxUsers;
		this->nbUsers = rSym.nbUsers;
		this->name = rSym.name;
		this->users = rSym.users;
		this->operators = rSym.operators;
		this->invitedUsers = rSym.invitedUsers;
	}
	return *this;
}
//------------------------------Getter & Setter-------------------------------//
//-------------------------------Other function-------------------------------//

//modifié par julien le 02/12/2023
int Channel::addUser(User *user, std::string &password)
{
	if (this->password.compare(password) == 0)
	{
		this->users.push_back(user);
		this->nbUsers++;
		return 0;
	}
	else
		return -1;
}

/* return 1 si une erreur a eter trouver dans un checkMode()*/
bool Channel::ft_checkMode(Channel *channel, User &user, std::string mode)
{
	std::map<std::string, bool>::iterator it = channel->modeTab.find(mode);
	if (it != channel->modeTab.end())
	{
		if (it->second == true)
			return true;
	}
	return false;
}

bool Channel::checkModeI(User &user)
{

}

void Channel::checkModeK(User &user)
{
	(void)user;
}

bool Channel::checkModeL(User &user)
{
	if (this->users.size() >= this->modeLMaxUser)
	{
		send(); // ERR_CHANNELISFULL (471)
		throw;
	}
	return true;
}

void Channel::checkModeO(User &user)
{
	(void)user;
}

void Channel::checkModeT(User &user)
{
	(void)user;
}

void Channel::ft_insertChanMode(std::string strmess, User &user, Server &server, Channel &chan)
{
	char symbol;
	size_t i;
	if (strmess[0] == '+' || strmess[0] == '-')
		symbol = strmess[0];
	else
	{
		send(error404);
		throw;
	}
	std::vector<User*>::iterator it = chan.operators.begin();
	while (it != chan.operators.end())
	{
		if (user.nickname != it.nickname)
			++it;
		else
			break;
	}
	if (it == chan.operators.end())
	{
	//	send(ERRORI482); "<client> <channel> :You're not channel operator"
		throw;
	}
	strmess.erase(0, 1);
	if (!strmess.empty())
	{
		for (i = 0; i < strmess.find(" "); ++i)
		{
			if (strmess[i] == 't')
				chan.setModeT(symbol);
			else if (strmess[i] == 'i')
				chan.setModeI(symbol);
			else if (strmess[i] == 'l')
			{
				chan.setModeL(symbol, strmess);
				break;
			}
			else if (strmess[i] == 'k')
			{
				chan.setModeK(symbol, strmess);
				break;
			}
			else if (strmess[i] == 'o')
			{
				chan.setModeO(symbol, strmess, chan, user);
				break;
			}
			else if (strmess[i] == '+' || strmess[i] == '-')
				symbol = strmess[i];
		}
		for (size_t j = i; j < strmess.find(" "); ++j)
		{
			if (strmess[j] == 't')
				chan.setModeT(symbol);
			else if (strmess[j] == 'i')
				chan.setModeI(symbol);
			else if (strmess[j] == '+' || strmess[j] == '-')
				symbol = strmess[j];
		}
	}
}

void Channel::initModeMap()
{
	std::map<std::string, bool> tab;
	tab.insert(std::pair<std::string, bool>("modeT", false));
	tab.insert(std::pair<std::string, bool>("modeO", false));
	tab.insert(std::pair<std::string, bool>("modeL", false));
	tab.insert(std::pair<std::string, bool>("modeK", false));
	tab.insert(std::pair<std::string, bool>("modeI", false));
	this->modeTab = tab;
}

void Channel::setModeI(char c)
{
	std::map<std::string, bool>::iterator it = this->modeTab.find("modeI");
	if (it != modeTab.end())
	{
		if (c == '+')
			it->second = true;
		else
			it->second = false;
	}
}

void Channel::setModeK(char c)
{

}

void Channel::setModeL(char symbol, std::string strmess)
{
	int resultat = MAXUSER;
	std::map<std::string, bool>::iterator it = this->modeTab.find("modeL");
	if (symbol == '-')
	{
		this->modeLMaxUser = resultat;
		if (it != modeTab.end())
			it->second = false;
		std::cout << "mode correctly removed" << std::endl;
	}
	if (strmess.find(" ") != std::string::npos)
	{
		strmess.erase(0, (strmess.find(" ") + 1)); // et quil ya un int apres, on recup le int et le met dans modLMaxUser
		for (size_t i = 0; i < strmess.size(); i++)
		{
			if (!std::isdigit(strmess[i]))
			{
				send(erreur inconnu);
				throw;
			}
		}
	}
	try {
        resultat = std::stoi(strmess);
    }
	catch (const std::invalid_argument& e) {
        std::cerr << "Erreur d'argument invalide : " << e.what() << std::endl;
		return;
    } catch (const std::out_of_range& e) {
        std::cerr << "Dépassement de capacité : " << e.what() << std::endl;
		return;
    }
	it = this->modeTab.find("modeL");
	if (it != modeTab.end())
	{
		if (symbol == '+')
			it->second = true;
		else
			it->second = false;
	}
	this->modeLMaxUser = resultat;
	std::cout << "mode correctly added" << std::endl;
}

void Channel::setModeO(char symbol, std::string strmess, Channel &chan, User &user)
{
	std::map<std::string, bool>::iterator it_m = this->modeTab.find("modeL");
	std::vector<User*>::iterator it_v = chan.users.begin();
	std::string nameParse;
	if (strmess.find(" ") != std::string::npos)
	{
		strmess.erase(0, (strmess.find(" ") + 1));
		nameParse = strmess.substr(0, strmess.size());
		while (it_v != chan.users.end())
		{
			if (nameParse != it_v.nickname)
				++it_v;
			else
				break;
		}
		if (it_v == chan.users.end())
		{
		//	send(ERRORI482); "<client> <channel> :You're not channel operator"
			throw;
		}
		if (symbol == '-') // faire la verif si il est dans operator et lenlever
			std::vector<User*>::iterator = std::find(myVector.begin(), myVector.end(), valueToFind);
			chan.operators.find(nameParse);
			chan.operators.erase(nameParse);
		if (symbol == '+') // faire la verif si l est pas deja operator et l'ajouter
			chan.operators.push_back(it_v);
	}
}

void Channel::setModeT(char c)
{
	std::map<std::string, bool>::iterator it = this->modeTab.find("modeT");
	if (it != modeTab.end())
	{
		if (c == '+')
			it->second = true;
		else
			it->second = false;
	}
}

//------------------------------Ostream overload------------------------------//
std::ostream &	operator<<(std::ostream & o, Channel const &rSym)
{
	(void)rSym;
	o << "nothing";
	return o;
}

Channel *findChanelbyNameMatt(std::string name, std::vector<Channel *> &chanelList)
{
	if (name.compare(0, 1, "#") == 0 || name.compare(0, 1, "&") == 0)
		name.erase(0, 1);
	//else
		//on peux décider de renvoyer une erreur ou de ne rien faire
	for (std::vector<Channel *>::iterator it = chanelList.begin(); it != chanelList.end(); it++)
	{
		if ((*it)->name == name)
			return (*it);
	}
	return NULL;
}
