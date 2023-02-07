#ifndef SERVER_SERVICE
#define SERVER_SERVICE

#include "header.hpp"
#include "Channel/ChannelManager.hpp"
#include "User/UserManager.hpp"
#include <map>

using namespace std;

/*
** @brief 가능하면 모든 기능은 해당 클래스를 통해서 사용하는 것을 권장함
*/
class ServerService {
private:
	ChannelManager 		_channelManager;
	UserManager     	_userManager;
	map<int, string>	_passwordMap;

	ServerService operator= (const ServerService& ref);
	ServerService(const ServerService& ref);

public:
	ServerService(void);
	~ServerService(void);

	/************************
	read
	************************/

	/*
	* @param userName: 찾고 싶은 유저 이름
	* @return User 객체의 포인터 값 (delete하면 안됨)
	* @throw 찾지 못하면 ServerService::UserNotExist를 던짐
	*/
	User* getUserWithName(const string& userName) const;
	
	/*
	* @param fd: 찾고 싶은 유저의 fd
	* @return User 객체의 포인터 값 (delete하면 안됨)
	* @throw 찾지 못하면 ServerService::UserNotExist를 던짐
	*/
	User* getUserWithFD(const int& fd) const;

	/*
	* @param channelName: 찾고 싶은 채널의 이름
	* @return Channel 객체의 포인터 값 (delete하면 안됨)
	* @throw 찾지 못하면 ServerService::ChannelNotExist를 던짐
	*/
	Channel* getChannelWithName(const string& channelName) const;

	/*
	* @param channelName: 검색할 채널 이름
	* @throw 만약 없는 채널이라면 ServerService::ChannelNotExist를 던짐
	* @return 해당 채널에 들어있는 User객체의 포인터값들의 vector를 리턴함
	*/
	vector<User*> getUsersInChannel(const string& channelName) const;

	/*
	* @param userName: 검색할 유저의 이름
	* @throw 만약 없는 유저라면 ServerService::UserNotExist를 던짐
	* @return 해당 유저가 들어가 있는 Channel객체의 포인터값들의 vector를 리턴함
	*/
	vector<Channel*> getChannelsFromUser(const string& userName) const;

	/************************
	create
	************************/

	/*
	* @param userName: 추가할 유저의 이름
	* @param fd: 추가할 유저의 f면
	* @brief 유저를 추가하고 성공, 실패 여부를 리턴
	* @throw 만약 유저가 있거나, fd가 존재하면 ServerService::UserAlreadyExist던짐
	* @return 추가한 User의 포인터값
	*/
	User* addUser(const string& userName, const int& fd);

	/*
	* @param channelName: 들어갈 채널 이름
	* @param userName: 들어갈 유저 이름
	* @throw 만약 없는 유저라면, ServerService::UserNotExist를 던짐
	* @brief 만약 채널이 존재하지 않는다면 새로운 채널을 만듬
	* @return join한 Channel의 포인터값
	*/
	Channel* joinChannelWithUserName(const string& channelName, const string& userName);

	/************************
	delete
	************************/

	/*
	* @param userName: 지우고 싶은 유저의 이름
	* @throw 만약 없는 유저를 지운다면,ServerService::UserNotExist를 던짐
	*/
	void deleteUserWithName(const string& userName);
	/*
	* @param userName: 지우고 싶은 유저의 fd
	* @throw 만약 없는 유저를 지운다면,ServerService::UserNotExist를 던짐
	*/
	void deleteUserWithFD(const int& fd);
	/*
	* @param userName: 지우고 싶은 유저의 pointer
	* @brief 만약 user가 NULL이면 아무것도 하지 않음. (pointer값이 잘못됬다는 것은 고려하지 않음)
	*/
	void deleteUser(User* user);

	/*
	* @param channelName: 나갈 채널 이름
	* @param userName: 나갈 유저 이름
	* @throw 만약 없는 채널이라면 ServerService::ChannelNotExist를 던짐
	* @throw 만약 없는 유저라면 ServerService::UserNotExist를 던짐
	*/
	void partChannelWithUserName(const string& channelName, const string& userName);

	/************************
	update
	************************/

	/*
	* @param oldUserName: 바꾸고 싶은 유저 이름
	* @param newUserName: 바꾸려는 유저 이름
	* @throw 만약 oldUserName이 존재하지 않는다면, ServerService::UserNotExist 던짐
	* @throw 만약 newUserNmae이 이미 존재한다면, ServerService::UserExist 던짐
	*/
	void changeUserName(const string& oldUserName, const string& newUserName);

	/*
	* @param fd : 새로 연결 된 fd
	* @param password : 새로 연결 된 fd가 입력한 password
	*/
	void insertPassword(const int& fd, const string& password) {
		_passwordMap[fd] = password;
	}

	const string& getMappedPassword(const int& fd) {
		map<int, string>::iterator it = _passwordMap.find(fd);
		return it->second;
	}

	class UserNotExist: public exception {
	public:
		const char* what() const throw();
	};

	class UserAlreadyExist: public exception {
	public:
		const char* what() const throw();
	};
	class ChannelNotExist: public exception {
	public:
		const char* what() const throw();
	};
	class ChannelAlreadyExist: public exception {
	public:
		const char* what() const throw();
	};
};

#endif