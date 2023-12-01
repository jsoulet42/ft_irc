#ifndef _CHANNEL_HPP_
# define _CHANNEL_HPP_

# include <ft_irc.hpp>
# include <User.hpp>
# include <Server.hpp>


class Channel
{
//-----------------------------------Public-----------------------------------//
	public:
//-------------------------------Copelien form--------------------------------//
		Channel(User *user, std::string name);
		Channel(Channel const &src);
		~Channel();
		Channel &	operator=(Channel const &rSym);

		int						maxUsers;
		int						nbUsers;
		std::string				name;
		// std::string				topic;
		// std::string				lastTopicUpdateWhen;
		// std::string				lastTopicUpdateWho;
		// std::string				mode;
		// std::string				password;
		std::vector<User *>		users;
		std::vector<User *>		operators;
		std::vector<User *>		invitedUsers;
//-----------------------------Operators overload-----------------------------//
//------------------------------Getter & Setter-------------------------------//
//-------------------------------Other function-------------------------------//
		void ft_kick(std::string buffer, User *user);
//----------------------------------Exeption----------------------------------//
//----------------------------------Private-----------------------------------//
	private:
		Channel(){}; // default constructor
};
//------------------------------Ostream overload------------------------------//
std::ostream &	operator<<(std::ostream & o, Channel const &rSym);

#endif
