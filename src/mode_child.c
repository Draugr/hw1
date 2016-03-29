#include "funct.h"

void sigchld_handle(int signum, siginfo_t *siginfo, void *data) {
	fprintf(stdout, "Signal SIGCHLD has received. siginfo_t:\n");
		fprintf(stdout, "si_signo=%i\n", siginfo->si_signo );
		fprintf(stdout, "si_errno=%i\n", siginfo->si_errno );
		fprintf(stdout, "si_code=%i\n", siginfo->si_code );
		fprintf(stdout, "si_pid=%i\n", siginfo->si_pid );
		fprintf(stdout, "si_uid=%i\n", siginfo->si_uid );
		fprintf(stdout, "si_status=%i\n", siginfo->si_status );
		fprintf(stdout, "si_utime=%i\n", (int)siginfo->si_utime );
		fprintf(stdout, "si_stime=%i\n", (int)siginfo->si_stime );
		fprintf(stdout, "si_value.sival_int=%i\n", siginfo->si_value.sival_int );
		fprintf(stdout, "si_value.sival_ptr=%p\n", siginfo->si_value.sival_ptr );
		fprintf(stdout, "si_int=%i\n", siginfo->si_int );
		fprintf(stdout, "si_ptr=%p\n", siginfo->si_ptr );
		fprintf(stdout, "si_addr=%p\n", siginfo->si_addr );
		fprintf(stdout, "si_band=%ld\n", siginfo->si_band );
		fprintf(stdout, "si_fd=%i\n", siginfo->si_fd );

}

void mode_child_f() {
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	fprintf(stdout, "CHILD mode ON.\n");

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &sigchld_handle;

	if (0 > sigaction(SIGCHLD, &act, NULL)) {
		system_call_error("sigaction");
	}

	int child_pid = fork();

	if (0 > child_pid) {
		system_call_error("fork");
	}

	if ( 0 == child_pid )
	{
		// child
		fprintf(stdout, "Child is working...\n");

		sleep(rand() % 2 + 1);

		fprintf(stdout, "Child ending work.\n");
		exit( EXIT_SUCCESS );
	}

	wait();

	fprintf(stdout, "Child exited.\n");
	fprintf(stdout, "CHILD mode succesfully completed the task.\n");
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

	exit( EXIT_SUCCESS );
}
