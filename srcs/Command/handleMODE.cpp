#include "Command.hpp"
#include "Utils.hpp"
#include "ReplieFactory.hpp"

void Command::_handleMODE(Server &server, int fd, const string &msg) {
    // user, channel 구분: 시작 '#'
    vector<string> words = split(msg, " ");
    string target = words[1];

    if (target[0] != '#') {	// USER MODE
        User* user = _service.getUserWithFD(fd);
		User* targetUser;
		string nickname = user->getName();
		try {
			targetUser = _service.getUserWithName(target);
		} catch (const exception& e) {
            // :irc.local 401 yubchoi2 noname :No such nick/channel
            sendMessage(fd, ERR_NOSUCHNICK_401(target));
            return ;
		}
		string targetNickname = targetUser->getName();
        // user nickname과 명령어를 보낸 nickname 같은지 확인
        if (targetNickname != nickname) {
            // :Can't change mode for other users / :Can't view modes for other users
			string res = ERR_USERSDONTMATCH_502();
			sendMessage(fd, res);
            return ;
        }

		// "/mode yubchoi" -> "yubchoi :+i / :+"
		if (words.size() == 2) {
			string res = "221 " + nickname + " :+";
			if (user->isSetMode(USER_I)) {
				res += "i";
			} else if (user->isSetMode(USER_S)) {
				res += "s";
			} else if (user->isSetMode(USER_W)) {
				res += "w";
			} else if (user->isSetMode(USER_O)) {
				res += "o";
			}
			res += "\r\n";
			sendMessage(fd, res);
			return ;
		}

        // mode 검사
		string newMode = words[2];
		string addMode = "+";
		string removeMode = "-";
		string res = "";
        for (int i = 0; i < newMode.length(); i++) {
            if (newMode[i] == 'i') {
                if (i > 0 && newMode[i-1] == '-' && user->isSetMode(USER_I)) {
                    user->unsetMode(USER_I);
					removeMode += "i";
                } else if (user->isSetMode(USER_I) == 0 && (i == 0 || newMode[i-1] == '+')){                    
                    user->setMode(USER_I);
					addMode += "i";
                }
            } else if (newMode[i] == 'w') {
                if (i && newMode[i-1] == '-' && user->isSetMode(USER_W)) {
					user->unsetMode(USER_W);
					removeMode += "w";
                } else if (user->isSetMode(USER_W) == 0 && (i == 0 || newMode[i-1] == '+')) {
					user->setMode(USER_W);
					addMode += "w";
                }
            } else if (newMode[i] == 'o') {
                if (i && newMode[i-1] == '-' && user->isSetMode(USER_O)) {
					user->unsetMode(USER_O);
					removeMode += "o";
                } else if (user->isSetMode(USER_O) == 0 && (i == 0 || newMode[i-1] == '+')) {
					user->setMode(USER_O);
					addMode += "o";
                }
            } else if (newMode[i] == 's') {
                if (i && newMode[i-1] == '-' && user->isSetMode(USER_S)) {
					user->unsetMode(USER_S);
					removeMode += "s";
                } else if (user->isSetMode(USER_S) == 0 && (i == 0 || newMode[i-1] == '+')) {
					user->setMode(USER_S);
					addMode += "s";
                }
            } else if (newMode[i] == '+' || newMode[i] == '-') {
				continue;
			} else { // yubchoi j :is unknown mode char to me
				// replieFactory
				res += newMode[i] + ERR_UNKNOWNMODE_472();
			}
        }
        
		if (addMode.length() > 1 && removeMode.length() > 1) {
			res += "MODE " + nickname + " " + addMode + removeMode + "\r\n";
		} else if (addMode.length() > 1) {
			res += "MODE " + nickname + " " + addMode + "\r\n";
		} else if (removeMode.length() > 1) {
			res += "MODE " + nickname + " " + removeMode + "\r\n";
		}
		sendMessage(fd, res);
		return;
    } else {
        // channel MODE: _handleChannelMode()

    }
}