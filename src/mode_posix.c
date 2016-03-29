#include "funct.h"

int sent_signal_table[256][5];
int received_signal_table[256][5];
int received_signal_index = 0;

void sighandler_posix(int signum, siginfo_t *siginfo, void *data) {
	int i;
	
	if ( signum == SIGCHLD )
	{
		// SIGCHLD received, print sent_signal_table

		fprintf(stdout, "SIGCHLD received, processed signals that sent by child :\n");

		// print sent signal table
		for (i = 0; i < received_signal_index; i++)
		{
			int *row = received_signal_table[i];

			fprintf(stdout, "%i | MYPID: %i | PARENTPID: %i | RECEIVEDSIGNAL: %i | VAL: %i\n", row[0], row[1], row[2], row[3], row[4]);
		}

	} else {

		sigval_t sigval = siginfo->si_value;

		int *row = received_signal_table[received_signal_index++];
		row[0] = received_signal_index;
		row[1] = getpid();
		row[2] = getppid();
		row[3] = signum;
		row[4] = sigval.sival_int;
	}
}


void do_posix_mode (int amount) {
	fprintf(stdout, "Entering POSIX mode.\n");

	int i;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &sighandler_posix;

	// set handlers to SIGCHLD
	if (0 > sigaction(SIGCHLD, &act, NULL)) {
		trigger_syscall_error("sigaction");
	}

	// set handlers to all POSIX signals in
	act.sa_sigaction = &sighandler_posix;
	for (i = SIGRTMIN; i <= SIGRTMAX; i++) {
		if (0 > sigaction(i, &act, NULL)) {
			trigger_syscall_error("sigaction");
		}
	}

	// fork current process
	sigval_t sigval;
	int child_pid = fork();

	if (0 > child_pid) {
		trigger_syscall_error("fork");

	} else if ( 0 == child_pid )
	{
		int mypid = getpid(),
			ppid = getppid();

		// child process
		fprintf(stdout, "I am child!\n");

		for (i = 0; i < amount; i++)
		{
			int randSignum = 0,
				randValue = rand();

			randSignum = rand_range(SIGRTMIN, SIGRTMAX);

			sent_signal_table[i][0] = i + 1;
			sent_signal_table[i][1] = mypid;
			sent_signal_table[i][2] = ppid;
			sent_signal_table[i][3] = randSignum;
			sent_signal_table[i][4] = randValue;

			sigval.sival_int = randValue;

			if (0 > sigqueue(ppid, randSignum, sigval)) {
				trigger_syscall_error("sigqueue");
			}

		}

		// print sent signal table
		for (i = 0; i < amount; i++)
		{
			// 1|MYPID|PARENTPID|RANDOMPOSIXSIGNALSENTNO|RANDOMVALUE

			fprintf(stdout, "%i | MYPID: %i | PARENTPID: %i | SENTSIGNAL: %i | VAL: %i\n",
				sent_signal_table[i][0], sent_signal_table[i][1], sent_signal_table[i][2],
				sent_signal_table[i][3], sent_signal_table[i][4]);
		}

		sleep(1);

		fprintf(stdout, "child end point.\n");
		exit( EXIT_SUCCESS );
	}

	fprintf(stdout, "child exited, pid=%i\n", wait_child(child_pid));

	exit( EXIT_SUCCESS );
}
