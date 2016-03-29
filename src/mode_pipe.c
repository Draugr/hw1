#include "funct.h"

void pipe_handle(int signum, siginfo_t *siginfo, void *data) {
	fprintf(stdout, "Signal SIGPIPE has received.\n");
}

void mode_pipe_f() {
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	fprintf(stdout, "PIPE mode ON.\n");

	int fds[2];
	int child_pid;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &pipe_handle;

	if (0 > sigaction(SIGPIPE, &act, NULL)) {
		system_call_error("sigaction");
	}

	if (0 > pipe(fds)) {
		system_call_error("pipe");
	}

	child_pid = fork();

	if (0 > child_pid) {
		system_call_error("fork");
	} else if ( 0 == child_pid )
	{
		// child
		fprintf(stdout, "Child is working...\n");

		char buffer[16];

		fprintf(stdout, "Closing pipe.\n");

		close(fds[0]);
		close(fds[1]);

		fprintf(stdout, "Child ending work.\n");
		exit( EXIT_SUCCESS );
	}

	sleep(1);

	fprintf(stdout, "Writing...\n");
	close(fds[0]);

	write(fds[1], "Hello!", 6);

	wait();

	fprintf(stdout, "Child exited.\n");
	fprintf(stdout, "PIPE mode succesfully completed the task.\n");
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

	exit( EXIT_SUCCESS );
}
