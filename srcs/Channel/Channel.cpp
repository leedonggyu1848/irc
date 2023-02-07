#include "Channel.hpp"

Channel Channel::operator= (const Channel& ref) { (void) ref; return *this; }
Channel::Channel (const Channel& ref) { (void) ref; }

Channel::Channel(void):_mode_bit(0) { }
Channel::Channel(const string &name) : _name(name), _mode_bit(0) { }

const string& Channel::getName(void) const {
	return _name;
}

bool	Channel::hasUser(User* const user) const {
	return _users.find(user) != _users.end();
}

bool	Channel::hasUser(void) const {
	return !_users.empty();
}

void	Channel::addUser(User* const user) {
	if (user == NULL || hasUser(user)) return;
	_users.insert(user);
}

void	Channel::deleteUser(User* const user) {
	users_const_iter iter = _users.find(user);
	if (iter == _users.end()) return;
	_users.erase(iter);
}

void	Channel::setOperator(User* const user) {
	_operator = user;
}

bool	Channel::isOperator(User* const user) const {
	return _operator == user;
}

User*	Channel::getOperator(void) const {
	return _operator;
}

void	Channel::setMode(const ChannelMode& mode) {
	_mode_bit |=  1 << mode;
}

void	Channel::unsetMode(const ChannelMode& mode) {
	_mode_bit &= ~(1 << mode);
}

bool	Channel::isSetMode(const ChannelMode& mode) const {
	return _mode_bit & (1 << mode);
}

vector<User*> Channel::getUsers(void) const {
	vector<User*> ret;
	for(users_const_iter iter = _users.begin(); iter != _users.end(); ++iter)
		ret.push_back(*iter);
	return ret;
}

vector<string> Channel::getUserNames(void) const {
	vector<string> ret;
	for(users_const_iter iter = _users.begin(); iter != _users.end(); ++iter)
		ret.push_back((*iter)->getName());
	return ret;
}

vector<int> Channel::getUserFDs(void) const {
	vector<int> ret;
	for(users_const_iter iter = _users.begin(); iter != _users.end(); ++iter)
		ret.push_back((*iter)->getFD());
	return ret;
}
