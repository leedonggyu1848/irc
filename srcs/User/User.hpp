#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <sys/poll.h>
#include <vector>
#include "Channel/Channel.hpp"
#include <map>

using namespace std;

enum UserMode {
	USER_I = 0,
	USER_S,
	USER_W,
	USER_O
};

#define HOST_NAME "root@127.0.0.1"

class Channel;

class User {
private :
	typedef set<Channel*> channels_type;
	typedef channels_type::const_iterator channels_const_iter;
	typedef channels_type::iterator channels_iter;
	string 					_name;
	string					_hostname;
	string					_id;
	int						_fd;
	unsigned int			_mode_bit;
	channels_type			_channels;

	User operator= (const User& ref);
	User (const User& ref);

public:
	User(void);
	User(const string& name, const int& fd);
	~User();

	const string&	getName(void) const;
	int				getFD(void) const;
	const string&	getHostname(void) const;
	const string&	getId(void) const;
	void			setName(const string& name);
	void			setHostname(const string& hostname);
	void			setId(const string& id);

	bool				hasChannel(Channel* const channel) const;
	void				joinChannel(Channel* channel);
	void				partChannel(Channel* const channel);
	vector<string>		getChannelNames(void) const;
	vector<Channel*>	getChannels(void) const;

	void	setMode(const UserMode& mode);
	void	unsetMode(const UserMode& mode);
	bool	isSetMode(const UserMode& mode) const;
};

#endif
