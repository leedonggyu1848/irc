#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "header.hpp"
#include "User/User.hpp"

using namespace std;

class User;

enum ChannelMode{
	CHANNEL_O = 0,
	CHANNEL_P,
	CHANNEL_S,
	CHANNEL_I,
	CHANNEL_T,
	CHANNEL_N,
	CHANNEL_M,
	CHANNEL_L,
	CHANNEL_B,
	CHANNEL_V,
	CHANNEL_K
};

class Channel {
private:
	typedef set<User*> users_type;
	typedef users_type::const_iterator users_const_iter;
	typedef users_type::iterator users_iter;

	string 				_name;
	users_type			_users;
	User*				_operator;
	unsigned int		_mode_bit;

	Channel operator= (const Channel& ref);
	Channel (const Channel& ref);

public :
	Channel(void);
	Channel(const string &name);

	const string& getName(void) const;

	bool	hasUser(User* const user) const;
	bool	hasUser(void) const;
	void	addUser(User* const user);
	void	deleteUser(User* const user);

	void	setOperator(User* const user);
	bool	isOperator(User* const user) const;
	User*	getOperator(void) const;

	void	setMode(const ChannelMode& mode);
	void	unsetMode(const ChannelMode& mode);
	bool	isSetMode(const ChannelMode& mode) const;

	vector<User*> getUsers(void) const;
	vector<string> getUserNames(void) const;
	vector<int> getUserFDs(void) const;
};

#endif
