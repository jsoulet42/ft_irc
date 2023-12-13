
#include "./includes/ft_irc.hpp"

bool isRunning = true;

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		std::cout << RED << ON_BLACK << "Received SIGINT. Shutting down..." << RESET << std::endl;
		isRunning = false;
	}
}

int main(int argc, char const* argv[])
{
	if (argc != 3)
	{
		std::cout << RED << ON_BLACK << "Usage: ./ircserv [port] [password]" << RESET << std::endl;
		return 0;
	}

	std::string		message;
	char			buffer[BUFFSIZE + 1];
	int				rc = 1;
					//i = 0,
	int				fdsId = 1;
	int				newFd = 0;
					//close_conn = 0;

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
	signal(SIGINT, signalHandler);
	while (isRunning)
	{
		std::cout << BLUE << ON_BLACK << "Waiting incoming connection ( poll() )..." << RESET << std::endl;
		rc = poll(&(server->fdP[0]), fdsId, -1);
		std::cout << "[" << rc << "]" << std::endl;
		server->fdNb = fdsId;
		for (int i = 0; i < server->fdNb; i++)
		{
			if(server->fdP[i].revents == 0)
				continue;
			newFd = 0;
			if (server->fdP[i].fd == server->getServerSocket()) // si serveur socket == fd
			{
				while (newFd != -1)
				{
					newFd = accept(server->getServerSocket(), NULL, NULL);
					if (newFd != -1)	// lorsque le retour de accept est différent de -1 cela signifie qu'un client demande une connexion
					{
						try
						{
							std::cout << YELLOW << ON_BLACK << "New user connected with id " << newFd << "." << RESET << std::endl;
							server->protocolNewUser(newFd);
							fdsId++;
						}
						catch (const std::exception& e)
						{
							std::cerr << "[Error] during user creation : " << std::endl;
							std::cerr <<  e.what() << std::endl;
							server->deleteUser(newFd);
							close(newFd);
						}
					}
				}
			}
			else if (server->fdP[i].fd == -1)
				break;
			else
			{
				rc = recv(server->fdP[i].fd, buffer, sizeof(buffer), 0);
				if (rc == -1)
				{

				}
				if (rc == 0)	// Lorsque le retour de recv cela signifie que le client a fermé la connexion
				{
					close(server->fdP[i].fd);
					server->deleteUser(server->fdP[i].fd);
					//server->checkChannel(); a faire et comprendre
					server->fdP[i].fd = -1;
					std::cout << "User succesfully deleted" << std::endl;
					std::cout << std::endl;
					continue;
					// ctrl-c de netcat, utilisateur quit
				}
				buffer[rc] = '\0';
				std::string strmess(buffer);
				if (server->haveN(strmess) == false) // pas de \n a la fin du message, ctrl-d
				{

				}
				else
				{
					try
					{
						interpretCommand(*server, strmess, server->fdP[i].fd);
						strmess.clear();
						bzero(buffer, BUFFSIZE + 1);
					}
					catch(const std::exception& e)
					{
						std::cerr << e.what() << '\n';
					}

				}
			}
			// std::cout << DIVIDER << RESET << std::endl;
		}
	}
	std::cout << GREEN << ON_BLACK << "END OF PROGRAM" << RESET << std::endl;
	return 0;
}
