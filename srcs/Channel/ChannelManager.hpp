#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include "header.hpp"
#include "Channel/Channel.hpp"

class ChannelManager {
private:
	map<string, Channel*>		_channels;
public:
	typedef map<string, Channel*>::const_iterator channel_const_iter;
	typedef map<string, Channel*>::iterator channel_iter;
	~ChannelManager(void);

	/************************
	read
	************************/
	Channel*	getChannelWithName(const string& name) const;

	/************************
	create
	************************/
	Channel*	addChannel(const string& name);
	
	/************************
	update
	************************/
	void		updateUserFromChannel(User* const user, Channel* const channel);

	/************************
	delete
	************************/
	void		deleteChannel(const string& name);
	void		partUserFromChannels(User* user, const vector<string>& channels);

};

#endif
