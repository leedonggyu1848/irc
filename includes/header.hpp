#ifndef HEADER_HPP
#define HEADER_HPP

#include <cstring>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <poll.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdexcept>
#include <set>

using namespace std;

#define PORT 20162
#define BUFFER_SIZE 512
#define LISTEN_QUEUE_SIZE 5
#define MAX_FD_SIZE 5
#define MAX_USER_SIZE MAX_FD_SIZE
#define MAX_CHANNEL_SIZE 5

// MODE
#define USER_MODE_I 0
#define USER_MODE_S 1
#define USER_MODE_W 2
#define USER_MODE_O 3

#define CHANNEL_MODE_O 0
#define CHANNEL_MODE_P 1
#define CHANNEL_MODE_S 2
#define CHANNEL_MODE_I 3
#define CHANNEL_MODE_T 4
#define CHANNEL_MODE_N 5
#define CHANNEL_MODE_M 6
#define CHANNEL_MODE_L 7
#define CHANNEL_MODE_B 8
#define CHANNEL_MODE_V 9
#define CHANNEL_MODE_K 10

// vector<string>	split(const string& str, const string& delimiter);

#endif
