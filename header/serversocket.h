#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>

#include "mysqlc.h"
#include <iostream>
#include <sstream>
#include <time.h>
#include <json/json.h> 
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

class HOSTINFO
{
private:
	
	char host_domain[64];		//宿主机域名
	double host_cpu_util;
	double host_mem_util;
	double disk;
	
	char* host;
  	char* username;
	char* pwd;
	char* database;
	
	
	
	Json::Value Jsoninfo;
	Json::Reader reader;
	
	int flag;					//记录是否有新数据到达
	
	int sock_fd;				//要绑定到端口上的socket
	struct sockaddr_in sock_addr;//socket地址
	
	char recvs[256];			//缓冲区
    int recvslen;
    struct sockaddr_in from;
    uint32_t fromlen;
	char RecvBuffer[1024];
public:
	int record();
	HOSTINFO();
	~HOSTINFO();
	
	
	
};