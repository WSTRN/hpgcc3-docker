//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***



/*!    \file conio.h
    \brief Header file for HP49 console I/O
*/

#ifndef _HPCONIO_H
#define _HPCONIO_H

//#include <stddef.h>
#include <stdlib.h>

// LONG TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05
#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif


/*!    \fn int puts(const char *buf)
    \brief Writes a string pointed to by \a buf to the screen and returns the number of characters written.
    \param buf Pointer to zero delimited string
    \retval Number of characters written
*/


/*!    \fn    int putchar(int c)
    \brief    Writes a single character \a c to the screen and returns it's argument.
    \param c Character to be written
    \retval Unchanged argument or -1 if error
    
*/

/*!    \fn    void home();
    \brief    Positions the virtual write mark at screen position (0,0), i.e. 'home'
*/

/*!    \fn    int getx()
    \brief    Returns the current write mark's x-component.
    \retval    Current screen position x
*/

/*!    \fn    int gety()
    \brief    Returns the current write mark's y-component.
    \retval    Current screen position y
*/

/*!    \fn    void clreol()
    \brief    Clear to End-Of-Line from current write mark position.
*/
    
/*!    \fn    void clear_screen()
    \brief    Clears the entire screen
*/

 
/*!    \fn    void gotoxy(int x,int y)
    \brief    Position write mark
    \param    x x-position to go to
    \param     y y-position to go to
*/

/*!    \fn    int get_tabsize()
    \brief    Returns current tab width in character units.
    \retval    Tab width
*/

/*!    \fn    void set_tabsize(int width)
    \brief    Sets current tab width in character units.
    \param    width New tab width
*/

/*!    \fn int getchar()
    \brief Reads a character from keyboard
    \retval ASCII code of entered character
*/



/*!    \fn    char    *gets(char *buf)
    \brief    Reads a string from the keyboard.
    \param    buf Pointer to buffer, large enough to hold the string.
    \retval    The argument pointer
*/



typedef struct {
	int  (*putchar)		(int);
	int  (*puts)		(const char *);
	int  (*puts_nonl)	(const char *);
	int  (*getchar)		(void);
	char *(*gets)		(char *);
	void (*clreol)		(void);
	void (*clear_screen)(void);
	void (*gotoxy)		(int,int);
	void (*wherexy)		(int *,int *);
} _CONSOLE;

extern const _CONSOLE __console[3];


typedef struct {
int __scr_h , __scr_w ;
int __pos_x , __pos_y , __ch_w , __ch_h ;
char *__decimal_separator;
int __decimal_digits;
int __auto_cr;
unsigned int    __display_buf;
} _CONSOLE_GLOBALS;



extern int putchar(int);
extern int puts(const char *);
extern int puts_nonl(const char *);
extern int getchar();
extern char *gets(char *buf);
extern void clreol();
extern void clear_screen();
extern void gotoxy(int,int);
extern void wherexy(int *x,int *y);


extern int dummy_putchar(int);
extern int dummy_puts(const char *);
extern int dummy_puts_nonl(const char *);
extern int dummy_getchar();
extern char *dummy_gets(char *buf);
extern void dummy_clreol();
extern void dummy_clear_screen();
extern void dummy_gotoxy(int,int);
extern void dummy_wherexy(int *x,int *y);

extern int putchar_orig(int);
extern int puts_orig(const char *);
extern int puts_nonl_orig(const char *);
extern int getchar_orig();
extern char *gets_orig(char *buf);
extern void clreol_orig();
extern void clear_screen_orig();
extern void gotoxy_orig(int,int);
extern void wherexy_orig(int *x,int *y);

// console io 

extern void home();
extern int getx();
extern int gety();
extern int screen_width();
extern int screen_height();
extern int get_tabsize();
extern void set_tabsize(int);

int get_decimal_digits();
void set_decimal_digits(int v);
char *get_decimal_separator();
void set_decimal_separator(char a);

extern void ShutDownConsole();
extern void InitConsole();
extern char * getsConsole(char *str);
extern int getcharConsole();
extern int putsConsole(const char *text);
extern int puts_nonlConsole(const char *text);
extern int putcharConsole(char c);
extern void clearConsole();
extern void clreolConsole();
extern void gotoxyConsole(int x,int y);
extern void wherexyConsole(int *x,int *y);




#endif 

