#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

/* l'execution du programme commence ici */
int main(int argc, char **argv)
{
    char    *program;
    pid_t   pid_prog, pid_procmon;
    char    pidstr[20];

    if (argc != 2) {
        printf("Usage: mon <nomFichier>\n - nomFichier est le nom d'un fichier executable.\n");
        exit(-1);
    } else
        program = argv[1];

    /* 1. Fork and exec the target program, obtain its PID */
    pid_prog = fork();
    if (pid_prog == 0) {
        execl(program, program, NULL);
        exit(-1);
    }

    /* 2. Start procmon with the target program's PID */
    sprintf(pidstr, "%d", pid_prog);
    pid_procmon = fork();
    if (pid_procmon == 0) {
        execl("./procmon", "procmon", pidstr, NULL);
        exit(-1);
    }

    /* 3. Sleep 20 seconds */
    sleep(20);

    /* 4. Kill the target program */
    kill(pid_prog, SIGTERM);

    /* 5. Sleep 2 seconds */
    sleep(2);

    /* 6. Kill procmon */
    kill(pid_procmon, SIGTERM);

    return 0;
}
