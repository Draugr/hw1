#ifndef __FUNCT_H__
#define __FUNCT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define MODE_UNDEFINED 0
#define MODE_STD 1
#define MODE_CHILD 2
#define MODE_POSIX 3
#define MODE_KILL 4
#define MODE_PIPE 5

extern int errno;

void system_call_error(const char *s);

void mode_std();
void mode_child();
void mode_posix(int amount);
void mode_kill(int signal, int pid);
void mode_pipe();

#endif
