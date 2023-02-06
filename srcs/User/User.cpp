#include "User/User.hpp"

User User::operator= (const User& ref) { return *this; }
User::User (const User& ref) {}
User::User(void): _name("*"), _fd(-1), _mode_bit(0) {}
User::User(const string& name, const int& fd): _name(name), _fd(fd), _mode_bit(0) { }

const string& User::getName(void) const { return _name; }
int	User::getFD(void) const { return _fd; }
const string& User::getHostname(void) const { return _hostname; }
void User::setName(const string& name) { _name = name; }
void User::setHostname(const string& hostname) { _hostname = hostname; }

bool User::hasChannel(Channel* const channel) const {
	return _channels.find(channel) != _channels.end();
}

void User::joinChannel(Channel* channel) {
	if (channel == NULL || hasChannel(channel)) return ;
	_channels.insert(channel);
}

void User::partChannel(Channel* const channel) {
	channels_const_iter iter = _channels.find(channel);
	if (iter == _channels.end()) return;
	_channels.erase(iter);
}

vector<Channel*>	User::getChannels(void) const {
	vector<Channel*> ret;
	for (channels_const_iter iter = _channels.begin(); iter != _channels.end(); ++iter)
		ret.push_back(*iter);
	return ret;
}

vector<string>	User::getChannelNames(void) const {
	vector<string> ret;
	for (channels_const_iter iter = _channels.begin(); iter != _channels.end(); ++iter)
		ret.push_back((*iter)->getName());
	return ret;
}

void	User::setMode(const UserMode& mode) {
	_mode_bit &=  1 << mode;
}

void	User::unsetMode(const UserMode& mode) {
	_mode_bit &= ~(1 << mode);
}

bool	User::isSetMode(const UserMode& mode) const {
	return _mode_bit & (1 << mode);
}