//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


#ifndef __SYSTEM_GLOBALS_H
#define __SYSTEM_GLOBALS_H

#ifndef __FS_PRIV_H
#include <fsyspriv.h>
#endif

typedef struct {

// DO NOT MOVE OR CHANGE ORDER
unsigned char exception_screen[1616];
unsigned int exception_stack[256];
unsigned int exception_save[9];
// TABLE OF INTERRUPT HANDLERS
unsigned int irq_table[32];
FS_PUBLIC FSystem;


} SYSTEM_GLOBALS ;


extern SYSTEM_GLOBALS *__sg;



#endif


