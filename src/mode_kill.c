#include "funct.h"

void do_kill_mode(int signal, int pid) {
	fprintf(stdout, "Entering KILL mode.\n");

	if ( 0 < signal)
	{
		fprintf(stdout, "Signal %i will send to process with pid=%i.\n", signal, pid);
	} else if ( 0 == signal )
	{
		fprintf(stdout, "Signal %i will send to group of current process with pid=%i.\n", signal, getpid());
	} else if ( -1 == signal )
	{
		fprintf(stdout, "Signal %i will send to all processes of user of current process.\n", signal);
	} else 
		fprintf(stdout, "Signal %i will send to group with pid=%i.\n", signal, abs(pid));

	if (0 > kill(pid, signal))
	{
		trigger_syscall_error("kill");
	}

	exit( EXIT_SUCCESS );
}
