
#pragma once

# include <ft_irc.hpp>
# include <User.hpp>
# include <Server.hpp>
# include <map>

extern bool errorCmd;

class Channel
{
//-----------------------------------Public-----------------------------------//
	public:
//-------------------------------Copelien form--------------------------------//
		Channel(User *user, std::string &cmd);
		Channel(Channel const &src);
		~Channel();
		Channel &	operator=(Channel const &rSym);

		std::map<std::string, bool> modeTab;
		void (Channel::*ftPtr[5])(User &user);
		int modeLMaxUser;
		// bool modeI;
		// bool modeT;
		// bool modeK;
		// bool modeO;
		// bool modeL;

		int						maxUsers;
		int						nbUsers;
		std::string				name;
		std::string				topic;
		// std::string				lastTopicUpdateWhen;
		// std::string				lastTopicUpdateWho;
		// std::string				mode;
		std::string				password;
		std::vector<User *>		users;
		std::vector<User *>		operators;
		std::vector<User *>		invitedUsers;
//-----------------------------Operators overload-----------------------------//
//------------------------------Getter & Setter-------------------------------//
//-------------------------------Other function-------------------------------//
		int addUser(User *user, std::string &cmd);
		void ft_fillPtrCheckMode(User &user);
		void ft_checkMode(Channel &channel, User &user);
		void checkModeI(User &user);
		void checkModeK(User &user);
		void checkModeL(User &user);
		void checkModeO(User &user);
		void checkModeT(User &user);

//----------------------------------Exeption----------------------------------//
		class UserIsAlredyInChannelException : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("User is already in channel");
				};
		};
//----------------------------------Private-----------------------------------//
	private:
		Channel(){}; // default constructor
};

//------------------------------Ostream overload------------------------------//
std::ostream &	operator<<(std::ostream & o, Channel const &rSym);
//-------------------------------Other function-------------------------------//
Channel *findChanelbyNameMatt(std::string name, std::vector<Channel *> &chanelList);
