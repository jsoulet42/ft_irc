# include <ft_irc.hpp>
# include <Server.hpp>
# include <User.hpp>
# include <Channel.hpp>

Channel *findChanelbyNameMatt(std::string name, std::vector<Channel *> &chanelList);
std::vector<std::string> splitString(const std::string &input, char delimiter);
