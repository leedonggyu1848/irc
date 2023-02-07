#include "Command.hpp"
#include "Utils.hpp"

void Command::_handleUSER(int fd, const string &msg) {
    vector<string> result = split(msg, " ");
    User* user = _service.getUserWithFD(fd);

    user->setId(result.at(1));
    user->setHostname(result.at(3));
    cout << msg << endl;
    cout << "USER!!!" << endl;

    string response;
    string inputPassword = _service.getMappedPassword(fd);
    if (_service.getPassword() != inputPassword) {
        response = ":irc.local 464 " + user->getName() +  " :Password Incorrect\r\n";
        sendMessage(fd, response);
        cout << "ERROR :Closing Link: localhost (Bad Password)\r\n";
        _service.deleteUser(user);
    }
}
