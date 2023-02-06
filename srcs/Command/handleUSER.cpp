#include "Command.hpp"
#include "Utils.hpp"

void Command::_handleUSER(Server &server, int fd, const string &msg) {
    vector<string> result = split(msg, " ");

    cout << msg << endl;
    cout << "USER!!!" << endl;
}
