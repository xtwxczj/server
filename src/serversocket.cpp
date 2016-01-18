#include "../header/serversocket.h"
using namespace std;

/*
struct HOSTINFO
{
	char host_domain[64];		//宿主机域名
	double host_cpu_util;
	double host_mem_util;
};

Json::Value Jsoninfo;
Json::Reader reader;
*/
/*
void do_recv(int sockfd,struct sockaddr* pcliaddr, socklen_t client)
{
	int n;
	socklen_t len;
	char mesg[80];
	
	for(::)
	{
		len = client;
		n = recvfrom(sockfd,mesg,80,0,pcliaddr,&len);
		
	}
}
*/

int HOSTINFO::record()
{
	
	class MY_SQLC msql;
	
	char cmd[4096];
	
	time_t tt =0;
	time(&tt);
	int rc = msql.init(host,username,pwd,database);
	if(rc == -1) {printf("error for mysql init\n");printf(msql.getmsg());return 0;}
	
	
	
	int res = 0;
    
    if(sock_fd < 0)
    {
        printf("error: Create Socket Failed!");
        return -1;
    }
     
    
     
    try
    {
        if(bind(sock_fd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr)) != 0)
        {
            printf("error: Bind Socket Failed!");
            res = -1;
            throw res;
        }
         
       
		
		//int length = sizeof(HOSTINFO);
		//char* RecvBuffer = (char*)malloc(sizeof(HOSTINFO)+1);
		
		
       // HOSTINFO info;
        while(1)
        {
            printf("\nWaiting for data...\n");
            fromlen = sizeof(struct sockaddr);
			
			memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
			//recvfrom(sock_fd, RecvBuffer, sizeof(RecvBuffer),0,(struct sockaddr*)&from, &fromlen);
			
			
            if((recvslen = recvfrom(sock_fd, RecvBuffer, sizeof(RecvBuffer),0,(struct sockaddr*)&from, &fromlen)) == -1)
            {
                printf("error: Receiving Failed!");
                res = -1;
                throw res;
            }
            else
            {
				/*
                memcpy(&info,RecvBuffer,sizeof(info));//将数据写入结构体对象
				printf("%s",RecvBuffer);
				sleep(1);
				*/
				
				string str_value(RecvBuffer);
				reader.parse(str_value,Jsoninfo);
				std::string hostname = Jsoninfo["host_domain"].asString();
				//const int len = hostname.length();
				//info.host_domain = new char[len+1];
				memset(host_domain,0x00,sizeof(host_domain));
				strcpy(host_domain,hostname.c_str());
				host_cpu_util = Jsoninfo["host_cpu_util"].asDouble();
				host_mem_util = Jsoninfo["host_mem_util"].asDouble();
				disk = Jsoninfo["disk"].asDouble();
				
				//printf("%s",inet_ntoa(from));
                printf("Received data:\n[%s]\n",RecvBuffer);
				printf("host_name = %s\n",host_domain);
				//printf(hostname);
				printf("cpu_util = %f\n",host_cpu_util);
				printf("mem_util = %f\n",host_mem_util);
				printf("%s",inet_ntoa(from.sin_addr));
				memset(cmd,0,4096);
				sprintf(cmd,"insert into %s.hypervisor_util values(%d,'%s','%s',%f,%f,NOW());",database,0,host_domain,inet_ntoa(from.sin_addr),host_cpu_util,host_mem_util);
				msql.insert(cmd);
				memset(cmd,0,4096);
				sprintf(cmd,"update %s.hypervisor set host_ip='%s',disk_gb_free='%f',check_at=NOW() where host_name='%s';",database,inet_ntoa(from.sin_addr),disk,host_domain);
				msql.insert(cmd);
				
            }
        }
	
    }
    catch(...)
    {
        printf("error catched~");
    }
    
    
     
    return res;
}

HOSTINFO::HOSTINFO()
{
	host = getenv("CLOUDFEE_DB_ADDR");
  	username = getenv("CLOUDFEE_DB_USER");
	pwd = getenv("CLOUDFEE_DB_PWD");
	database = getenv("CLOUDFEE_DB");
	
	
	
	
	sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
	sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(SERVER_PORT);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(sock_addr.sin_zero, 0x00, 8);
	
	memset(RecvBuffer, 0x00, sizeof(RecvBuffer));//将接收缓冲区格式化
}

HOSTINFO::~HOSTINFO()
{
	free(RecvBuffer);
	close(sock_fd);
}





