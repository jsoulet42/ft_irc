
#pragma once

# include "ft_irc.hpp"
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
		/// @brief map of mode soit "modeI", "modeK", "modeL", "modeO", "modeT"
		std::map<std::string, bool> modeTab;
		void (Channel::*ftPtr[5])(User &user);
		long unsigned int modeLMaxUser;
		bool modeI;
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
		//std::vector<User *>		operators;
		std::map<std::string, bool>		operators;
		std::vector<User *>		invitedUsers;

    void	channelSendLoop(std::string message, int & sFd);
		bool	isInChannel(User *user);
		bool	isOpInChannel(User *user);
		bool	isModeT();
//-----------------------------Operators overload-----------------------------//
//------------------------------Getter & Setter-------------------------------//
//-------------------------------Other function-------------------------------//

		int addUser(User *user, std::string &cmd);
		void ft_fillPtrCheckMode();
		void ft_checkMode(Channel *channel, User &user);
		void checkModeI(User &user);
		void checkModeK(User &user);
		void checkModeL(User &user);
		void checkModeO(User &user);
		void checkModeT(User &user);

		/// @brief
		/// @param nameMode soit "modeI", "modeK", "modeL", "modeO", "modeT"
		/// @return
		bool checkRights(std::string &nameMode)
		{
			return (modeTab[nameMode]);
		}

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


