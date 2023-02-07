#include "header.hpp"
#include "UserManager.hpp"

UserManager::~UserManager() {
	for (users_iter iter = _users.begin(); iter != _users.end(); ++iter)
		delete iter->second;
}

User*	UserManager::getUserWithFD(const int& fd) const {
	users_const_iter temp = _users.find(fd);
	if (temp == _users.end()) return NULL;
	return temp->second;
}

User*	UserManager::getUserWithName(const string& name) const {
	for(users_const_iter iter = _users.begin(); iter != _users.end(); ++iter)
		if (iter->second->getName() == name) return iter->second;
	return NULL;
}

void	UserManager::deleteChannelFromUser(Channel* const channel, User* const user) {
	user->partChannel(channel);
}

User*	UserManager::addUser(const string& name, const int& fd) {
	if (getUserWithName(name) || getUserWithFD(fd)) return NULL;
	_users[fd] = new User(name, fd);
	return _users[fd];
}

void	UserManager::deleteUser(const string& name) {
	for (users_iter iter = _users.begin(); iter != _users.end(); ++iter) {
		if (iter->second->getName() == name) {
			delete iter->second;
			_users.erase(iter);
			return;
		}
	}
}

void	UserManager::deleteUser(const int& fd) {
	users_iter temp = _users.find(fd);
	if (temp == _users.end()) return;
	delete temp->second;
	_users.erase(temp);
}

void	UserManager::deleteUser(User* const user) {
	deleteUser(user->getFD());
}
