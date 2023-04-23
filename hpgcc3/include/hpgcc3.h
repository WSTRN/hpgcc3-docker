//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


/*! \mainpage HPGCC Standard Library
 *
 * The HPGCC standard library contains basic code for writing applications to
 * run on the calculator.  It consists of all of the following:
 *
 * - A simplified version of most parts of the C language standard library.
 * - Routines for interacting with the calculator hardware (interrupts,
 *   timers, buzzer, LCD controller, etc.)
 * - Low-level routines for reading from the calculator keyboard.
 * - Very simple screen graphics routines.
 * - An exposed interface to the underlying KOS operating system calls (for
 *   advanced users only)
 * - Routines to interact with data from the calculator stack, as well as
 *   other aspects of the emulated Saturn environment.
 */

/*!
 * \file  hpgcc3.h
 * \brief Umbrella header file for the HPGCC standard library.
 *
 * This header file can be used to include the entirety of the HPGCC standard
 * library.  It defines no unique interfaces of its own, and is provided
 * merely as a convenience.
 */
#ifndef _HPGCC3_ALL
#define _HPGCC3_ALL

#ifdef __cplusplus
extern "C" {
#endif


//includes everything for the HP49g+

// < ibl ; 2005-09-06 >

// 64-BIT TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05

#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif

#include <hpgcc_config.h>
#include <conio.h>
#include <ctype.h>
#include <keyb.h>
#include <math.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys.h>
#include <time.h>
#include <kos.h>
#include <syscall.h>
#include <extreal.h>
#include <sat3.h>
#include <signal.h>

#ifdef __cplusplus
}
#endif

#endif
