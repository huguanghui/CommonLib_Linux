#ifndef __proc_pid_statm_h__
#define __proc_pid_statm_h__

#ifdef __cplusplus
extern "C" {
#endif

struct process_statm {
    unsigned long size; // total program size
    unsigned long resident; // resident set size
    unsigned long share; // shared pages
    unsigned long text; // text (code)
    unsigned long lib; // library
    unsigned long data; // data/stack
    unsigned long dt; // dirty pages (unused in Linux 2.6)
};

int ProcPidStatm(int pid, struct process_statm *statm);

#ifdef __cplusplus
}
#endif

#endif // !__proc_pid_statm_h__
