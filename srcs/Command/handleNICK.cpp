#include "Command.hpp"
#include "Utils.hpp"

void Command::_handleNICK(Server &server, int fd, const string &msg) {
    vector<string> result = split(msg, " ");
    // TODO: 유저가 없을 경우 try catch를 써야하나?
    string oldNickname = "*";
    string newNickname;
    string response;
    // "/nick"
    if (result.size() <= 1) {
        // ERR_NONICKNAMEGIVEN, 431,  :No nickname given
        response = string(SERVER_PREFIX) + " 431 " + oldNickname + " :No nickname given";
        cout << response << endl;
        sendMessage(fd, response);
        return;
    }
    newNickname = result.at(1);
    // "/nick morethan9letters"
    if (newNickname.length() > 9) {
        // :irc.local 432 <nickname> <nickname> :Erroneus nickname
        response = string(SERVER_PREFIX) + " 432 " + oldNickname + " " + newNickname + " :Erroneus nickname";
		cout << response << endl;
        sendMessage(fd, response);
        return;
    }
    try {
        _service.addUser(newNickname, fd);
    } catch (const exception &e) {
        response = ":" + string(SERVER_PREFIX) + " 433 " + oldNickname + " " + newNickname + " :Nickname is already in use.\r\n";
        cout << response << endl;
        sendMessage(fd, response);
        cerr << e.what() << endl;
    }

    string userName = _service.getUserWithFD(fd)->getName();
    response = ":irc.local 001 " + userName +
                    " :Welcome to the Localnet IRC Network " + userName +
                    "!root@127.0.0.1\r\n";
    sendMessage(fd, response);    //server._service.       // "/nick newnickname"
}
