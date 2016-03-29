#include "funct.h"

void signal_handle(int signum, siginfo_t *siginfo, void *data) {
	fprintf(stdout, "Signal %s (%i) has received, sended from pid=%i, gid=%i\n", strsignal(signum), signum, siginfo->si_pid, getpgid(siginfo->si_pid));
}

void mode_std_f() {
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	fprintf(stdout, "STD mode ON.\n");

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &signal_handle;

	if (0 > sigaction(SIGUSR1, &act, NULL)) {
		system_call_error("sigaction");
	}

	if (0 > sigaction(SIGUSR2, &act, NULL)) {
		system_call_error("sigaction");
	}

	if (0 > sigaction(SIGHUP, &act, NULL)) {
		system_call_error("sigaction");
	}

	sleep(1);
	raise(SIGUSR1);
	sleep(1);
	raise(SIGUSR2);
	sleep(1);
	raise(SIGHUP);

	fprintf(stdout, "STD mode succesfully complited the task.\n");
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

	exit( EXIT_SUCCESS );
}
