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
	typedef void(Command::*_pfunc)(Server& server, int, const string&);
	typedef map<string, _pfunc>::iterator iter_cmd;

	ServerService _service;
	map<string, _pfunc>	_cmds;

	void	_handleNICK(Server&, int, const string&);
	void	_handleCAP(Server&, int, const string&);
	void	_handlePRIVMSG(Server&, int, const string&);
	void	_handleINVITE(Server&, int, const string&);
	void	_handleKICK(Server&, int, const string&);
	void	_handlePING(Server&, int, const string&);
	void	_handleJOIN(Server&, int, const string&);
	void	_handleQUIT(Server&, int, const string&);
	void	_handlePART(Server&, int, const string&);
	void	_handleNOTICE(Server&, int, const string&);
	void	_handlePASS(Server&, int, const string&);
	void	_handleUSER(Server&, int, const string&);
	void	_handleMODE(Server&, int, const string&);

	void	_handleTOPIC(Server&, int, const string&);
	void	_handleOPER(Server&, int, const string&);
	void	_handleLIST(Server&, int, const string&);
public:
	Command();
	void execute(Server& server, int fd, const string& msg);
};

void sendMessage(const int& fd, const string& msg);

#endif
