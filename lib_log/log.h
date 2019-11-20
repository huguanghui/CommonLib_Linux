/*
//使用demo:
//mylog(DEBUG,"hello world!\n");
//输出:[2019-07-26 14:31:51:882] [DEBUG] comLib.c:1257] hello world!
//
//目前只为个人使用，暂无考虑线程安全,高效率和高并发
//考虑了一点儿效率，写文件操作IO比较耗时，因此日志使用了异步写入，linux消息队列。
//因linux的消息队列，容量和长度有限制，因此若单个消息超1024byte或并发发送几千个消息
//且发送速度很快，大于了队列的接收速度，那么肯定，会发送失败
*/
#ifndef LOG_H_
#define LOG_H_
 
#ifdef __cplusplus 
extern "C" {
#endif
 
enum LogLevel
{
    ERROR = 1,
    WARN  = 2,
    INFO  = 3,
    DEBUG = 4,
};
 
void mylog1(const char* filename, int line, enum LogLevel level, const char* fmt, ...) __attribute__((format(printf,4,5)));
 
#define mylog(level, format, ...) mylog1(__FILE__, __LINE__, level, format, ## __VA_ARGS__)
 
 
//================================================================
//============日志模块操作接口，配置和使用都在下面================
//日志级别定义，小于该级别的日志方能输出
#ifndef LOGLEVEL
#define LOGLEVEL DEBUG
#endif
 
//目前暂只支持管理目录下的30个日志文件名，文件名长度50以内。大了不处理
#define LOGFILE_MAXCOUNT  30
#define LOGFILE_NAMELENTH 50
 
//===========日志文件的配置=======================================
//是否启用记录日志到文件功能
#define LOGFILE_ENABLE 1
 
//日志文件的路径,后面不能带"/"
#define LOGFILE_PATH "/root/log"
//日志文件名称的前缀
#define LOGFILE_PREFIX "log_b503_"
//日志文件存在的时间 单位（天）,会自动删除当前日期-ALIVEDAYS 之前的文件
//限制日志最长保留时间不能超 LOGFILE_MAXCOUNT 天
#define LOGFILE_ALIVEDAYS 7  
 
//单个日志文件的限制大小 单位kb
#define LOGFILE_MAXSIZE 1*1024 
//================================================================
 
 
//日志模块初始化，若要记录日志到文件中，必须init_log且开启LOGFILE_ENABLE
//若不需要记录日志到文件功能，则不要调用init_log();
//调用了也没事，只是别开启LOGFILE_ENABLE
//参数prifix,日志文件名前缀，最好传终端唯一编号
extern int init_log(char *prifix); 
 
#ifdef __cplusplus 
};
#endif

#endif