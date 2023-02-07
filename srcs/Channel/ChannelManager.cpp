#include "ChannelManager.hpp"

ChannelManager::~ChannelManager(void) {
	for (channel_iter iter = _channels.begin(); iter != _channels.end(); ++iter)
		delete iter->second;
}

Channel*	ChannelManager::getChannelWithName(const string& name) const {
	channel_const_iter temp = _channels.find(name);
	if (temp == _channels.end()) return NULL;
	return temp->second;
}

Channel*		ChannelManager::addChannel(const string& name) {
	if (getChannelWithName(name)) return NULL;
	_channels[name] = new Channel(name);
	return _channels[name];
}

void		ChannelManager::deleteChannel(const string& name) {
	channel_iter temp = _channels.find(name);
	if (temp == _channels.end()) return;
	delete temp->second;
	_channels.erase(temp);
}

void		ChannelManager::deleteChannel(Channel* const channel) {
	deleteChannel(channel->getName());
}

void		ChannelManager::partUserFromChannels(User* const user, const vector<string>& channels) {
	for (vector<string>::const_iterator iter = channels.begin(); iter != channels.end(); ++iter)
		getChannelWithName(*iter)->deleteUser(user);
}

void		ChannelManager::partUserFromChannel(User* const user, Channel* const channel) {
	channel->deleteUser(user);
	if (!channel->hasUser()) {
		deleteChannel(channel);
	}
}
