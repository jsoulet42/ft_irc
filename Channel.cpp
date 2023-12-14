
#include "includes/ft_irc.hpp"

void printMessageSendToClientChannel(std::string fonction, User &user, std::string message)
{
	std::cout << "J'ai envoye au client le message : |" << message << "| de |" << user.nickname << "| pour la fonction |" << fonction << "|" << std::endl;
}

void msgError(std::string const &code, User &user, std::string const &msg);

//-------------------------------Copelien form--------------------------------//
Channel::Channel(User *user, std::string &name)
{
	this->maxUsers = 10;
	this->nbUsers = 1;
	this->name = name;
	this->invitedUsers.push_back(user);
	this->initModeMap();
	this->password = "";
	this->modeLMaxUser = DEFAULTMAXUSER;
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
	if (!this->password.empty())
	{
		if (this->password.compare(password) == 0)
		{
			this->users.push_back(user);
			this->operators.insert(std::pair<User *, bool>(user, false));
			this->nbUsers++;
			return 0;
		}
		else
			return -1;
	}
	else if (findUserInChannel(this, user) == false)
	{
		this->users.push_back(user);
		this->nbUsers++;
		return 0;
	}
	else
		return -1;
}

bool Channel::ft_checkMode(Channel *channel, std::string mode)
{
	std::map<std::string, bool>::iterator it = channel->modeTab.find(mode);
	if (it != channel->modeTab.end())
	{
		if (it->second == true)
			return true;
	}
	return false;
}

void Channel::ft_insertChanMode(std::string strmess, User &user, Channel &chan)
{
	char symbol;
	std::string tempFor;
	if (!strmess.empty() && (strmess[0] == '+' || strmess[0] == '-'))
		symbol = strmess[0];
	else
	{
		msgError("403", user, ERRORM403);
		std::cout << "ERRORM403" << std::endl;
		return;
	}
	strmess.erase(0, 1);
	if (!strmess.empty())
	{
		std::cout << "je rentre ds la for    -> " << strmess[0] << std::endl;
		tempFor = strmess;
		for (size_t i = 0; i < tempFor.size(); i++)
		{
			if (tempFor[i] == ' ')
				break;
			else if (tempFor[i] == 't')
			{
				std::cout << "je rentre dans t  ----------  " << strmess[i] << std::endl;
				chan.setModeT(symbol);
			}
			else if (tempFor[i] == 'b')
				return;
			else if (tempFor[i] == 'i')
			{
				std::cout << "je suis ds i  ----------------  " << strmess[i] << std::endl;
				chan.setModeI(symbol);
			}
			else if (tempFor[i] == 'l')
			{
				std::cout << "je rentre dans l ---------------------  " << strmess[i] << std::endl;
				chan.setModeL(symbol, strmess);
			}
			else if (tempFor[i] == 'k')
			{
				std::cout << "je rentre dans k -----------------  " << strmess[i] << std::endl;
				chan.setModeK(symbol, strmess);
			}
			else if (tempFor[i] == 'o')
			{
				chan.setModeO(symbol, strmess, chan, user);
				std::cout << "je rentre dans o ----------------  " << strmess[i] << std::endl;
			}
			else if (tempFor[i] == '+' || tempFor[i] == '-')
			{
				std::cout << "je rentre dans - ou + ----------------------  " << strmess[i] << std::endl;
				symbol = tempFor[i];
			}
		}
	}
	std::cout << "je sort du for" << std::endl;
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
		{
			it->second = true;
			std::cout << "mode +i added" << std::endl;
		}
		else
		{
			it->second = false;
			std::cout << "mode -i removed" << std::endl;
		}
	}
}

void Channel::setModeK(char symbol, std::string &strmess)
{
	std::map<std::string, bool>::iterator it = this->modeTab.find("modeK");
	std::string temp;

	if (symbol == '-')
	{
		it->second = false;
		this->password = "";
	}
	else
	{
		if (strmess.find(" ") != std::string::npos)
			strmess.erase(0, (strmess.find(" ") + 1));
		if (!strmess.empty())
		{
			temp = strmess.substr(0, strmess.find(" "));
			// strmess.erase(0, (strmess.find(" ") + 1));
			for (size_t i = 0; i < (strmess.size() && strmess[i] != ' '); i++)
			{
				if (strmess[i] == '#' || strmess[i] == ',')
		//	send(error 525); // "<client> <target chan> :Key is not well-formed"
				std::cout << "Key is not well formed" << std::endl;
				return;
			}
			this->password = temp;
			it->second = true;
		}
		else
		{
		//	send(error 525); // "<client> <target chan> :Key is not well-formed"
			std::cout << "Wrong param mode" << std::endl;
			return;
		}
	}
	std::cout << "Mode +k added and password '" << temp << "' is active" << std::endl;
}

void Channel::setModeL(char symbol, std::string &strmess)
{
	int resultat;
	std::map<std::string, bool>::iterator it = this->modeTab.find("modeL");
	if (strmess[0] != 'l')
		return;
	if (symbol == '-')
	{
		this->modeLMaxUser = DEFAULTMAXUSER;;
		if (it != modeTab.end())
			it->second = false;
		std::cout << "mode -l correctly removed" << std::endl;
		return;
	}
	else if (strmess.find(" ") != std::string::npos)
	{
		strmess.erase(0, (strmess.find(" ") + 1));
		for (size_t i = 0; i < strmess.find(" "); i++)
		{
			if (!strmess.empty())
			{
				std::cout << "je suis lLALALALALAL" << std::endl;
				for (size_t i; i < (strmess.size() && strmess[i] != ' '); i++)
				{
					if (!std::isdigit(strmess[i]))
					{
					//	send(erreur inconnu);
						//throw;
						std::cout << "not a number, mode +l cancelled" << std::endl;
					}
				}
			}
		}
	}
	std::string temp = strtok((char *)strmess.c_str(), (char *)strmess.find(" "));
	try {
        resultat = std::atoi(temp.c_str());
    }
	catch (const std::invalid_argument& e) {
        std::cerr << "Erreur d'argument invalide : " << e.what() << std::endl;
		return;
    } catch (const std::out_of_range& e) {
        std::cerr << "Dépassement de capacité : " << e.what() << std::endl;
		return;
    }
	it->second = true;
	this->modeLMaxUser = resultat;
	std::cout << "mode +l correctly added" << std::endl;
}

void Channel::setModeO(char symbol, std::string &strmess, Channel &chan, User &user)
{
	std::string nameParse;
	if (strmess.find(" ") != std::string::npos)
	{
		strmess.erase(0, (strmess.find(" ") + 1));
		if (!strmess.empty())
			nameParse = strmess.substr(0, strmess.find(" "));
		User *tempUser = findUserByName(chan.users, nameParse);
		if (symbol == '-')
		{
			if (tempUser)
			{
				std::map<User*, bool>::iterator it = this->operators.find(tempUser);
				if (it != this->operators.end())
				{
					operators.erase(tempUser);
					std::cout << "Operator " << tempUser << " removed" << std::endl;
				}
			}
			else
				std::cout << "User is not an operator" << std::endl;
		}
		else if (symbol == '+')
			if (checkRightsUserInChannel(&chan, &user, OPERATOR) == false)
			{
				this->operators.insert(std::pair<User*, bool>(tempUser, true));
				std::cout << "User "<< tempUser << " is now an operator" << std::endl;
			}
	}
	else
	{
	//	renvoiyer a l'utiisateur la liste des operators
	}
}

void Channel::setModeT(char c)
{
	std::map<std::string, bool>::iterator it = this->modeTab.find("modeT");
	if (it != modeTab.end())
	{
		if (c == '+')
		{
			it->second = true;
			std::cout << " Mode +t successfully added" << std::endl;
		}
		else
		{
			it->second = false;
			std::cout << " Mode -t successfully removed" << std::endl;
		}
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
	std::cout << name << "|";
	for (std::vector<Channel *>::iterator it = chanelList.begin(); it != chanelList.end(); it++)
	{
		std::cout << (*it)->name << "|";
		if ((*it)->name == name)
			return (*it);
	}
	return NULL;
}

void	Channel::channelSendLoop(std::string message, int & sFd)
{
	std::vector<User *>::iterator	it = this->users.begin();

	while (it != this->users.end())
	{
		if (sFd != (*it)->_fdUser)
		{
			send((*it)->_fdUser, message.c_str(), message.length(), 0);
			printMessageSendToClientChannel("Channel send loop - user", (*(*it)), message);
		}
		it++;
	}
	//it = this->operators.begin();
	//while (it != this->operators.end())
	//{
	//	if (sFd != (*it)->_fdUser)
	//	{
	//		send((*it)->_fdUser, message.c_str(), message.length(), 0);
	//		printMessageSendToClientChannel("Channel send loop - operator", (*(*it)), message);
	//	}
	//	it++;
	//}
}

bool	Channel::isInChannel(User *user)
{
	if (!user)
		return false;

	std::vector<User *>::iterator		it = this->users.begin();

	while (it != this->users.end())
	{
		if (user == *it)
			return true;
		it++;
	}
	return false;
}

/*bool	Channel::isOpInChannel(User *user)
{
	if (!user)
		return false;

	//std::vector<User *>::iterator		it = this->operators.begin();

	//while (it != operators.end())
	//{
	//	if (user == *it)
	//	{
	//		//std::cout << std::endl << std::endl << std::endl << "User finded" << std::endl;
	//		return true;
	//	}
	//	it++;
	//}
	return false;
}*/

bool	Channel::isModeT()
{
	//if (this->mode.find('t') != std::string::npos)
	//	return true;
	std::cout << "ici il faut une fonction qui verifie que le channel est en mode T" << std::endl;
	return false;
}

void Channel::deleteUserInChannel(User *user)
{
	std::vector<User *>::iterator		it = this->users.begin();

	while (it != this->users.end())
	{
		if (user == *it)
		{
			this->users.erase(it);
			this->nbUsers--;
			return;
		}
		it++;
	}
}
