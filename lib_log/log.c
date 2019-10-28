/**
日志打印示例。
使用：
mylog(DEBUG, "This is debug info\n");
结果：
[2018-07-22 23:37:27:172] [DEBUG] [main.cpp:5] This is debug info
默认打印当前时间（精确到毫秒）、文件名称、行号。
*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
 
 
#include "log.h"
 
//#ifndef LOGLEVEL
//#define LOGLEVEL DEBUG
//#endif
 
// 使用了GNU C扩展语法，只在gcc（C语言）生效，
// g++的c++版本编译不通过
static const char* s_loginfo[] = {
    [ERROR] = "ERROR",
    [WARN]  = "WARN",
    [INFO]  = "INFO",
    [DEBUG] = "DEBUG",
};
 
 
static char file_names[LOGFILE_MAXCOUNT][LOGFILE_NAMELENTH];
//记录文件名前缀（最好取自终端编号）
static char file_prifix[LOGFILE_NAMELENTH];
 
 
//linux消息队列
static int s_msg_id;
static int r_msg_id;
 
#define MSG_TYPE 1001
 
#define MAX_TEXT 1024
 
struct msg_st{
	long int msg_type;
	char text[MAX_TEXT];
};
 
 
static pthread_t tid;
 
//=============================================
static void get_timestamp(char *buffer)
{
    time_t t;
    struct tm *p;
    struct timeval tv;
    int len;
    int millsec;
 
    t = time(NULL);
    p = localtime(&t);
 
    gettimeofday(&tv, NULL);
    millsec = (int)(tv.tv_usec / 1000);
 
    /* 时间格式：[2011-11-15 12:47:34:888] */
    len = snprintf(buffer, 32, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ",
        p->tm_year+1900, p->tm_mon+1,
        p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, millsec);
 
    buffer[len] = '\0';
}
 
//获取当前时间
static int get_curtime( char* outTimeStr )
{
	int ret = 0;
	time_t tTime;
	struct tm *tmTime;
	struct timeval mTime;
	time( &tTime );
	tmTime = localtime( &tTime );
	gettimeofday( &mTime, NULL );
	sprintf( outTimeStr, "%04d%02d%02d%02d%02d%02d",
			tmTime->tm_year + 1900, tmTime->tm_mon + 1,
			tmTime->tm_mday, tmTime->tm_hour,
			tmTime->tm_min, tmTime->tm_sec );
	return ret;
}
 
 
//创建文件夹
static int create_dir(const char *sPathName)  
{  
	char dirName[256];  
	strcpy(dirName, sPathName);  
	int i,len = strlen(dirName);
	for(i=1; i<len; i++)  
	{  
		if(dirName[i]=='/')  
		{  
			dirName[i] = 0; 
			if(access(dirName, 0)!=0)  
			{  
				if(mkdir(dirName, 0755)==-1)  
				{   
					fprintf(stderr,"mkdir   error\n");   
					return -1;   
				}  
			}  
			dirName[i] = '/';  
 
		}  
	}  
 
	return 0;  
} 
 
 
//获取文件大小
static unsigned long get_size(const char *path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	if(stat(path, &statbuff) < 0){
		return filesize;
	}else{
		filesize = statbuff.st_size;
	}
	return filesize;
}
 
//文件是否存在,返回 1文件存在，返回0，文件不存在
// static int file_exists(char *filename)
// {
// 	return (access(filename, 0) == 0);
// } 
 
 
static int read_filelist(char *basePath)
{
	DIR *dir;
	struct dirent *ptr;
	// char base[1000];
	int count = 0;
 
	if ((dir=opendir(basePath)) == NULL)
	{
		fprintf(stderr,"Open dir error...");
		return -1;
	}
 
	while ((ptr=readdir(dir)) != NULL)
	{
		//printf("count=%d\n",i++);
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
			continue;
		else if(ptr->d_type == 8)    ///file
		{
			printf("f_name:%s/%s\n",basePath,ptr->d_name);
			sprintf(file_names[count],"%s",ptr->d_name);
			count++;
			if(count > LOGFILE_MAXCOUNT -1){
				fprintf(stderr,"error!only allow files count=%d\n",LOGFILE_MAXCOUNT);
				return -2;
			}
		}
		else if(ptr->d_type == 10)    ///link file
			printf("l_name:%s/%s\n",basePath,ptr->d_name);
		else if(ptr->d_type == 4)    ///dir
		{
			printf("d_name:%s/%s\n",basePath,ptr->d_name);
			printf("this is a dir\n");
			continue;
			//是目录，继续递归读目录下面的
			//memset(base,'\0',sizeof(base));
			//strcpy(base,basePath);
			//strcat(base,"/");
			//strcat(base,ptr->d_name);
			//read_filelist(base);
		}
	}
	closedir(dir);
	return 0;
}
 
 
//处理日志文件是否保留
//原理算法：把日期转换成时间戳，然后由配置的允许保留的天数换算出一个时间范围，
//在遍历日志目录中所有的文件名，提取出日期，在这个时间范围内的保留，否则删除
//关键的地方，算出这个允许保留文件的时间范围，原理是日期转时间戳，再时间戳转日期
static int file_alives_proc()
{
	int ret = 0;
	char curtime[20];  //当前日期时间
	char deadtime[20]; //截止的历史日期
 
 
	printf("file_alives_proc:\n");
	get_curtime(curtime);
	//只取日期，省去时间
	memset(&curtime[8],0x30,6);
	printf("ailve maxdays:%d\n",LOGFILE_ALIVEDAYS);
	printf("curtime:%s\n",curtime);
	
	struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));
	//字符串转时间
	strptime(curtime,"%Y%m%d%H%M%S",tmp_time);
	time_t t = mktime(tmp_time);
	printf("t now = %ld\n",t);
 
	free(tmp_time);  
 
	time_t t1 = t - LOGFILE_ALIVEDAYS*24*60*60;
	//再把t1转换为时间，即时间戳转时间
	
	struct tm *p; 
	p = gmtime(&t1); 
	//日期时间转字符串,由于只比较日期，因此忽略时间
	strftime(deadtime, sizeof(deadtime), "%Y%m%d000000", p); 	
	printf("deadtime:%s\n",deadtime);
 
	//以上获取到了curtime和deadtime,有了这个时间范围，接下来就去找这个范围的日志
	//日志文件日期在这个范围内的保留，否则删除
	for(int i = 0; i < LOGFILE_MAXCOUNT; i++ )
	{
		if(strlen(file_names[i]) > 0)
		{
			printf("file_name=%s\n",file_names[i]);
			char ftime[20];
			memset(ftime,0,20);
			memset(ftime,0x30,8);
			//关键，这个截取不能错
			memcpy(ftime,&file_names[i][strlen(LOGFILE_PREFIX)+1+strlen(file_prifix)],8);
			printf("file_time=%s\n",ftime);
			
			//开始比较 是否在日期范围内
			if(memcmp(ftime,deadtime,8) > 0)
			{
				//大于截止日期的文件保留
				printf("%s------keep alive\n",file_names[i]);
			}else{
				printf("%s----------------dead,need remove!\n",file_names[i]);
				//删除文件
				char dfname[50];
				sprintf(dfname,"%s/%s",LOGFILE_PATH,file_names[i]);
				remove(dfname);
			}
			//
		}else{
			//printf("fname=NULL\n");
		}
 
	}
 
	return ret;
}
 
 
 
int open_msg(void) {
	key_t msgkey;
	if ((msgkey = ftok("/tmp", 'a')) < 0) {
		printf("send ftok failed!\n");
		return -1;
	}
	printf("----msgkey is %d\n",msgkey);
	if ((s_msg_id = msgget(msgkey, IPC_CREAT | 0666)) == -1) {
		printf("msgget failed!\n");
		return -1;
	}
 
	printf("----s_msg_id is %d\n",s_msg_id);
	msgctl(s_msg_id,IPC_RMID,0);//先删除，否则可能满，因其生命周期同内核
 
	if ((s_msg_id = msgget(msgkey, IPC_CREAT | 0666)) == -1) {
		printf("msgget failed!\n");
		return -1;
	}
 
 
	r_msg_id = s_msg_id;
	return 0;
}
 
static int send_msg(char *buf,int len){
	struct msg_st data;
	data.msg_type = MSG_TYPE;
	strcpy(data.text,buf);
	//s_msg_id = 0;
	if(msgsnd(s_msg_id,&data,len,IPC_NOWAIT) == -1){
		printf("msgsnd failed.\n");
		perror("msgsnd");
		return -1;
	}
	return 0;
}
 
 
//若大量连续发的太快，收的太慢，会导致发送失败
static int recv_msg(void)
{
	int rsize;
	struct msg_st data;
	int msgtype = MSG_TYPE;
 
	char tmpfname[ 50 ];
	char tmpTime[ 14 + 1 ];
	FILE* fp;
	unsigned long filesize;
 
	memset(data.text,0,sizeof(data.text));
	/*接收消息队列*/    
	//阻塞接收
	rsize = msgrcv(r_msg_id,&data,sizeof(data.text),msgtype,MSG_NOERROR );    
	if(rsize == -1)
	{
		if (errno != ENOMSG) {
			perror("msgrcv");
		}
		printf("No message available for msgrcv()\n");
		return -1;
	}
	else
	{
		//printf("message received: %s\n", data.text);
		get_curtime( tmpTime );
		sprintf( tmpfname, "%s/%s%s_%8.8s.log", LOGFILE_PATH, LOGFILE_PREFIX,file_prifix, tmpTime );
		fp = fopen( tmpfname, "a" );
		if ( NULL == fp )
		{
			fprintf(stderr,"failed to open file,filename=%s\n",tmpfname);
			return -2;
		}
		else
		{
			filesize = get_size(tmpfname);
		//	printf("filesize=%u\n",filesize);
			if((filesize/1024) > LOGFILE_MAXSIZE){
				fprintf(stderr,"failed to write log,only allow maxsize=%ukb,current size=%lukb\n",LOGFILE_MAXSIZE,filesize/1024);
				fclose(fp);
				return -3 ;
			}
			//printf("%s\n",tmpfname);
			fprintf( fp, "%s", data.text );
			fclose(fp);
		}
	}
	return 0;
}
 
void mylog1(const char* filename, int line, enum LogLevel level, const char* fmt, ...)
{
    if(level > LOGLEVEL)
        return;
 
    va_list arg_list;
    char buf[1024];
	char sbuf[MAX_TEXT];
 
    memset(buf, 0, 1024);
	memset(sbuf,0,MAX_TEXT);
 
    va_start(arg_list, fmt);
    vsnprintf(buf, 1024, fmt, arg_list);
    char time[32] = {0};
 
    // 去掉*可能*存在的目录路径，只保留文件名
    const char* tmp = strrchr(filename, '/');
    if (!tmp) tmp = filename;
    else tmp++;
    get_timestamp(time);
 
	switch(level){
		case DEBUG:
			//绿色
			sprintf(sbuf,"\033[1;32m%s[%s] [%s:%d] %s\n\033[0m", time, s_loginfo[level], tmp, line, buf);
			break;
		case INFO:
			//蓝色
			sprintf(sbuf,"\033[1;34m%s[%s] [%s:%d] %s\n\033[0m", time, s_loginfo[level], tmp, line, buf);
			break;
		case ERROR:
			//红色
			sprintf(sbuf,"\033[1;31m%s[%s] [%s:%d] %s\n\033[0m", time, s_loginfo[level], tmp, line, buf);
			break;
		case WARN:
			//黄色
			sprintf(sbuf,"\033[1;33m%s[%s] [%s:%d] %s\n\033[0m", time, s_loginfo[level], tmp, line, buf);
			break;
	}
 
	printf("%s",sbuf);
 
#if(LOGFILE_ENABLE)
	//记录日志到文件
	send_msg(sbuf,strlen(sbuf));
#endif
 
    va_end(arg_list);
}
 
 
//考虑了一点儿效率，操作IO比较耗时，那就异步写入吧
//想用单独的线程，负责写日志到文件,自己实现消息队列?
//不这么做了，直接使用linux的消息队列吧
 
static void* thread_writelog(void* args)
{
	mylog(INFO,"thread_writelog begin...\n");
	while(1)
	{
		recv_msg();
		usleep(20*1000);							
	}
    return NULL;
}
 
 
//参数prifix,日志文件名前缀,传终端编号
int init_log(char *prifix)
{
	int ret = 0;
	printf("init log:\n");
#if(LOGFILE_ENABLE)
	//检查目录是否存在
	ret = create_dir(LOGFILE_PATH);
	if(0 != ret){
		return ret;
	}
	printf("create dir %s\n success!\n",LOGFILE_PATH);
 
	ret = read_filelist(LOGFILE_PATH);
	if(0 != ret){
		printf("read_filelist err!,ret =%d\n",ret);
		//return ret;
	}
 
	//文件名前缀
	if(strlen(prifix) > 0){
		strcpy(file_prifix,prifix);
	}
	//处理是否保留历史记录文件
	file_alives_proc();
	//for(int i = 0; i<30;i++){
	//	printf("%s\n",file_names[i]);
	//}
	//创建消息队列
	ret = open_msg();
	if(0 != ret){
		return ret;
	}
	printf("create msg quene success!\n");
 
	//创建写日志文件线程
	ret = pthread_create(&tid,
			NULL,
			thread_writelog,
			NULL);
	if(0 != ret)
	{
		fprintf(stderr, "couldn't create thread_writelog, errno %d\n", ret);
	}
	else
	{
		printf("create thread_writelog success!\n");
	}
	printf("init log success!enabled log file...\n");
#else
	printf("init log success!no enable log file...\n");
#endif
 
	return ret;
}
