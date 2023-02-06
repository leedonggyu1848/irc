#include "Server/Server.hpp"
#include "header.hpp"
#include "Utils.hpp"

static bool isAllNumber(const string& str) {
	for (size_t i = 0; i < str.length(); ++i)
		if (!isdigit(str[i])) return false;
	return true;
}

Server::Server(const string& port, const string& password) :_buf("") {
	if (port.length() > 5 || !isAllNumber(port)
		|| (_port = atoi(port.c_str())) > 65535)
		throw InitServerException();
	for (int i = 0; i < MAX_FD_SIZE; ++i) _pollFDs[i].fd = -1;
	_command = new Command();
	_password = password;
}

Server::~Server() {
	delete _command;
	close(_socket);
}

void	Server::prepare(void) {
	int opt = 1;
	struct sockaddr_in	sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(_port);
	if ((_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1
		|| setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1
		|| bind(_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1
		|| listen(_socket, LISTEN_QUEUE_SIZE) == -1) {
		close(_socket);
		throw InitServerException();
	}
	_pollFDs[0].fd = _socket;
	_pollFDs[0].events = POLLIN;
	_pollFDs[0].revents = 0;
}

void	Server::start(void) {
	int result;
	while (true) {
		result = poll(_pollFDs, MAX_FD_SIZE, -1);
		if (result > 0) {
			_acceptConnections();
			_sendResponse();
		}
	}
}

void	Server::_acceptConnections(void) {
	struct sockaddr_in	addr;
	socklen_t			addrSize;

	if (_pollFDs[0].revents != POLLIN) return;
	int	connectFD = accept(_socket, (struct sockaddr *)&addr, &addrSize);
	for (int i=1; i<MAX_FD_SIZE; ++i) {
		if (_pollFDs[i].fd == -1) {
			_pollFDs[i].fd = connectFD;
			_pollFDs[i].events = POLLIN;
			_pollFDs[i].revents = 0;
			break ;
		}
	}
}

void	Server::_sendResponse(void) {
	ssize_t	length;
	char	buf[BUFFER_SIZE+ 1];
	vector<string> list;
	vector<string>::iterator iter;

	for (int i = 1; i < MAX_FD_SIZE; ++i) {
		if (_pollFDs[i].revents & POLLHUP) {
			_buf = "";
			close(_pollFDs[i].fd);
			_pollFDs[i].fd = -1;
			_pollFDs->revents = 0;
		} else if (_pollFDs[i].revents & POLLIN) {
			length = recv(_pollFDs[i].fd, buf, BUFFER_SIZE, 0);
			buf[length] = '\0';
			_buf += string(buf);
			if (buf[length - 1] != '\n') 
				break;
			list = split(_buf, "\n");
			_buf = "";
			for(iter = list.begin(); iter != list.end(); ++iter) {
				try{
					_command->execute(*this, _pollFDs[i].fd, *iter);
				} catch(exception& e) {
					cerr << e.what() << endl;
				}
			}
		} 
	}
}

const string &Server::getPassword() const {
	return this->_password;
}

const struct pollfd *Server::getPollFDs() const {
	return this->_pollFDs;
}

const char* Server::InitServerException::what(void) const throw() {
	return "Initiating Server Failed...";
}
