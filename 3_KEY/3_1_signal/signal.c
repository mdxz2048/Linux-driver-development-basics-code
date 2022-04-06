#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void sig_handler(int signo)
{
    if (signo == SIGIO)
        printf("received SIGIO\n");
}

int main(int argc, char **argv)
{
    pid_t pid = -1;
    if (signal(SIGIO, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGIO\n");
    // A long long wait so that we can easily issue a signal to this
    // process
    pid = getpid();
    printf("The pid is %d\n", pid);
    while (1)
        sleep(1);

    return 0;
}