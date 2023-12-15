
#include "includes/ft_irc.hpp"

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
	if (this->password != "")
	{
		if (this->password.compare(password) == 0)
		{
			std::cout << GREEN << ON_BLACK << "password ok" <<  RESET << std::endl;
			this->users.push_back(user);
			this->operators.insert(std::pair<User *, bool>(user, false));
			this->nbUsers++;
			return 0;
		}
		else
			return -1;
	}
	else if (findUserInChannel(this, user) == false && this->password == "")
	{
		std::cout << GREEN << ON_BLACK << "this channel no need pass" << RESET << std::endl;
		this->operators.insert(std::pair<User *, bool>(user, false));
		this->users.push_back(user);
		this->nbUsers++;
		return 0;
	}
	else
		return -1;
}

void Channel::deleteUserInOperator(User *user)
{
	std::map<User *, bool>::iterator it = this->operators.begin();

	while (it != this->operators.end())
	{
		if (it->first == user)
		{
			this->operators.erase(it);
			return;
		}
		it++;
	}
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
				std::cout << "je rentre dans o ----------------  " << strmess[i] << std::endl;
				chan.setModeO(symbol, strmess, chan, user);
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
		std::cout << strmess << "     ----- test strmess" << std::endl;
		if (!strmess.empty())
		{
			temp = strmess.substr(0, strmess.size());
			// strmess.erase(0, (strmess.find(" ") + 1));
			for (size_t i = 0; i < strmess.size(); i++)
			{
				if (strmess[i] == '#' || strmess[i] == ',' || strmess[i] == ' ')
				{
				//	send(error 525); // "<client> <target chan> :Key is not well-formed"
					std::cout << "Key is not well formed" << std::endl;
					return;
				}
			}
			std::cout << temp << std::endl;
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
		std::cout << "je suis dans +l    le nbr = " << strmess << std::endl;
		for (size_t i = 0; i < strmess.size(); i++)
		{
			if (!strmess.empty())
			{
				if (!std::isdigit(strmess[i]))
				{
				//	send(erreur inconnu);
					//throw;
					std::cout << "not a number, mode +l cancelled" << std::endl;
					return;
				}
			}
		}
	}
	//std::string temp = strtok((char *)strmess.c_str(), (char *)strmess.find(" "));
	try
	{
		resultat = std::atoi(strmess.c_str());
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << "Erreur d'argument invalide : " << e.what() << std::endl;
		return;
	} catch (const std::out_of_range& e)
	{
		std::cerr << "Dépassement de capacité : " << e.what() << std::endl;
		return;
	}
	it->second = true;
	this->modeLMaxUser = resultat;
	std::cout << "mode +l correctly added" << resultat << this->modeLMaxUser << std::endl;
}

void Channel::setModeO(char symbol, std::string &strmess, Channel &chan, User &user)
{
	(void)user;
	std::string nameParse;
	if (strmess.find(" ") != std::string::npos)
	{
		strmess.erase(0, (strmess.find(" ") + 1));
		if (strmess.empty())
		{
			// send;
			// throw;
			return;
		}
		nameParse = strmess.substr(0, strmess.find(" "));
		User *tempUser = findUserByName(chan.users, nameParse);
		std::map<User*, bool>::iterator it = this->operators.begin();
		while (it != this->operators.end())
		{
			if (tempUser->nickname == it->first->nickname)
				break;
			++it;
		}
		std::cout << nameParse << "     et  " << tempUser->nickname << std::endl;
		it = this->operators.find(tempUser);
		if (symbol == '-')
		{
			if (it != this->operators.end())
			{
				it->second = false;
				std::cout << "Operator " << tempUser << " removed" << std::endl;
			}
			else
				std::cout << "User is not an operator" << std::endl;
		}
		else if (symbol == '+')
		{
				it->second = true;
				std::cout << "User "<< nameParse << " is now an operator" << std::endl;
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

bool Channel::checkModeL()
{
	long unsigned int count = 1;
	std::vector<User*>::iterator it = this->users.begin();
	for (; it != this->users.end(); it++)
	{
		count++;
	}
	if (count < this->modeLMaxUser)
		return true;
	return false;
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
			printMessageSendToClient("Channel send loop - user", (*(*it)), message);
		}
		it++;
	}
	//it = this->operators.begin();
	//while (it != this->operators.end())
	//{
	//	if (sFd != (*it)->_fdUser)
	//	{
	//		send((*it)->_fdUser, message.c_str(), message.length(), 0);
	//		printMessageSendToClient("Channel send loop - operator", (*(*it)), message);
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

void Channel::deleteUserInChannel(User &user)
{
	std::vector<User *>::iterator		it = this->users.begin();

	while (it != this->users.end())
	{
		if (&user == *it)
		{
			this->users.erase(it);
			this->nbUsers--;
			return;
		}
		it++;
	}
}

User *Channel::getOperator()
{
	std::map<User *, bool>::iterator	it = this->operators.begin();

	while (it != this->operators.end())
	{
		if (it->second == true)
			return it->first;
		it++;
	}
	return NULL;
}

void Channel::getDateTime()
{
	std::time_t rawtime;
	std::time(&rawtime);

	std::ostringstream oss;
	oss << rawtime;
	this->creationDate = oss.str();
}
