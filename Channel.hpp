
#pragma once

# include <ft_irc.hpp>
# include <User.hpp>
# include <Server.hpp>


class Channel
{
//-----------------------------------Public-----------------------------------//
	public:
//-------------------------------Copelien form--------------------------------//
		Channel(User *user, std::string &cmd);
		Channel(Channel const &src);
		~Channel();
		Channel &	operator=(Channel const &rSym);

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

