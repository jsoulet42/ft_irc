
#include "ft_irc.hpp"
#include "Server.hpp"
#include "User.hpp"

bool isRunning = true;

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		std::cout << "Received SIGINT. Shutting down..." << std::endl;
		isRunning = false;
	}
}

int main(int argc, char const* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]"<<std::endl;
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
		std::cout << "Waiting incoming connection ( poll() )..." << std::endl;
		rc = poll(&(server->fdP[0]), fdsId, -1);
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
							std::cout << "New user connected with id " << newFd << "." << std::endl;
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
				if (rc == 0)	// Lorsque le retour de recv cela signifie que le client a fermé la connexion
				{
					// ctrl-c de netcat, utilisateur quit
				}
				buffer[rc] = '\0';
				std::string strmess(buffer);
				// if (server->haveN(strmess) == false) // pas de \n a la fin du message, ctrl-d
				// {
				// }
				// else
				// {
				// 	interpret command(server, strmess, server->fdP[i].fd)
				// }
			}
			// std::cout << DIVIDER << RESET << std::endl;
		}
	}
	std::cout << "END OF PROGRAM" << std::endl;
	return 0;
}
