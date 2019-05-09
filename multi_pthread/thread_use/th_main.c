#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct abc {
    char abc[32];
    char par[32];
};

static int s_idx = 0;

static void *thr_fn(void *arg)
{
    struct abc *par = (struct abc *)arg;

    sleep(4);

    sprintf(par->abc, "result_%d", s_idx++);

	return NULL;
}

int main(int argc, char const *argv[])
{
    int i;
    struct abc data[4];
    pthread_t arrPid[4];

    for (i = 0; i < 4; i++) {
        pthread_create(&arrPid[i], NULL, thr_fn, &data[i]);
    }

    for (i = 0; i < 4; i++) {
        pthread_join(arrPid[i], NULL);
    }

    for (i = 0; i < 4; i++) {
        printf("result[%d][%s]\n", i, data[i].abc);
    }

    return 0;
}
