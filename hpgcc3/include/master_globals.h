//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


#ifndef __MASTER_GLOBALS_H
#define __MASTER_GLOBALS_H

// $Id: master_globals.h 514 2007-11-18 03:11:26Z ingo $

#include <sys.h>
#include <stdio.h>
#include <conio.h>
#include <sat3.h>
#include <keyb.h>

#ifndef __HPGRAPHICS_TYPE_DEFINED
#define __HPGRAPHICS_TYPE_DEFINED 1
// TYPE DEFINITIONS NEEDED FOR HPG
struct hpg_graphics;
typedef struct hpg_graphics hpg_t;
#endif


typedef struct {
    
// *****************************
// STRING FUNCTIONS STATE
// *****************************

// strtok()
char * __strtok_save;
// < ibl ; 2006-11-30 >


// *****************************
// MATH FUNCTIONS STATE
// *****************************

// < ibl ; 2006-04-03 >

// gauss()
int __gauss_have_next_gaussian;
double __gauss_next_gaussian;

// *****************************
// RANDOM NUMBER GENERATOR
// *****************************

// < ibl ; 2006-04-03 >
unsigned long long __mwc_w;




// *****************************
// STDIO AND SCREEN-RELATED GLOBALS
// *****************************

unsigned int __console_type;	// TYPE OF CONSOLE TO USE 0=NO CONSOLE (/dev/null) 1=OLD-STYLE, 2=GUI CONSOLE
_CONSOLE_GLOBALS *__console_vars;	// OLD-STYLE CONSOLE EXTERNAL VARIABLES
void *ConsPtr;	// NEW CONSOLE OBJECT - USE (void *) TO AVOID DECLARING THE CLASS HERE


// *****************************
// HEAP MANAGEMENT
// *****************************


char *__heap_ptr;
void *_malloc_bottom , *_malloc_top , *_malloc_empty;

// *****************************
// INTERRUPT STATE
// *****************************


unsigned int __saveint;


// *****************************
// EXIT AND RELATED GLOBALS
// *****************************


unsigned int *__exit_stk_state;

unsigned int errno;

ATEXIT_ENTRY *__at_exit_vector;
int _exit_lock;

//OPEN_LIST_ENTRY *__open_file_list;

// *****************************
// VARIABLES USED BY LOADER/CRT0
// *****************************

unsigned int    _heap_base_addr;
unsigned int _mmu_table_entries;
SAT_CPU *_saturn_cpu;


// *********************************
// VARIABLES USED BY THE HPG LIBRARY
// *********************************

//hpg_t main_hpg;
int inited;
hpg_t *hpg_stdscreen;

// *************************************
// VARIABLES USED BY THE TIMER MACHINERY
// *************************************

volatile ULONGLONG __systmr;
volatile ULONGLONG __evtmr;
ULONGLONG __sysfreq;
timed_event tmr_events[NUM_EVENTS];


// ****************************************
// VARIABLES USED BY THE KEYBOARD MACHINERY
// ****************************************

unsigned short int __keyb_buffer[KEYB_BUFFER];
int __keyflags;
int __kused,__kcurrent;
keymatrix __kmat;
int __keyplane;
int __keynumber,__keycount;
int __keyb_repeattime,__keyb_longpresstime,__keyb_debounce;

} MASTER_GLOBALS ;


extern MASTER_GLOBALS *__mg;



#endif

