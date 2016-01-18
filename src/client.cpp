#include<sys/types.h>
#include<sys/socket.h>
#include <sys/statfs.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>

#include <iostream>
#include <sstream>
#include <json/json.h> 
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024
#define CK_TIME   1  //间隔时间，单位是秒

using namespace std;
struct HOSTINFO
{
	char host_domain[64];		//宿主机域名
	double host_cpu_util;
	double host_mem_util;
};

Json::Value Jsoninfo;
Json::FastWriter fast_writer;


double get_cpu_util()
{
	 FILE *fp;
        char buf[128];
	char cpu[3];
       // char* cpu = "cpu";
        long int user,nice,sys,idle,iowait,irq,softirq;
	char userbuf[20],nicebuf[20],sysbuf[20],idlebuf[20],iowaitbuf[20],irqbuf[20],softirqbuf[20];
	int temp1,temp2,temp3;
        long int all1,all2,idle1,idle2;
        double usage;

        while(1)
        {
                fp = fopen("/proc/stat","r");
                if(fp == NULL)
                {
                        perror("fopen:");
                        exit (0);
                }

		memset(buf,0x00,sizeof(buf));
                fgets(buf,sizeof(buf),fp);
#if __DEBUG__
                printf("buf=%s",buf);
#endif
	//	cpu = "cpu";
		char* buf1="123 456";
		int abc;
		int def;
		char cc[5];
		sscanf(buf1,"%d %d",&abc,&def);
		//char userbuf[100];
                sscanf(buf,"%s %s %s %s %s %s %s %s %d %d %d",cpu,userbuf,nicebuf,sysbuf,idlebuf,iowaitbuf,irqbuf,softirqbuf,&temp1,&temp2,&temp3);
		user = atoi(userbuf);
		nice = atoi(nicebuf);
		sys = atoi(sysbuf);
		idle = atoi(idlebuf);
		iowait = atoi(iowaitbuf);
		irq = atoi(irqbuf);
		softirq = atoi(softirqbuf);

		memset(userbuf,0x00,sizeof(userbuf));
		memset(nicebuf,0x00,sizeof(nicebuf));
		memset(sysbuf,0x00,sizeof(sysbuf));
		memset(idlebuf,0x00,sizeof(idlebuf));
		memset(iowaitbuf,0x00,sizeof(iowaitbuf));
		memset(irqbuf,0x00,sizeof(irqbuf));
		memset(softirqbuf,0x00,sizeof(softirqbuf));

#if __DEBUG__
		printf("%s,%d,%d,%d,%d,%d,%d,%d\n",cpu,user,nice,sys,idle,iowait,irq,softirq);
#endif

                all1 = user+nice+sys+idle+iowait+irq+softirq;
                idle1 = idle;
                rewind(fp);
                /*第二次取数据*/
                sleep(CK_TIME);
                memset(buf,0,sizeof(buf));
                cpu[0] = '\0';
                user=nice=sys=idle=iowait=irq=softirq=0;
                fgets(buf,sizeof(buf),fp);
#if __DEBUG__
                printf("buf=%s",buf);
#endif
             //   sscanf(buf,"%s%d%d%d%d%d%d%d",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
		
		sscanf(buf,"%s %s %s %s %s %s %s %s %d %d %d",cpu,userbuf,nicebuf,sysbuf,idlebuf,iowaitbuf,irqbuf,softirqbuf,&temp1,&temp2,&temp3);
                user = atoi(userbuf);
                nice = atoi(nicebuf);
                sys = atoi(sysbuf);
                idle = atoi(idlebuf);
                iowait = atoi(iowaitbuf);
                irq = atoi(irqbuf);
                softirq = atoi(softirqbuf);

                memset(userbuf,0x00,sizeof(userbuf));
                memset(nicebuf,0x00,sizeof(nicebuf));
                memset(sysbuf,0x00,sizeof(sysbuf));
                memset(idlebuf,0x00,sizeof(idlebuf));
                memset(iowaitbuf,0x00,sizeof(iowaitbuf));
                memset(irqbuf,0x00,sizeof(irqbuf));
                memset(softirqbuf,0x00,sizeof(softirqbuf));                
/*
#if __DEBUG__
printf("%s,%d,%d,%d,%d,%d,%d,%d\n",cpu,user,nice,sys,idle,iowait,irq,softirq);
#endif
*/
                all2 = user+nice+sys+idle+iowait+irq+softirq;
                idle2 = idle;

                usage = (double)(all2-all1-(idle2-idle1)) / (all2-all1)*100 ;


                printf("all=%d\n",all2-all1);
                printf("ilde=%d\n",all2-all1-(idle2-idle1));
                printf("cpu use = %.2f\%\n",usage);
                printf("=======================\n");

                fclose(fp);
				return usage;
        }
}

double get_mem_util()
{
	double mem_util;
	char buf[128];
	memset(buf, 0, 128);
	
	char sStatBuf[256];
	FILE* fp;
	int flag;
	int TotalMem;
	int UsedMem;
	char* line;
	if(system("free -m | awk '{print $2,$3}' > mem"));
			memset(sStatBuf, 0, 256);
	fp = fopen("mem", "rb");
	if(fp < 0)
	{
		return -1;
	}
	fread(sStatBuf,1, sizeof(sStatBuf) , fp);
 
	line = strstr(sStatBuf, "\n");
	TotalMem = atoi(line);
	line = strstr(line, " ");
	UsedMem = atoi(line);
	memset(sStatBuf, 0, 256);
	sprintf(sStatBuf, "Used %dM/Total %dM\n", UsedMem, TotalMem);
	/*
	if(strlen(sStatBuf) > len)
	{
		return -1;
	}
	*/
	mem_util = (double)UsedMem/TotalMem*100;
    //memcpy(Info, sStatBuf, strlen(sStatBuf));
    return mem_util;
	
	
}

double get_disk()
{
	struct statfs diskInfo;  
      
	statfs("/", &diskInfo);  
    unsigned long long blocksize = diskInfo.f_bsize;    //每个block里包含的字节数  
    unsigned long long totalsize = blocksize * diskInfo.f_blocks;   //总的字节数，f_blocks为block的数目  
    printf("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n",   
        totalsize, totalsize>>10, totalsize>>20, totalsize>>30);  
      
    unsigned long long freeDisk = diskInfo.f_bfree * blocksize; //剩余空间的大小  
    unsigned long long availableDisk = diskInfo.f_bavail * blocksize;   //可用空间大小  
    printf("Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n",   
        freeDisk>>20, freeDisk>>30, availableDisk>>20, availableDisk>>30); 
	unsigned long long temp = freeDisk>>30;
	return (double)temp;
}

int get_host_name(char* host_name)
{
	int z;
    char buf[100];
    z = gethostname(buf,sizeof(buf));
    if(z==-1)
    {
		fprintf(stderr,"%s:gethostname(2)\n",strerror(errno));
		exit(1);
    }
    printf("host name = '%s'\n",buf);
	strcpy(host_name,buf);
    return 0;
}

/** 
 * double转换为string 
 */  
std::string convertToString(double d) {  
	ostringstream os;  
	if (os << d)  
		return os.str();  
	std::string error = "invalid conversion";
	return error;  
}  

/** 
 * double转string 
 */  
double convertFromString(string str) {  
	istringstream iss(str);  
	double x;  
	if (iss >> x)  
		return x;  
	return 0.0;  
}  
int main()
{
	//printf("cpu_util=%f\n",get_cpu_util());
	//printf("mem_util=%f\n",get_mem_util());
	//get_host_name();
	//get_disk();
	
	HOSTINFO info;
	get_host_name(info.host_domain);
	info.host_cpu_util = get_cpu_util();
	info.host_mem_util = get_mem_util();
	printf("mem_util=%f\n",info.host_mem_util);
	
	Jsoninfo["host_domain"] = Json::Value(info.host_domain);
	Jsoninfo["host_cpu_util"] = Json::Value(info.host_cpu_util);
	Jsoninfo["host_mem_util"] = Json::Value(info.host_mem_util);
	Jsoninfo["disk"] = Json::Value(get_disk());
	//Jsoninfo["host_cpu_util"] = convertToString(info.host_cpu_util);
	//Jsoninfo["host_mem_util"] = convertToString(info.host_mem_util);
	std::string str_value = fast_writer.write(Jsoninfo);
	
	char* value;
	const int len = str_value.length();
	value = new char[len+1];
	strcpy(value, str_value.c_str());
	
	
	int res = 0;
    int sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0)
    {
        printf("error: Create Socket Failed!");
        return -1;
    }
     
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(SERVER_PORT);
    sock_addr.sin_addr.s_addr = inet_addr("192.168.83.116");
    memset(sock_addr.sin_zero, 0x00, 8);
     
    try
    {        
        //const char * tos = "abc";
        //int toslen = strlen(tos) + 1;
        struct sockaddr_in to = sock_addr;
        uint32_t tolen = sizeof(struct sockaddr);        
        printf("\nSending for data...\n");
		int length = sizeof(HOSTINFO);
		/*	
		//申请缓存
		char SendBuffer[++length]; 
		memset(SendBuffer,0x00,sizeof(SendBuffer));
		//将数据写入缓存
		//memcpy(SendBuffer,&info,sizeof(info));
		sprintf(SendBuffer, "%s?%f?%f", info.host_domain, info.host_cpu_util, info.host_mem_util);//会自动追加结束符
		int ret = sendto(sock_fd, SendBuffer, (int)strlen(SendBuffer), 0, (struct sockaddr*)&to,tolen);
        	*/
		int ret = sendto(sock_fd, value, (int)strlen(value), 0, (struct sockaddr*)&to,tolen);
        if(ret == -1)
        {
            printf("error: Sending Failed!");
            res = -1;
            throw res;
        }
        else
        {
            char s[256];
            //strncpy(s, tos, toslen);
            printf("Sended data:\n[%s]\n\n", s);
        }
		//free(SendBuffer); 
    }
    catch(...)
    {
        printf("error catched~");
    }
   
    close(sock_fd);
     
    return 0;
	

}
