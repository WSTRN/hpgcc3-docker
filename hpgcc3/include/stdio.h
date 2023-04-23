//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

// $Id: stdio.h 514 2007-11-18 03:11:26Z ingo $

#ifndef __HPSTDIO_H
#define __HPSTDIO_H

// LONG TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05
#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif


#include <sys.h>
#include <stdarg.h>

#ifndef _FSYSTEM_H
#include <fsystem.h>
#endif

typedef struct
{
    FS_FILE    *file;
    int cacheused;
    char *writecache;
} FILE;

// open file list handling < ibl ; 2005-02-06 >
/*
typedef struct _open_list_entry {
    FILE *file;
    struct _open_list_entry *next;
} OPEN_LIST_ENTRY;
*/

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

FILE *fopen(const char *fn, const char *mode);

int fclose(FILE *f);

int fgetc(FILE *f);

char * fgets(char *buf, int n, FILE *f);

int fputc(int c, FILE *f);

int fputs(const char *s, FILE *f);

SIZE_T fread(void *buf, SIZE_T size, SIZE_T cnt, FILE *f);

SIZE_T fwrite(const void *buf, SIZE_T size, SIZE_T cnt, FILE *f);


int fseek (FILE *f, long offset, int whence);
#define rewind(f) fseek((f),0L,SEEK_SET)
int feof(FILE *f);


//! RETURN THE CURRENT CALCULATOR MINIFONT
/** USES THE (STABLE) MINI_FONT ENTRY IN RAM (NOT ROM, SO THIS WON'T MOVE)
 * 
 * THIS IS 0x812CF IN SATURN DOMAIN. NOTICE THAT THE FONT IS NOT BYTE-ALIGNED
 * THE ACTUAL FONT STARTS 7 NIBBLES AFTER THIS ADDRESS
 */
void *get_minifont();

// Note: Bigfont is always 8*8 pixels
//       Height is for display purposes only
//       Width is normally 6 pixels
char *get_bigfont();
int get_bigfontheight();


/*!    \fn    int        readint()
    \brief    Read numbers from keyboard
    \retval The appropriate number, read until end of input ,i.e. \\r, or syntax error position.
*/
int        readint();

/*!    \fn    unsigned    readuint()
    \brief    Read numbers from keyboard
    \retval The appropriate number, read until end of input ,i.e. \\r, or syntax error position.
*/
unsigned    readuint();

/*!    \fn    unsigned    readhex()
    \brief    Read numbers from keyboard
    \retval The appropriate number, read until end of input ,i.e. \\r, or syntax error position.
*/
unsigned    readhex();

/*!    \fn    LONGLONG    readlonglong()
    \brief    Read numbers from keyboard
    \retval The appropriate number, read until end of input ,i.e. \\r, or syntax error position.
*/
LONGLONG    readlonglong();

/*!    \fn    ULONGLONG readulonglong()
    \brief    Read numbers from keyboard
    \retval The appropriate number, read until end of input ,i.e. \\r, or syntax error position.
*/
ULONGLONG    readulonglong();

/*!    \fn    double         readdouble()
    \brief    Read numbers from keyboard
    \retval The appropriate number, read until end of input ,i.e. \\r, or syntax error position.
*/
double    readdouble();


/*!    \fn int puts(const char *buf)
    \brief Writes a string pointed to by \a buf to the screen and returns the number of characters written.
    \param buf Pointer to zero delimited string
    \retval Number of characters written
*/
EXTERN int puts(const char *);

/*!    \fn    int putchar(int c)
    \brief    Writes a single character \a c to the screen and returns it's argument.
    \param c Character to be written
    \retval Unchanged argument or -1 if error
    
*/
EXTERN int putchar(int);

/*!    \fn    char    *gets(char *buf)
    \brief    Reads a string from the keyboard.
    \param    buf Pointer to buffer, large enough to hold the string.
    \retval    The argument pointer
*/

// standard file functions

int write(int fd, const char *buf, size_t count);
int isatty(int fd);

#ifdef NO_ASSEMBLY_GETS
EXTERN char    *_gets(char *buf);
#define gets _gets
#else
EXTERN char    *gets(char *buf);
#endif

// console io

/*!    \fn    int    printf(const char *fmt, ...)
    \brief  Stripped down printf surrogate
    Refer to the source for format restrictions.
*/

int printf(const char *fmt, ...);
int sprintf(char *out, const char *fmt, ...);
int vsprintf(char *out,const char *fmt, va_list argp);
int fprintf(FILE *f, const char *fmt, ...);


// Stripped down Integer versions of *printf
// These are lacking floating point formats

int iprintf(const char *fmt, ...);
int isprintf(char *out, const char *fmt, ...);
int ivsprintf(char *out,const char *fmt, va_list argp);
int ifprintf(FILE *f, const char *fmt, ...);


#ifdef TINY_PRINTF
#define printf iprintf
#define sprintf isprintf
#define vsprintf ivsprintf
#define fprintf ifprintf
#endif


// *scanf family

int _vsscanf(const char *str, const char *format, va_list ap);
int _sscanf(const char *s, const char *f, ...);
int _scanf(const char *f, ...);

#define vsscanf _vsscanf
#define sscanf _sscanf
#define scanf _scanf


#ifndef DOXYGEN_SKIP
int get_decimal_digits();
void set_decimal_digits(int v);
char *get_decimal_separator();
void set_decimal_separator(char a);
#endif



#endif // __HPSTDIO_H
