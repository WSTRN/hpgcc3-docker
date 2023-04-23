//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

// $Id: signal.h 395 2007-03-05 03:47:55Z ingo $

#ifndef __SIGNAL_H
#define __SIGNAL_H


/*
 * Pro forma declarations.
 * HP-GCC does not support signals, but these decls are required by some modules.
 *
 */

typedef void (*sighandler_t)(int);

#define SIGHUP         1
#define SIGINT         2
#define SIGQUIT         3
#define SIGILL         4
#define SIGTRAP         5
#define SIGABRT         6
#define SIGIOT         6
#define SIGBUS         7
#define SIGFPE         8
#define SIGKILL         9
#define SIGUSR1        10
#define SIGSEGV        11
#define SIGUSR2        12
#define SIGPIPE        13
#define SIGALRM        14
#define SIGTERM        15
#define SIGSTKFLT    16
#define SIGCHLD        17
#define SIGCONT        18
#define SIGSTOP        19
#define SIGTSTP        20
#define SIGTTIN        21
#define SIGTTOU        22
#define SIGURG        23
#define SIGXCPU        24
#define SIGXFSZ        25
#define SIGVTALRM    26
#define SIGPROF        27
#define SIGWINCH    28
#define SIGIO        29
#define SIGPOLL        SIGIO
/*
#define SIGLOST        29
*/
#define SIGPWR        30
#define SIGSYS        31


int raise(int);
sighandler_t signal(int sig, sighandler_t handler);

#endif
