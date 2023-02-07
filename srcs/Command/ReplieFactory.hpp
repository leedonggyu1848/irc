#ifndef REPLIEFACTORY_HPP
#define REPLIEFACTORY_HPP

#include "header.hpp"

inline static string join(const vector<string>& emts) {
	string ret;
	for (vector<string>::const_iterator iter = emts.begin(); iter != emts.end(); ++iter) {
		ret += *iter;
		if (iter + 1 != emts.end())
			ret += " ";
	}
	return ret + "\r\n";
}

typedef const string replie_type;

inline replie_type ERR_NOSUCHNICK_401(const string& name) {
	return name + " :No such nick/channel\r\n";
}

inline replie_type ERR_NONICKNAMEGIVEN_431(void) {
	return ":No nickname given\r\n";
}

inline replie_type ERR_ERRONEUSNICKNAME_432(const string& nick) {
	return nick + " :Erroneus nickname\r\n";
}

inline replie_type ERR_NICKNAMEINUSE_433(const string& nick) {
	return nick + " :Nickname is already in use\r\n";
}

inline replie_type ERR_USERNOTINCHANNEL_441(const string& nick, const string& channel) {
	return nick + " " + channel + " :They aren't on that channel\r\n";
}

inline replie_type ERR_NOTONCHANNEL_442(const string& channel) {
	return channel + " :You're not on that channel\r\n";
}

inline replie_type ERR_USERONCHANNEL_443(const string& user, const string& channel) {
	return user + " " + channel + " :is already on channel\r\n";
}

inline replie_type RPL_NAMREPLY_353(const string& channel, const vector<string>& names) {
	string ret = channel + " :";
	ret += join(names) + "\r\n";
	return ret;
}

inline replie_type RPL_ENDOFNAMES_366(const string& channel) {
	return channel + " :End of /NAMES list\r\n";
}

inline replie_type RPL_INVITING_341(const string& channel, const string& nick) {
    return channel + " " + nick + "\r\n";
}

inline replie_type ERR_UNKNOWNMODE_472(void) {
	return " :is unknown mode char to me\r\n";
}

inline replie_type ERR_USERSDONTMATCH_502(void) {
	return " :Can't change mode for other users\r\n";
}

inline replie_type ERR_UMODEUNKNOWNFLAG_501(void) {
	return ":Unknown MODE flag\r\n";
}

// :irc.local 482 yubchoi #t2 :You must be a channel operator
inline replie_type ERR_CHANOPRIVSNEEDED_482(const string& user, const string& channel) {
	return ":irc.local 482 " +  user + " " + channel + " :You must be a channel operator\r\n";
}

#endif 