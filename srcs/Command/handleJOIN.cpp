#include "Command.hpp"
#include "Utils.hpp"
#include "ReplieFactory.hpp"

void Command::_handleJOIN(int fd, const string &msg) {
    vector<string>  msgChannel = split(msg, " ");
    vector<string> channels = split(msgChannel[1], ",");

    for (vector<string>::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
        const string&   channelName = *ch;
        // 모든 채널에서 이름으로 조회해서 없으면
        User* user = _service.getUserWithFD(fd);
        Channel* channel = _service.joinChannelWithUserName(channelName, user->getName());
        vector<string> userNames = channel->getUserNames();
        replace(userNames.begin(), userNames.end(), channel->getOperator()->getName(), "@" + channel->getOperator()->getName());
        sendMessage(fd, ":" + user->getName() + "!" + user->getId() + "@" + user->getHostname() + " JOIN :" + channel->getName() + "\r\n");
        sendMessage(fd, ":irc.local 353 " + user->getName() + " = " + RPL_NAMREPLY_353(channel->getName(), userNames));
        sendMessage(fd, ":irc.local 366 " + user->getName() + " " + RPL_ENDOFNAMES_366(channel->getName()));

        string response = ":" + user->getName() + "!" + user->getId() + "@" + user->getHostname() + " JOIN :" + channelName + "\r\n";
        vector<User*> users = channel->getUsers();
        for(vector<User*>::iterator iter = users.begin(); iter != users.end(); ++iter) {
            if ((*iter)->getFD() != fd) {
                cout << "[JOIN RESPONSE] " << (*iter)->getName() << " >> " << response << endl;
                sendMessage((*iter)->getFD(), response);
            }
        }
    }
}
