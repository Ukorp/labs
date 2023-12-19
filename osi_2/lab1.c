#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid, ppid, pgid, uid, euid, gid, egid;

    pid = getpid();
    ppid = getppid();
    pgid = getpgid(0);
    uid = getuid();
    euid = geteuid();
    gid = getgid();
    egid = getegid();

    printf("Process ID: %d\n", pid);
    printf("Parent Process ID: %d\n", ppid);
    printf("Process Group ID: %d\n", pgid);
    printf("Real User ID: %d\n", uid);
    printf("Effective User ID: %d\n", euid);
    printf("Real Group ID: %d\n", gid);
    printf("Effective Group ID: %d\n", egid);

    return 0;
}