#include "Command.hpp"
#include "Utils.hpp"
#include "header.hpp"
#include "ReplieFactory.hpp"

Command::Command(const string& password): _service(password) {
    _cmds["CAP"] = &Command::_handleCAP;
    _cmds["NICK"] = &Command::_handleNICK;
    _cmds["PRIVMSG"] = &Command::_handlePRIVMSG;
    _cmds["LIST"] = &Command::_handleLIST;
    _cmds["INVITE"] = &Command::_handleINVITE;
    _cmds["KICK"] = &Command::_handleKICK;
    _cmds["PING"] = &Command::_handlePING;
    _cmds["JOIN"] = &Command::_handleJOIN;
    _cmds["QUIT"] = &Command::_handleQUIT;
    _cmds["PART"] = &Command::_handlePART;
    _cmds["NOTICE"] = &Command::_handleNOTICE;
    _cmds["PASS"] = &Command::_handlePASS;
    _cmds["USER"] = &Command::_handleUSER;
    _cmds["MODE"] = &Command::_handleMODE;
}

Command::Command(void): _service("default") {}

void sendMessage(const int& fd, const string& msg) {
    const char *temp = msg.c_str();
    cout << msg;    // TEST
    write(fd, temp, strlen(temp));
}

void Command::execute(int fd, const string &msg) {
    vector<string> words = split(msg, " ");
    iter_cmd cmd = _cmds.find(*words.begin());
    if (cmd != _cmds.end())
        (this->*(cmd->second))(fd, msg);
    else
        throw runtime_error("Command has not existed");
};

void Command::_handleCAP(int fd, const string &msg) {
    (void)fd;
    cout << msg << endl;
}


void Command::_handleLIST(int fd, const string &msg) {
    (void)fd;

    cout << "LIST!!!" << endl;
    cout << msg << endl;
}

// TODO : invite 모드 체크하고 실행하는 부분 추가 필요
void Command::_handleINVITE(int fd, const string &msg) {
    
    vector<string> result = split(msg, " ");
    string  userToInviteName = result.at(1);
    string  channelName = result.at(2);
    User    *user = _service.getUserWithFD(fd);
    User    *userToInvite;
    string  messages;
    Channel *channel;

    try {
        channel = _service.getChannelWithName(channelName);                 // 채널이 존재하는지
        userToInvite = _service.getUserWithName(userToInviteName);                   // 초대할 유저가 존재하는지
    } catch (const ServerService::UserNotExist& e) {
        sendMessage(fd, ERR_NOSUCHNICK_401(userToInviteName));
        return ;
    } catch (const ServerService::ChannelNotExist& e) {
        sendMessage(fd, ERR_NOSUCHNICK_401(channelName));
        return ;
    }
    
    vector<string> userChannel = user->getChannelNames();                           // 명령어를 실행하는 유저가 채널에 존재하는지
    for (vector<string>::iterator it = userChannel.begin(); true ; ++it) {
        if (it == userChannel.end()) {
            messages = channelName + " :You're not on that channel\r\n";
            sendMessage(fd, messages);
            return ;
        }
        else if ((*it) == channelName)
            break ;
    }

    if (channel->hasUser(userToInvite)) {
        sendMessage(fd, ERR_USERONCHANNEL_443(userToInviteName, channelName));
    } else {
        messages = ":" + user->getName() + "!root@127.0.0.1 INVITE " + userToInviteName + " :" + channelName + "\r\n";
        cout << RPL_INVITING_341(channelName, user->getName()) <<  endl;
        sendMessage(fd, RPL_INVITING_341(channelName, user->getName()));
        sendMessage(userToInvite->getFD(), messages);
    }
}

void Command::_handlePING(int fd, const string &msg) {
    (void) msg;
    string response = ":irc.local PONG irc.local :irc.local\r\n";
    sendMessage(fd, response);
}

void Command::_handlePRIVMSG(int fd, const string &msg) {
    vector<string> result = split(msg, " ");
    string  channelName = result.at(1);
    const int idx = msg.find(':');
    string  msgInfo = msg.substr(idx);
    
    // :ace!root@127.0.0.1 PRIVMSG #test :adsfasdf
    Channel* channel = _service.getChannelWithName(channelName);
    vector<User *> users = channel->getUsers();
    for(vector<User *>::iterator user = users.begin(); user != users.end(); ++user) {
        if (fd != (*user)->getFD()){
            string response = ":" + _service.getUserWithFD(fd)->getName() + "!yooh@localhost PRIVMSG " + channel->getName() + " " + msgInfo + "\r\n";
            std::cout << "[response] : " << response << std::endl;
            sendMessage((*user)->getFD(), response);
        }
    }
}

/* 유저가 없으면 throw함 나중에 처리할 필요가 있음 */ 
void Command::_handleQUIT(int fd, const string &msg) {
	string              endMsg      = msg.substr(msg.find(":") + 1, msg.size());
	string              message     = "ERROR :Closing link: (" + string(HOST_NAME) + ") [Quit: " + endMsg + "]\r\n";
    User*               reqUser     = _service.getUserWithFD(fd);
    string              reqUserName = reqUser->getName(); 
    vector<Channel*>    channels    = reqUser->getChannels();
    vector<User*>       tempUsers;
    set<User*>          targetUsers;

  _service.deleteUserWithName(reqUserName);
  sendMessage(fd, message);
  for (vector<Channel*>::iterator iter = channels.begin(); iter != channels.end(); ++iter) {
      tempUsers = (*iter)->getUsers();
      targetUsers.insert(tempUsers.begin(), tempUsers.end());
  }
  for (set<User*>::iterator iter = targetUsers.begin(); iter != targetUsers.end(); ++iter) {
      message = ":" + reqUserName + "!" + string(HOST_NAME) + "QUIT :Quit: " + endMsg + "\r\n";
      sendMessage((*iter)->getFD(), message);
    }
}

void Command::_handlePASS(int fd, const string &msg) {
    vector<string> result = split(msg, " ");
    const string &inputPassword = result.at(1);
    _service.insertPassword(fd, inputPassword);
    cout << "PASS!!!" << endl;
}