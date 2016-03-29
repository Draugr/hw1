#include "funct.h"

void sighandler_SIGCHLD(int signum, siginfo_t *siginfo, void *data) {

	fprintf(stdout, "SIGCHLD received. siginfo_t structure:  " \
		"si_signo=%i\n" \
		"si_errno=%i\n" \
		"si_code=%i\n" \
		"si_pid=%i\n" \
		"si_uid=%i\n" \
		"si_status=%i\n" \
		"si_utime=%i\n" \
		"si_stime=%i\n" \
		"si_value.sival_int=%i\n" \
		"si_value.sival_ptr=%p\n" \
		"si_int=%i\n" \
		"si_ptr=%p\n" \
		"si_addr=%p\n" \
		"si_band=%ld\n" \
		"si_fd=%i\n",
		siginfo->si_signo, siginfo->si_errno, siginfo->si_code, siginfo->si_pid,
		siginfo->si_uid, siginfo->si_status, (int)siginfo->si_utime, (int)siginfo->si_stime,
		siginfo->si_value.sival_int, siginfo->si_value.sival_ptr, siginfo->si_int,
		siginfo->si_ptr, siginfo->si_addr, siginfo->si_band, siginfo->si_fd
	);
}


void do_child_mode() {
	fprintf(stdout, "Entering CHILD mode.\n");

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &sighandler_SIGCHLD;

	if (0 > sigaction(SIGCHLD, &act, NULL)) {
		trigger_syscall_error("sigaction");
	}

	int child_pid = fork();

	if (0 > child_pid) {
		trigger_syscall_error("fork");
	}

	if ( 0 == child_pid )
	{
		// child process
		fprintf(stdout, "I am child!\n");

		sleep(rand() % 2 + 1);

		fprintf(stdout, "child end point.\n");
		exit( EXIT_SUCCESS );
	}

	fprintf(stdout, "child exited, pid=%i\n", wait_child(child_pid));

	exit( EXIT_SUCCESS );
}
