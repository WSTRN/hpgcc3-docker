//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

// $Id: string.h 514 2007-11-18 03:11:26Z ingo $


/*!    \file    string.h
    \brief    Standard String functions
*/

#ifndef _HPSTRING_H
#define _HPSTRING_H

// LONG TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05
#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif


#include <ctype.h>
#include <kos.h>


//!    Length of s
int    strlen(const char *s);

//!    Copy source to destination
char    *strcpy(char *dest, const char *src);

//!    Copy n chars of src in dest
char     *strncpy(char *dest, const char *src, size_t n);

//!    Append source after enf of destination.  */
char    *strcat(char *dest, const char *src);

//!    Compare s1 and s2 (<0 ==0 or >0)
int    strcmp(register const char *s1, register const char *s2);

//!    Compare n chars of s1 and s2 (<0 ==0 or >0)
int strncmp ( const char *s1, const char *s2, int num);

//!    Convert int to a string with specific base
//char     *itoa(int z, char * buff, int base);
#define itoa(z,buff,base) lltoa((long long)z,buff,base)


//!    \fn    char     *utoa(unsigned z, char * buff, int base)
//char     *utoa(unsigned z, char * buff, int base);
#define utoa(z,buff,base) ulltoa((unsigned long long)z,buff,base)

//!    \fn    char    *lltoa(LONGLONG, char *,int base)
char    *lltoa(LONGLONG, char *,int base);

//!    \fn     char    *ulltoa(ULONGLONG, char *, int base)
char    *ulltoa(ULONGLONG, char *, int base);

/*!    \fn    char     *upper(char *t,char *s)
    \brief    Convert string to uppercase
    \param    t pointer to target
    \param    s pointer to source
    \retval    pointer to target
*/
char     *upper(char *t, const char *s);
#define strupr(str) upper(str,str)

/*!    \fn    char     *lower(char *t,char *s)
    \brief    Convert string to uppercase
    \param    t pointer to target
    \param    s pointer to source
    \retval    pointer to target

*/

/*!
    \fn    char *strset(char *s, int fill)
    \brief    Set a string to a constant char \b fill or random in ['a'..'z'] if fill is zero.
    \param    s pointer to string to be set
    \param    fill the value to fill in, zero means random 
    \retval pointer to string s
*/

char *strset(char *s, int fill);

char *str_unquote(char *s, char c);
char *strdup(const char *s);

char     *lower(char *t, const char *s);

#ifndef DOXYGEN_SKIP


char *__dtoa(double x, char *buf, char exp_sym);

char     *xpad(char *t, char *s, int n, int left, char c);

// REMOVED BY CLAUDIO - MAR 2006
//int        atoi_base(char *, int base);
//unsigned    atoui_base(char *, int base);
LONGLONG    atoll_base(const char *, int base);
//ULONGLONG    atoull_base(char *, int base);

#define atoi(str) ((int)atoll_base(str,10))
#define atoi_base(str,base) ((int)atoll_base(str,base))
#define atoui(str) ((unsigned int)atoull_base(str,10))
#define atoui_base(str,base) ((unsigned int)atoull_base(str,base))

#define atoll(str) atoll_base(str,10)
#define atoll_base(str,base) atoll_base(str,base)
#define atoull(str) ((unsigned long long)atoll_base(str,10))
#define atoull_base(str,base) ((unsigned long long)atoll_base(str,base))



#endif

// REMOVED BY CLAUDIO - MAR 2006
/*
int        atoi(char *);
unsigned    atoui(char *);
LONGLONG     atoll(char *);
ULONGLONG    atoull(char *);
*/

// ADDED BY CLAUDIO - MAR 2006
// LOW-LEVEL UNSIGNED TO STRING CONVERSION LOOP
char *_str2ull(const char *number,unsigned long long *result,int base);




/* -*- OBSOLETE -*-
// < ibl ; 2006-04-05 >

char *
ints(int z, int base);

char *
uints(unsigned z, int base);
*/


double        atof(const char *);

void* memcpy(void *out, const void *in, int n);
void memcpyw(void *dest,const void *source,int nwords);
void * memset (char * dest, int C, int LENGTH);
void memsetw(void *dest,int value, int nwords);
void *memmove(void *_dest, const void *_source, size_t nbytes);
//Additional string functions
//Added by Al Borowski, 19th Nov, 2004
//TODO: Add proper documentation?

char * strchr(const char *s, int c);
char * strncat(char *s1, const char * s2, size_t n);
char *strrchr(const char *s, int c);
size_t strcspn(const char *s1, const char *s2);
char *strpbrk(const char *s1, const char *s2);
char *strtok(char *s1, const char *s2);
char *strstr(const char *s1, const char *s2);
size_t strspn(const char *s1, const char *s2);


#endif 
