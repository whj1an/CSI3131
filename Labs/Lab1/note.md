# CSI3131 Lab 1 — Process Monitoring in Linux

> **Course:** CSI3131 — Operating Systems  
> **Topic:** Process (进程) creation, process states (进程状态), and CPU/system time tracking using `fork()` (创建子进程), `exec()` (执行新程序), `kill()` (发送信号), and the `/proc` filesystem (Linux内核虚拟文件系统).

---

## Lab Requirements

Complete the `mon.c` program so that it:

1. Takes one command-line argument: the path to a target executable.
2. Uses `fork()` + `execl()` to launch the target program and saves its PID (Process ID，进程号).
3. Uses a second `fork()` + `execl()` to launch `./procmon`, passing the target's PID as a string argument.
4. Sleeps 20 seconds while `procmon` monitors the target.
5. Sends `SIGTERM` (终止信号) to kill the target program.
6. Sleeps 2 seconds, then sends `SIGTERM` to kill `procmon`.

---

## Key Files

- **`mon.c`** — The file the student must complete. It is the main driver that launches and manages both the target program and `procmon` using `fork()`/`exec()`/`kill()`.

- **`code/procmon.c`** — A pre-written process monitor (进程监视器). Takes a PID, reads `/proc/<PID>/stat` once per second, and prints the process state (`R`/`S`/`D`/`Z`), system time (内核态CPU时间), and user time (用户态CPU时间) in a table. Exits when the target process disappears.

- **`code/calcloop.c`** — A CPU-bound test program (CPU密集型程序). Alternates between sleeping 3 seconds and doing 400,000,000 arithmetic increments, 10 times. When monitored, `UsrTm` rises sharply during the loop and `SysTm` stays near zero.

- **`code/cploop.c`** — An I/O-bound test program (I/O密集型程序). Copies a 500,000-byte file one byte at a time using `read()`/`write()` system calls (系统调用), 10 times. When monitored, `SysTm` climbs significantly while `UsrTm` stays low — the opposite of `calcloop`.

- **`compile.sh`** — Shell script that compiles all `.c` files under `code/` with `gcc` and places the binaries in the current directory.

- **`calc.log` / `cp.log`** — Sample `procmon` output captured during a run of `calcloop` and `cploop` respectively. Used to observe and compare CPU-bound vs I/O-bound process behavior.

- **`tstcalc` / `tstcp`** — Shell wrapper scripts that run the full pipeline (`mon` + test program) and redirect output to the log files.

- **`fromfile`** — A leftover data file created by `cploop` at runtime; contains 500,000 `x` characters and serves as the copy source.

---

## Completion Process

The original `mon.c` had the correct structure (variable declarations and argument check) but the body after argument parsing was empty. The following implementation was added:

**File: `lab1/mon.c`**

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

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
```

**Implementation notes:**

- `fork()` returns `0` in the child process (子进程) and the child's PID in the parent process (父进程). The child calls `execl()` immediately to replace itself with the target program — so the parent retains `pid_prog` for later use.
- `sprintf(pidstr, "%d", pid_prog)` converts the integer PID to a string before passing it to `procmon` as a command-line argument.
- The second `fork()` + `execl("./procmon", "procmon", pidstr, NULL)` starts the monitor as a separate process running concurrently with the target.
- `kill(pid, SIGTERM)` sends signal 15 to the specified process, causing it to terminate by default.
- The 2-second gap between killing the target and killing `procmon` gives `procmon` time to detect the disappearance of `/proc/<PID>/stat` and print its final message before being stopped.
