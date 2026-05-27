/* ------------------------------------------------ ------------
File: cpr.c

Last name: Wang
Student number: 300411829

Last Name: Xie
Student number: 300452622

Description: This program contains the code for creation
 of a child process and attach a pipe to it.
	 The child will send messages through the pipe
	 which will then be sent to standard output.

Explanation of the zombie process
(point 5 of "To be completed" in the assignment):
Answer: A zombie process is one that has terminated but 
whose exit status has not yet been collected by its parent. 
Since we use the pipe close as our synchronization signal
(not wait()), the child becoms a zombie until the parent eventually
exists, at which point init(PID 1) adopts and reaps it.
------------------------------------------------------------- */
#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
/* Prototype */
void createChildAndRead(int);

/* -------------------------------------------------------------
Function: main
Arguments: 
	int ac	- number of command arguments
	char **av - array of pointers to command arguments
Description:
	Extract the number of processes to be created from the
	Command line. If an error occurs, the process ends.
	Call createChildAndRead to create a child, and read
	the child's data.
-------------------------------------------------- ----------- */

int main(int ac, char **av) {
    int processNumber;

    if (ac == 2) {
        if (sscanf(av[1], "%d", &processNumber) == 1) {
            createChildAndRead(processNumber);
        } else fprintf(stderr, "Cannot translate argument\n");
    } else fprintf(stderr, "Invalid arguments\n");
    return (0);
}


/* ------------------------------------------------ -------------
Function: createChildAndRead
Arguments: 
	int prcNum - the process number
Description:
	Create the child, passing prcNum-1 to it. Use prcNum
	as the identifier of this process. Also, read the
	messages from the reading end of the pipe and sends it to 
	the standard output (df 1). Finish when no data can
	be read from the pipe.
-------------------------------------------------- ----------- */

void createChildAndRead(int prcNum) {
    /* case 1: prcNum == 1 -> leaf process, no child created */
     if (prcNum == 1) {
    	char buf [64];
    	int len;

    	len = sprintf(buf, "Process 1 begins\n");
    	write(1, buf, len);

    	sleep(5);

    	len = sprintf(buf, "Process 1 ends\n");
    	write(1, buf, len);

    	sleep(10);

    	close(1);
    	exit(0);
    }

	/*
	 * case 2: prcNum >= 2 -> Internal process, creates a child
	 * step 1: Create a pipe.
     * fd[9]: read en
     * fd[2]: write end
     *
     * create a pipe
	 */
	int fd[2];
	if (pipe(fd) == -1) {
		fprintf(stderr, "pipr() failed\n");
		exit(1);
	}

    char childArg[32];
	sprintf(childArg, "%d", prcNum - 1);

	char *args[] = { "./cpr", childArg, NULL };

	/* step 2: build the argument string for execvup.
	 * we ll call cpr(prcNum-1)
     */
    pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "fork() failed\n");
        exit(1);
    }

    if (pid == 0)
    {
        dup2(fd[1],1);
        close(fd[0]);
        close(fd[1]);

        execvp("./cpr", args);
        
        fprintf(stderr, "execvp() failed\n");
        exit(1);
    }

    /*
     *step 4: parent close the write end of the pipe.
     * */
    close(fd[1]);

    /* step 5: print and stdout*/
    char buf[256];
    int len;
    
    len = sprintf(buf, "Process %d begins\n", prcNum);
    write(1, buf, len);
    

    char readBuf[512];
    int bytesRead;

    while ((bytesRead = read(fd[0], readBuf, sizeof(readBuf))) > 0)
    {
        write(1, readBuf, bytesRead);
    }

    close(fd[0]);

    len = sprintf(buf, "process %d ends\n", prcNum);
    write(1, buf, len);

    sleep(10);
    exit(0);

    }