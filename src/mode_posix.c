#include "funct.h"

int out_signals[256][5];
int in_signals[256][5];
int in_signal_num = 0;
int it = 0;

void chld_handle(int signum, siginfo_t *siginfo, void *data){
	int i;

	for (i = 0; i < in_signal_num; i++)
	{
		fprintf(stdout, "N=%i | MYPID=%i | PPID=%i | SIGNAL No=%i | VALUE=%i\n", in_signals[i][0], in_signals[i][1], in_signals[i][2], in_signals[i][3], in_signals[i][4]);
	}
}

void posix_handle(int signum, siginfo_t *siginfo, void *data){
	sigval_t sigval = siginfo->si_value;

	in_signals[in_signal_num][0] = in_signal_num + 1;
	in_signals[in_signal_num][1] = getpid();
	in_signals[in_signal_num][2] = getppid();
	in_signals[in_signal_num][3] = signum;
	in_signals[in_signal_num][4] = sigval.sival_int;
	in_signal_num++;
}

void mode_posix_f (int amount) {
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	fprintf(stdout, "POSIX mode ON.\n");

	int i;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;

	// SIGCHLD
	act.sa_sigaction = &chld_handle;
	if (0 > sigaction(SIGCHLD, &act, NULL)) {
		system_call_error("sigaction");
	}

	// POSIX signals
	act.sa_sigaction = &posix_handle;
	for (i = SIGRTMIN; i <= SIGRTMAX; i++) {
		if (0 > sigaction(i, &act, NULL)) {
			system_call_error("sigaction");
		}
	}

	sigval_t sigval;
	int child_pid = fork();

	if (0 > child_pid) {
		system_call_error("fork");

	} else if ( 0 == child_pid )
	{
		int mypid = getpid(),
			ppid = getppid();

		// child process
		fprintf(stdout, "Child is working...\n");

		for (i = 0; i < amount; i++)
		{
			int rand_signum = 0,
				rand_value = rand();

			rand_signum = rand() % (SIGRTMAX - SIGRTMIN + 1) + SIGRTMIN;

			out_signals[i][0] = i + 1;
			out_signals[i][1] = mypid;
			out_signals[i][2] = ppid;
			out_signals[i][3] = rand_signum;
			out_signals[i][4] = rand_value;

			sigval.sival_int = rand_value;

			if (0 > sigqueue(ppid, rand_signum, sigval)) {
				system_call_error("sigqueue");
			}

		}

		// sended signal table
		for (i = 0; i < amount; i++)
		{
			fprintf(stdout, "N=%i | MYPID=%i | PPID=%i | SIGNAL_No=%i | VALUE=%i\n", out_signals[i][0], out_signals[i][1], out_signals[i][2], out_signals[i][3], out_signals[i][4]);
		}

		sleep(1);

		fprintf(stdout, "Child ending work.\n");
		exit( EXIT_SUCCESS );
	}
	int status;
	while( 0 > wait(&status) ){
		if ( errno == EINTR ){
			continue;
		}
		system_call_error("wait");
	}

	fprintf(stdout, "Child exited.\n");
	fprintf(stdout, "POSIX mode succesfully completed the task.\n");
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

	exit( EXIT_SUCCESS );
}
