#include "Server.hpp"
#include <sys/_types/_ssize_t.h>

void	Server::setSocket() {
	_listenFD = socket(PF_INET, SOCK_STREAM, 0);
	// 기존에 열려 있던 소켓 덮어쓰기
	int option = 1;
	setsockopt(_listenFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	memset(&_listenSocket, 0, sizeof(_listenSocket));

	_listenSocket.sin_family = AF_INET;
	_listenSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	_listenSocket.sin_port = htons(PORT);

	if (bind(_listenFD, (struct sockaddr *) &_listenSocket, sizeof(_listenSocket)) == -1
		|| listen(_listenFD, LISTEN_QUEUE_SIZE) == -1)
		throw InitServerException();

	_pollFDs[0].fd = _listenFD;
	_pollFDs[0].events = POLLIN;
	_pollFDs[0].revents = 0;

	for (int i=1; i<MAX_FD_SIZE;i++) {
		_pollFDs[i].fd = -1;
	}
}

struct pollfd	*Server::getPollFDs() {
	return _pollFDs;
}

void			Server::addPollFD() {
	if (_pollFDs[0].revents == POLLIN) {
		int	connectFD = accept(_listenFD, (struct sockaddr *)&_connectSocket, &_addrSize);
		for (int i=1; i<MAX_FD_SIZE; ++i) {
			if (_pollFDs[i].fd == -1) {
				_pollFDs[i].fd = connectFD;
				_pollFDs[i].events = POLLIN;
				_pollFDs[i].revents = 0;
				break ;
			}
		}
	}
}

void		Server::sendResponse() {
	ssize_t	length;
	char	buf[BUFFER_SIZE+ 1];

	for (int i = 1; i < MAX_FD_SIZE; ++i) {
		switch (_pollFDs[i].revents) {
			case 0 : //no events
				break ;
			case POLLIN : //data is ready
				length = recv(_pollFDs[i].fd, buf, BUFFER_SIZE, 0);
				buf[length] = '\0';
				printf("%lu bytes read\n", length);
				printf("[%s]", buf);
				for (int j = 1; j < MAX_FD_SIZE; ++j) {
					if (_pollFDs[j].fd != -1 && _pollFDs[i].fd != _pollFDs[j].fd) {
						write(_pollFDs[j].fd, buf, strlen(buf));
					}
				}
				break ;
			default :
				close(_pollFDs[i].fd);
				_pollFDs[i].fd = -1;
				_pollFDs->revents = 0;
		}
	}
}

void			Server::closeListenFD() {
	close(_listenFD);
}

const char *Server::InitServerException::what() const throw(){
	return "Initiating Server Failed...";
}