#include "Command.hpp"
#include "Utils.hpp"
#include "ReplieFactory.hpp"
#include "User/UserManager.hpp"

void Command::_handleKICK(int fd, const string &msg) {
	vector<string> words = split(msg, " ");
	User *user = _service.getUserWithFD(fd);
	string channelName = words[1];
	string targetName = words[2];
	Channel *channel = _service.getChannelWithName(channelName);
	string comment;

	// channel 검사
	if (channel == NULL) {
		sendMessage(fd, ERR_NOSUCHNICK_401(channelName));
		return ;
	}

	if (words.size() > 3)
		comment = words[3];

	// channel의 operator인지 확인
	if (channel->isOperator(user) == false) {
		// :irc.local 482 yubchoi #t2 :You must be a channel operator
		sendMessage(fd, ERR_CHANOPRIVSNEEDED_482(user->getName(), channelName));
		return ;
	}

	// user 검사
	// 1. user가 서버에 존재하는지 확인
	if (_service.getUserWithName(targetName) == NULL) {
		sendMessage(fd, ERR_NOSUCHNICK_401(targetName));
		return ;
	}
	// 2. user가 채널에 존재하는지 확인
	User *targetUser = _service.getUserWithName(targetName);
	if (channel->hasUser(targetUser) == false) {
		sendMessage(fd, ERR_USERNOTINCHANNEL_441(targetName, channelName));
		return ;
	}

	vector<User*> users = channel->getUsers();
	// :yubin!root@127.0.0.1 KICK #t2 yubchoi :hi
	string response = ":" + user->getName() + "!" + user->getId() + "@" + user->getHostname() + " KICK " + channelName + " " + targetName + " " + comment + "\r\n";

	for (vector<User*>::iterator iter = users.begin(); iter != users.end(); ++iter) {
		sendMessage((*iter)->getFD(), response);
	}
	_service.partChannelWithUserName(channelName, targetName);
}