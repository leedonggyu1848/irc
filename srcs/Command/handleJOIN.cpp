#include "Command.hpp"
#include "Utils.hpp"

void Command::_handleJOIN(Server &server, int fd, const string &msg) {
    vector<string>  msgChannel = split(msg, " ");
    const string&   channelName = msgChannel[1];
    Channel*        channel = NULL;
    User*           user = NULL;

    // 모든 채널에서 이름으로 조회해서 없으면
    user = _service.getUserWithFD(fd);
    channel = _service.joinChannelWithUserName(channelName, user->getName());

    string joinMessage = ":" + user->getName() + "!yooh@localhost JOIN :" + channel->getName() + "\r\n";
    string nameReply   = ":irc.local 353 " + user->getName() + " = " + channel->getName() + " :@";
    
    vector<User *> list = channel->getUsers();
    vector<User *>::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        nameReply += (*it)->getName() + " ";
    }
    nameReply += "\r\n";
    string endOfNames   = ":irc.local 366 " + user->getName() + " " + channel->getName() + " :End of /NAMES list.\r\n";

    sendMessage(fd, joinMessage);
    sendMessage(fd, nameReply);
    sendMessage(fd, endOfNames);

    string response = ":" + user->getName() + "!yooh@localhost JOIN :" + channelName + "\r\n";

    for(it = list.begin();it != list.end(); ++it) {
        if ((*it)->getFD() != fd) {
            cout << "[JOIN RESPONSE] " << _service.getUserWithFD((*it)->getFD())->getName() << " >> " << response << endl;
            sendMessage((*it)->getFD(), response);
        }
    }

        /*
        TODO:
        1. 채널이 invite-only인 경우 초대를 꼭 받아야함.
        2. active-bans에 유저의 /nick/username/hostname이 없어야한다.
        3. 비밀번호가 설정되어 있는 채널이면 올바른 비밀번호를 입력해야한다.
        */
}