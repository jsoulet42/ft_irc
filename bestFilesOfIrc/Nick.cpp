int flags = fcntl(this->_serverSocket, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("[ERROR] : can't get socket flags, please check your permissions or cpu arch.");
	if (fcntl(this->_serverSocket, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("[ERROR] : can't set socket to non-blocking mode, please check your permissions or cpu arch.");
