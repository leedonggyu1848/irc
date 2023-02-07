#ifndef SERVER_HPP
#define SERVER_HPP

#include "header.hpp"
#include "Command/Command.hpp"
#include "Server/ServerService.hpp"
#include <sys/socket.h>
#include <sys/poll.h>
#include <string>

class Command;

#define SERVER_PREFIX "irc.local"

class Server {
typedef void (Server::*pfunc) (int, string);
private :
	int								_port;
	string							_password;
	int								_socket;
	struct pollfd					_pollFDs[MAX_FD_SIZE];
	string							_bufFDs[MAX_FD_SIZE];
	Command*						_command;

	void	_acceptConnections(void);
	void	_sendResponse(void);

public :
	Server(const string& port, const string& password);
	~Server();
	const string &getPassword() const;
	const struct pollfd	*getPollFDs() const;

	/*
	** @brief server 소켓 liten까지 실행
	** @throw 도중에 실패하면 _socket close하고 InitServerException 던짐
	*/
	void 	prepare(void);

	/*
	** @brief server poll 실행 무한 반복
	*/
	void	start(void);

	/*
	** @brief UserManager 멤버 클래스  반반환환
	*/

	class InitServerException : public exception {
	public :
		const char *what(void) const throw();
	};
};

#endif
