#include "Command.hpp"
#include "Utils.hpp"
#include "ReplieFactory.hpp"

void Command::_handlePART(int fd, const string &msg) {
	vector<string>  msgChannel = split(msg, " ");
	vector<string>  channels = split(msgChannel[1], ",");

	for (vector<string>::iterator it = channels.begin(); it != channels.end(); ++it) {
		Channel*        channel = NULL;
		User*           user = _service.getUserWithFD(fd);
		const string&	channelName = *it;

		cout << "channelName: " + channelName << endl;	// TEST
		// 해당 채널이 존재하지 않는 경우
		try {
			channel = _service.getChannelWithName(channelName);
		} catch (const std::exception& e) {
			cout << "RESPONSE: " + ERR_NOSUCHNICK_401(*it) << endl;	// TEST
			sendMessage(fd, ERR_NOSUCHNICK_401(*it));
			continue;
		}

		// 해당 채널에 유저가 존재하지 않음
		if (!channel->hasUser(user)) {
			cout << "RESPONSE: " + ERR_NOTONCHANNEL_442(channelName) << endl;	// TEST
			sendMessage(fd, ERR_NOTONCHANNEL_442(channelName));
			continue;
		}

		string response = ":" + user->getName() + "!" + user->getId() + "@" + user->getHostname() + " PART " + channelName + "\r\n";
		// cout << response;	// TEST
		// 채널 내 모든 유저에게 메시지 전송
		vector<User*> users = channel->getUsers();
		for (vector<User*>::iterator iter = users.begin(); iter != users.end(); ++iter) {
				sendMessage((*iter)->getFD(), response);
		}
		_service.partChannelWithUserName(channelName, user->getName());
	}
}