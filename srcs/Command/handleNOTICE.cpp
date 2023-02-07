#include "Command.hpp"
#include "Utils.hpp"
#include "ReplieFactory.hpp"

void Command::_handleNOTICE(int fd, const string &msg) {
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
            noticeMsg = ":" + sender->getName() + "!" + HOST_NAME + " " +msg + "\r\n";
            for (vector<User*>::iterator iter = users.begin(); iter != users.end(); iter++)
                if ((*iter) != sender) sendMessage((*iter)->getFD(), noticeMsg);
        } catch (const exception& e) {
            noticeMsg = ":irc.local 401 " + sender->getName() + " " + ERR_NOSUCHNICK_401(target);
            sendMessage(fd, noticeMsg);
        }
    } else {
        // 개인에게 보내면 개인에게 문자 보내기
        try {
            User* user = _service.getUserWithName(target);
            noticeMsg = ":" + sender->getName() + "!" + HOST_NAME + " " + msg + "\r\n";
            sendMessage(user->getFD(), noticeMsg);
        } catch (const exception& e) {
            noticeMsg = ":irc.local 401 " + sender->getName() + " " + ERR_NOSUCHNICK_401(target);
            sendMessage(fd, noticeMsg);
        }
    }
}
