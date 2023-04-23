//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


#ifndef SETJMP_H_
#define SETJMP_H_

typedef unsigned int jmp_buf[14]; 

int setjmp(jmp_buf env);
void longjmp(jmp_buf env,int val);


#endif /*SETJMP_H_*/
