//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


/*!    \file    ctype.h
    \brief    The C type definitions
*/

#ifndef _CTYPE_H_
#define _CTYPE_H_

// ctype

#define _islower(c) ('a' <= (c) && (c) <= 'z')
#define _isupper(c) ('A' <= (c) && (c) <= 'Z')
#define _isdigit(c) ('0' <= (c) && (c) <= '9')
#define _isalpha(c) (_islower(c) || _isupper(c))
#define _toupper(c) (_islower((c)) ? ((c)-32) : (c))
#define _tolower(c) (_isupper((c)) ? ((c)+32) : (c))



// wrappers for above macros for pointer security
// iblank 2004-10-26

int islower(int c);
int isupper(int c);
int isdigit(int c); 
int isalpha(int c); 
int toupper(int c); 
int tolower(int c); 
int isalnum(int c);



#endif //_CTYPE_H_
