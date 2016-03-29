#include "funct.h"
#include <getopt.h>

void system_call_error(const char *s)
{
	perror(s);
	fprintf(stderr, "Errno is: %d\n", errno);
	exit( EXIT_FAILURE );
}

void usage() {
	fprintf(stdout,  "Usage: lab -m MODE [-a AMOUNT] [-s SIGNAL] [-p PID]\n");
}

int main(int argc, char *argv[]) {
	
	int mode = MODE_UNDEFINED;
	int amount = 0;
	int signal = 0;
	int pid = 0;

	static struct option long_opts[] = {
		{"mode", required_argument, NULL, 0},
		{"amount", required_argument, NULL, 0},
		{"signal", required_argument, NULL, 0},
		{"pid", required_argument, NULL, 0},
		{NULL, 0, NULL, 0 }
	};

	int opt_index;
	int opt = getopt_long( argc, argv, "", long_opts, &opt_index );

	while (-1 != opt){
		if ( 0 == strcmp( "mode", long_opts[opt_index].name )){
			if (!strcasecmp(optarg, "std")) {
				mode = MODE_STD;
			} else if (!strcasecmp(optarg, "child")) {
				mode = MODE_CHILD;
			} else if (!strcasecmp(optarg, "kill")) {
				mode = MODE_KILL;
			} else if (!strcasecmp(optarg, "posix")) {
				mode = MODE_POSIX;
			} else if (!strcasecmp(optarg, "pipe")) {
				mode = MODE_PIPE;
			}
		}
		if ( 0 == strcmp( "amount", long_opts[opt_index].name )){
			amount = atoi(optarg);
		}
		if ( 0 == strcmp( "signal", long_opts[opt_index].name )){
			signal = atoi(optarg);
		}
		if ( 0 == strcmp( "pid", long_opts[opt_index].name )){
			pid = atoi(optarg);
		}
		opt = getopt_long( argc, argv, "", long_opts, &opt_index );
	}


	fprintf(stdout, "Current pid now is: %i\n", getpid());

	srand(time(NULL));

	if (MODE_UNDEFINED == mode)
	{
		usage();
		return EXIT_FAILURE;

	} else if (MODE_STD == mode)
	{
		mode_std_f();
	} else if (MODE_CHILD == mode)
	{
		mode_child_f();
	} else if (MODE_POSIX == mode)
	{
		if ( 0 >= amount )
		{
			fprintf(stderr, "--amount must be > 0\n");
			return EXIT_FAILURE;
		}
		mode_posix_f(amount);
	} else if (MODE_KILL == mode)
	{
		if ( 0 >= signal )
		{
			fprintf(stderr, "--signal must be specified\n");
			return EXIT_FAILURE;
		}
		mode_kill_f(signal, pid);
	} else if (MODE_PIPE == mode)
	{
		mode_pipe_f();
	}

	return EXIT_SUCCESS;
}
