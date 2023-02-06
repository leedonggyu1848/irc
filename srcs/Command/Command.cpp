#include "Command.hpp"
#include "Utils.hpp"
#include "header.hpp"

Command::Command() {
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

void sendMessage(const int& fd, const string& msg) {
    const char *temp = msg.c_str();
    write(fd, temp, strlen(temp));
}

void Command::execute(Server &server, int fd, const string &msg) {
    vector<string> words = split(msg, " ");
    iter_cmd cmd = _cmds.find(*words.begin());
    if (cmd != _cmds.end())
        (this->*(cmd->second))(server, fd, msg);
    else
        throw runtime_error("Command has not existed");
};

void Command::_handleCAP(Server &server, int fd, const string &msg) {
    (void)fd;
    (void)server;

    cout << msg << endl;
}


void Command::_handleLIST(Server &server, int fd, const string &msg) {
    (void)fd;
    (void)server;

    cout << "LIST!!!" << endl;
    cout << msg << endl;
}

void Command::_handleINVITE(Server &server, int fd, const string &msg) {
    (void)fd;
    (void)server;

    cout << "INVITE!!!" << endl;
    cout << msg << endl;
}

void Command::_handleKICK(Server &server, int fd, const string &msg) {
    (void)fd;
    (void)server;

    cout << "KICK!!!" << endl;
    cout << msg << endl;
}

void Command::_handlePING(Server &server, int fd, const string &msg) {
    (void)server;

    cout << msg << endl;
    const char *res = ":irc.local PONG irc.local :irc.local";
    write(fd, res, strlen(res));
}

void Command::_handlePRIVMSG(Server &server, int fd, const string &msg) {
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
void Command::_handleQUIT(Server &server, int fd, const string &msg) {
	string              endMsg      = msg.substr(msg.find(":") + 1, msg.size());
	string              message     = "ERROR :Closing link: (" + string(HOST_NAME) + ") [Quit: " + endMsg + "]";
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
      message = ":" + reqUserName + "!" + string(HOST_NAME) + "QUIT :Quit: " + endMsg;
      sendMessage((*iter)->getFD(), message);
    }
}

void Command::_handlePART(Server &server, int fd, const string &msg) {
  (void)fd;
  (void)server;
  cout << "PART!!!" << endl;
}

void Command::_handlePASS(Server &server, int fd, const string &msg) {
    vector<string> result = split(msg, " ");
    const string &inputPassword = result.at(1);

    if (server.getPassword() != inputPassword) {
        string res("Password Wrong\r\n");
        cout<<res<<endl;
        //_sendMessage(fd, RES_SELF, res, server);
        return;
    }
    cout << "PASS!!!" << endl;
}

void Command::_handleMODE(Server &server, int fd, const string &msg){

}

//void Command::_handleUSER(Server &server, int fd, const string &msg) {
//    cout << msg << endl;
//    vector<string> list;
//    // const string &userName =
//    server.getUserManager().getUserWithFD(fd).getName();
//    list.push_back((":irc.local 001 " + userName +
//                    " :Welcome to the Localnet IRC Network " + userName +
//                    "!root@127.0.0.1\n"));
//    vector<string>::iterator it;
// if (targetName[0] != '#') {
//        // user MODE: _handleUserMode()
//        user = _service.getUserWithFD(fd);
//        try {
//            target = _service.getUserWithName(targetName);
//        } catch (const exception& e) {
//            string res = SERVER_PREFIX + string(" 401 ") + user->getName() + " " + targetName + " :No such nick/channel\n";
//            sendMessage(fd, res);
//            return ;
//        }
        
//        // user nickname과 명령어를 보낸 nickname 같은지 확인
//        if (user->getName() != targetName) {
//            // :irc.local 502 nickname_ :Can't change mode for other users
//            string res(SERVER_PREFIX + string(" 502 ") + user->getName() + " :Can't change mode for other users\n");
//            sendMessage(fd, res);
//            return ;
//        }

//        // mode 검사
//        for (int i = 0; i < targetMode.length(); i++) {
//            switch (targetMode[i]) {
//            case 'i':
//                -iows -i-o+s  ios
//                break;
//            case 'w':
//                break;
//            case 'o':
//                break;
//            case 's':
//                break;
//            case '-':
//            case '+':
//                break;
//            default:
//            }
//            if (words[2][i] != 'i' && words[2][i] != 'w' && words[2][i] != 'o' && words[2][i] != 's' && words[2][i] != '+' && words[2][i] != '-') {
//                // :irc.local 501 nickname_ :Unknown MODE flag
//                string res(SERVER_PREFIX + string(" 501 ") + nickname + string(" :Unknown MODE flag\n"));
//                _sendMessage(fd, RES_SELF, res, server);
//                continue;
//            }
//            // find i
//            else if (words[2][i] == 'i') {
//                if (i && words[2][i-1] == '-' && user.mode[USER_MODE_I]) {
//                    user.setInvisible(false);
//                    // :nickname_!root@127.0.0.1 MODE nickname_ -i
//                    _sendMessage(fd, RES_SELF, string(user.), server);
//                } else if (user.mode[USER_MODE_I] == 0) {                    
//                    user.setInvisible(true);
//                }
//            } else if (words[2][i] == 'w') {
//                if (i && words[2][i-1] == '-' && user.mode[USER_MODE_W]) {
//                    user.setReceiveWallops(false);
//                } else if (user.mode[USER_MODE_W] == 0) {
//                    user.setReceiveWallops(true);
//                }
//            } else if (words[2][i] == 'o') {
//                if (i && words[2][i-1] == '-' && user.mode[USER_MODE_O]) {
//                    user.setOperator(false);
//                } else if (user.mode[USER_MODE_O] == 0) {
//                    user.setOperator(true);
//                }
//            } else if (words[2][i] == 's') {
//                if (i && words[2][i-1] == '-' && user.mode[USER_MODE_S]) {
//                    user.setInvisible(false);
//                } else if (user.mode[USER_MODE_S] == 0) {
//                    user.setInvisible(true);
//                }
//            }
//        }
        
        
//    } else {
//        // channel MODE: _handleChannelMode()

//    }
//    // user MODE

//    // 
//}
