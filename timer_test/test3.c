#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/select.h>
#include <resolv.h>
#include <execinfo.h>
#include <pthread.h>

static inline uint64_t sim_monotonic_ms()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (uint64_t)tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
}

typedef int(m_timer_func_t)(void *u);

struct m_timer_args_t {
    void *u;
    m_timer_func_t *func;
    timer_t timerid;
};

static sem_t main_run_sem;
static int g_runing = 1;

void print_trace(void)
{
    void *array[16];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 16);
    strings = backtrace_symbols(array, size);
    if (NULL != strings) {
        printf("Obtained %zd stack frames.\n", size);
        for (i = 0; i < size; i++) {
            printf(" [%02ld] %s\n", i, strings[i]);
        }

        free(strings);
    }

    return;
}

static void signal_handler(int sig)
{
    printf("mpp signal_handler signal: %d\n", sig);
    if ((sig == SIGINT) || (sig == SIGTERM)) {
        sem_post(&main_run_sem);
    } else if ((sig == SIGSEGV) || (sig == SIGFPE)) {
        print_trace();
        abort();
    } else {
        struct sigaction sa;
        sa.sa_handler = SIG_DFL;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(sig, &sa, NULL);
        raise(sig);
    }

    return;
}

static void setup_signal_handler()
{
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // ctrl+c
    sigaction(SIGINT, &sa, NULL);
    // kill
    sigaction(SIGTERM, &sa, NULL);
    // Segmentation violation
    sigaction(SIGSEGV, &sa, NULL);
    // float point exception
    sigaction(SIGFPE, &sa, NULL);
    // broken pipe
    signal(SIGPIPE, SIG_IGN);

    return;
}

static void *check_task(void *parm)
{
    prctl(PR_SET_NAME, __FUNCTION__);
    int cnt = 0;

    while (g_runing) {
        usleep(500 * 1000);
        cnt++;
        cnt = (cnt) % 10000;
    }

    return NULL;
}

static void timer_thread(sigval_t v)
{
    struct m_timer_args_t *args;
    args = (struct m_timer_args_t *)v.sival_ptr;

    printf("Timer expired %lu\n", sim_monotonic_ms());
    timer_delete(args->timerid);

    return;
}

int main(int argc, char *argv[])
{
    pthread_t check_task_id;

    sem_init(&main_run_sem, 0, 0);
    setup_signal_handler();

    printf("Start timer: %lu\n", sim_monotonic_ms());
    struct itimerspec it;
    struct sigevent evp;
    struct m_timer_args_t *args;

    args = (struct m_timer_args_t *)calloc(1, sizeof(struct m_timer_args_t));

    memset(&evp, 0, sizeof(struct sigevent));
    evp.sigev_value.sival_ptr = args;
    evp.sigev_notify = SIGEV_THREAD;
    evp._sigev_un._sigev_thread._function = timer_thread;
    timer_create(CLOCK_REALTIME, &evp, &args->timerid);

    int ms = 200;
    it.it_interval.tv_sec = ms / 1000;
    it.it_interval.tv_nsec = 1000000 * (ms % 1000);
    it.it_value.tv_sec = ms / 1000;
    it.it_value.tv_nsec = 1000000 * (ms % 1000);
    timer_settime(args->timerid, 0, &it, 0);

    pthread_create(&check_task_id, NULL, check_task, NULL);
    sem_wait(&main_run_sem);
    g_runing = 0;
    pthread_join(check_task_id, NULL);
    sem_destroy(&main_run_sem);

    return EXIT_SUCCESS;
}
