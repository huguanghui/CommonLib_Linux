#ifndef __proc_uptime_h__
#define __proc_uptime_h__

#ifdef __cplusplus
extern "C" {
#endif

struct system_uptime {
    double uptime; // 系统启动时间
    double idletime; // 系统空闲时间
};

int ProcUptime(struct system_uptime *uptime);

#ifdef __cplusplus
}
#endif

#endif // !__proc_uptime_h__
