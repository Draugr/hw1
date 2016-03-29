#include "funct.h"

void mode_kill_f(int signal, int pid) {
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	fprintf(stdout, "KILL mode ON.\n");

	if ( 0 < pid)
	{
		fprintf(stdout, "Signal %i will send to process with pid=%i.\n", signal, pid);
	} else if ( 0 == pid )
	{
		fprintf(stdout, "Signal %i will send to group of current process with pid=%i.\n", signal, getpid());
	} else if ( -1 == pid )
	{
		fprintf(stdout, "Signal %i will send to all processes of user of current process.\n", signal);
	} else 
		fprintf(stdout, "Signal %i will send to group with pid=%i.\n", signal, abs(pid));

	if (0 > kill(pid, signal))
	{
		system_call_error("kill");
	}

	fprintf(stdout, "KILL mode succesfully complited the task.\n");
	fprintf(stdout, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

	exit( EXIT_SUCCESS );
}
