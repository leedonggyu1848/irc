#include "Server/Command.hpp"
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

static void sendMessage(const int& fd, const string& msg) {
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
        response = string(SERVER_PREFIX) + " 432 " + oldNickname + " " + newNickname +
                 " :Erroneus nickname";
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

void Command::_handleUSER(Server &server, int fd, const string &msg) {
    vector<string> result = split(msg, " ");

    cout << msg << endl;
    cout << "USER!!!" << endl;
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

void Command::_handleJOIN(Server &server, int fd, const string &msg) {
    vector<string>  msgChannel = split(msg, " ");
    const string&   channelName = msgChannel[1];
    Channel*        channel = NULL;
    User*           user = NULL;

    // 모든 채널에서 이름으로 조회해서 없으면
    user = _service.getUserWithFD(fd);
    channel = _service.joinChannelWithUserName(channelName, user->getName());

    vector<User *> list = channel->getUsers();
    vector<User *>::iterator it;

    for(it = list.begin();it != list.end(); ++it) {
        std::cout << "list : " << (*it)->getName() << endl;
    }
        /*
        TODO:
        1. 채널이 invite-only인 경우 초대를 꼭 받아야함.
        2. active-bans에 유저의 /nick/username/hostname이 없어야한다.
        3. 비밀번호가 설정되어 있는 채널이면 올바른 비밀번호를 입력해야한다.
        */
}

void Command::_handlePRIVMSG(Server &server, int fd, const string &msg) {
    vector<string> result = split(msg, " ");
    string  channelName = result.at(1);
    string  msgInfo = result.at(2);
    
    // :ace!root@127.0.0.1 PRIVMSG #test :adsfasdf
    Channel* channel = _service.getChannelWithName(channelName);
    vector<User *> users = channel->getUsers();
    for(vector<User *>::iterator user = users.begin(); user != users.end(); ++user) {
        cout<<"in vector fd : " << fd << "  user->getFD : " << (*user)->getFD() <<  " msg : " << msgInfo << endl;
        if (fd != (*user)->getFD()){
            string response = ":" + _service.getUserWithFD(fd)->getName() + "!yooh@localhost PRIVMSG " + channel->getName() + " " + msgInfo + "\r\n";
            std::cout <<response << std::endl;
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

void Command::_handleNOTICE(Server &server, int fd, const string &msg) {
	bool			isChannel;
	vector<string>	taker = split(msg, " ");
	string 			noticeMsg;
    User*           sender = _service.getUserWithFD(fd);
    const string&   target = taker[1];

    /* 두가지로 나뉘는데 그 안에 두가지가 또 있어 그 두가지 형태가 같아요 그래서 함수로 만들까 하다다가 안했다*/
    /* Notice 받는사람 메시지 */
	isChannel = (taker[1][0] == '#');
    if (isChannel) {
        // 채널이 존재하면, 채널 멤버 받고, 모두에게 뿌리기.
        try {
            Channel* channel = _service.getChannelWithName(target);
            vector<User*> users = channel->getUsers();
            noticeMsg = ":" + sender->getName() + "!" + HOST_NAME + msg;
            for (vector<User*>::iterator iter = users.begin(); iter != users.end(); iter++)
                if ((*iter) != sender) sendMessage((*iter)->getFD(), noticeMsg);
        } catch (const exception& e) {
            noticeMsg = ":irc.local 401 " + sender->getName() + " " + target + " :No such nick/channel";
            sendMessage(fd, noticeMsg);
        }
    } else {
        // 개인에게 보내면 개인에게 문자 보내기
        try {
            User* user = _service.getUserWithName(target);
            noticeMsg = ":" + sender->getName() + "!" + HOST_NAME + msg;
            sendMessage(user->getFD(), noticeMsg);
        } catch (const exception& e) {
            noticeMsg = ":irc.local 401 " + sender->getName() + " " + target + " :No suchnick/channel";
            sendMessage(fd, noticeMsg);
        }
    }
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
