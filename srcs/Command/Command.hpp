#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "header.hpp"
#include "Server/Server.hpp"
#include "Server/ServerService.hpp"
#include "poll.h"

#define RES_SELF 0
#define RES_OTHER 1

class Server;

class Command {
private:
	typedef void(Command::*_pfunc)(int, const string&);
	typedef map<string, _pfunc>::iterator iter_cmd;

	ServerService _service;
	map<string, _pfunc>	_cmds;

	void	_handleNICK(int, const string&);
	void	_handleCAP(int, const string&);
	void	_handlePRIVMSG(int, const string&);
	void	_handleINVITE(int, const string&);
	void	_handleKICK(int, const string&);
	void	_handlePING(int, const string&);
	void	_handleJOIN(int, const string&);
	void	_handleQUIT(int, const string&);
	void	_handlePART(int, const string&);
	void	_handleNOTICE(int, const string&);
	void	_handlePASS(int, const string&);
	void	_handleUSER(int, const string&);
	void	_handleMODE(int, const string&);
	void	_handleLIST(int, const string&);

	Command(void);
public:
	Command(const string& password);
	void execute(int fd, const string& msg);
};

void sendMessage(const int& fd, const string& msg);

#endif
