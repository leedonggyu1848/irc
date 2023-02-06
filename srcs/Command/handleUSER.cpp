#include "Command.hpp"
#include "Utils.hpp"

void Command::_handleUSER(Server &server, int fd, const string &msg) {
    vector<string> result = split(msg, " ");
    User* user = _service.getUserWithFD(fd);

    user->setId(result.at(1));
    user->setHostname(result.at(3));
    cout << msg << endl;
    cout << "USER!!!" << endl;
}
