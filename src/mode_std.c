#include "funct.h"

void sighandler(int signum, siginfo_t *siginfo, void *data) {
	fprintf(stdout, "%s received: signum=%i, sender pid=%i, gid=%i\n",
		strsignal(signum), signum, siginfo->si_pid, getpgid(siginfo->si_pid));
}

void do_std_mode() {
	fprintf(stdout, "Entering STD mode.\n");

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &sighandler;

	if (0 > sigaction(SIGUSR1, &act, NULL)) {
		trigger_syscall_error("sigaction");
	}

	if (0 > sigaction(SIGUSR2, &act, NULL)) {
		trigger_syscall_error("sigaction");
	}

	if (0 > sigaction(SIGHUP, &act, NULL)) {
		trigger_syscall_error("sigaction");
	}

	// test
	raise(SIGUSR1);
	raise(SIGUSR2);
	raise(SIGHUP);
	//

	exit( EXIT_SUCCESS );
}
