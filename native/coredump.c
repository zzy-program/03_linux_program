#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

int main(void)
{
    int *a=NULL;
    //struct rlimit limit;

    //limit.rlim_cur = RLIM_INFINITY;
    //limit.rlim_max = RLIM_INFINITY;
    //if(setrlimit(RLIMIT_CORE, &limit) == 0)
    //   printf("setrlimit: RLIM_INFINITY\n");
    //else
    //   printf("failed to setrlimit.\n");
    printf("zzytest, pid=%d\n", getpid());
    *a = 3;
    return 0;
}