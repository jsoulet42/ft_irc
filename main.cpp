#include "ft_irc.hpp"
#include "Server.hpp"

int main(int argc, char const* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]"<<std::endl;
		return 0;
	}

	std::string		message;
	char			buffer[BUFFSIZE + 1];
	int				rc = 1, \
					i = 0, \
					fdsId = 1, \
					newSd = 0, \
					close_conn = 0;

	Server			*server;
	try
	{
		server = new Server(std::atoi(argv[1]), argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << "[ERROR] : Error during server creation : " << std::endl;
		std::cerr <<  e.what() << std::endl;
		delete server;
	}

	while (1)
	{
		std::cout << "Waiting incoming connection ( poll() )..." << std::endl;
		rc = poll(&(server->fdP[0]), fdsId, -1);
		server->fdNb = fdsId;
		for (int i = 0; i < server->fdNb; i++)
		{
			if(server->fdP[i].revents == 0)
				continue;
			newSd = 0;
			if (server->fdP[i].fd == server->getServerSocket()) // si serveur socket == fd
			{
				while (newSd != -1)
				{
					newSd = accept(server->getServerSocket(), NULL, NULL);
					if (newSd != -1)
					{
						// nouvel utilisateur
					}
				}
			}
			else if (server->fdP[i].fd == -1)
				break;
			/*else
			{
				rc = recv(server->fdP[i].fd, buffer, sizeof(buffer), 0);
				if (rc == 0)
				{
					// ctrl-c de netcat, utilisateur quit
				}
				buffer[rc] = '\0';
				if (server->haveN(strmess) == false) // pas de \n a la fin du message, ctrl-d
				{
				}
				else
					//interpret command
			}
			std::cout << DIVIDER << RESET << std::endl;*/
		}
	}
	std::cout << "END OF PROGRAM" << std::endl;
	return 0;
}
