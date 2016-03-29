#include "funct.h"

void sighandler_SIGPIPE(int signum, siginfo_t *siginfo, void *data) {
	fprintf(stdout, "SIGPIPE received.\n");
}

void do_pipe_mode() {
	fprintf(stdout, "Entering PIPE mode.\n");

	int fds[2];
	int child_pid;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &sighandler_SIGPIPE;

	if (0 > sigaction(SIGPIPE, &act, NULL)) {
		trigger_syscall_error("sigaction");
	}

	if (0 > pipe(fds)) {
		trigger_syscall_error("pipe");
	}

	child_pid = fork();

	if (0 > child_pid) {
		trigger_syscall_error("fork");
	} else if ( 0 == child_pid )
	{
		// child process
		fprintf(stdout, "I am child!\n");

		char buffer[16];

		fprintf(stdout, "closing pipe.\n");

		close(fds[0]);
		close(fds[1]);

		fprintf(stdout, "child end point.\n");
		exit( EXIT_SUCCESS );
	}

	sleep(1);

	fprintf(stdout, "trying to write.\n");
	close(fds[0]);

	write(fds[1], "test", 4);

	fprintf(stdout, "child exited, pid=%i\n", wait_child(child_pid));

	exit( EXIT_SUCCESS );
}
