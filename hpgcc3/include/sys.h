//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***



/*!    \file sys.h
    \brief Header file for system functions
*/

#ifndef _HPSYS_H
#define _HPSYS_H

// LONG TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05
#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif


#include <kos.h>

#ifndef EXTERN
#define EXTERN extern
#endif

#ifndef NULL
#define NULL 0
#endif


//! Type definition for interrupt handler functions

typedef void (*__interrupt__)(void);

// atexit machinery added by ibl 2005-01-31

typedef void (*ATEXITFUNC)();

typedef struct _atexit_entry {
    ATEXITFUNC f;
    struct _atexit_entry *next;
} ATEXIT_ENTRY;

int atexit(ATEXITFUNC f);

/* -*- OBSOLETE -*-
// < ibl ; 2006-04-06 >

// Convert a BCD format number to an hexa value
// char  *sys_bcd2str(unsigned int BCD);
*/

//! Convert a BCD format BYTE (least significant 2 digits) to an integer value
#define sys_bcd2bin(BCD) sys_bcd2bin_nibbles(BCD,2)
//! Convert the least significant n (=nibbles, max. 8) digits of a BCD formatted number to it's integer value 
unsigned int sys_bcd2bin_nibbles(unsigned int  BCD,int nibbles);





// arithmetics

 typedef struct
 {
 LONGLONG quot;
 LONGLONG rem;
 } lldiv_t;

 lldiv_t lldiv(LONGLONG,LONGLONG);

/*!    \fn    unsigned divlu2(unsigned num_h,unsigned num_l,unsigned den,unsigned *rem)
    \brief    Unsigned 64/32 bit division
*/

/*!    \fn    ULONGLONG udiv64(ULONGLONG nom, unsigned den, unsigned *rem)
    \brief    Unsigned 64/32 bit division with 64 bit quotient.
*/

/*!    \fn    LONGLONG sdiv64(LONGLONG nom, int den, int *rem)
    \brief    Signed 64/32 bit division with 64 bit quotient.
*/



unsigned divlu2(unsigned num_h,unsigned num_l,unsigned den,unsigned *rem);

ULONGLONG
udiv64(ULONGLONG nom, unsigned den, unsigned *rem);

LONGLONG
sdiv64(LONGLONG nom, int den, int *rem);


// loader globals

//! Value of r1 register at the begining of execution
/**
 * See post of Robert Hildinger:
 * "The R1 register contains the base address for all the ARM
 * globals, which can be used to access all of the Saturn registers.
 * Altering this register will not affect the calling code. The LR
 * register contains the return address to get back into Saturn
 * emulation."
 *
 * API INFORMATION\n
 * ---------------
 *
 * So far there is no real API information available, although
 * experimentally I've been able to determine the following offsets from
 * the global base register for accessing the emulated Saturn CPU
 * registers:\n
 * 
 * Base (R1) offset      Description\n
 * ---------------------------------------
 *
 * 0x90C                 Saturn register A (low order 8 nibbles)\n
 * 0x910                 Saturn register A (high order 8 nibbles)\n 
 * 0x914                 Saturn register B (low order 8 nibbles)\n 
 * 0x918                 Saturn register B (high order 8 nibbles)\n 
 * 0x91C                 Saturn register C (low order 8 nibbles)\n 
 * 0x920                 Saturn register C (high order 8 nibbles)\n 
 * 0x924                 Saturn register D (low order 8 nibbles)\n 
 * 0x928                 Saturn register D (high order 8 nibbles)\n 
 * \n
 * 0x92C                 Saturn register R0 (low order 8 nibbles)\n 
 * 0x930                 Saturn register R0 (high order 8 nibbles)\n 
 * 0x934                 Saturn register R1 (low order 8 nibbles)\n 
 * 0x938                 Saturn register R1 (high order 8 nibbles)\n 
 * 0x93C                 Saturn register R2 (low order 8 nibbles)\n 
 * 0x940                 Saturn register R2 (high order 8 nibbles)\n 
 * 0x944                 Saturn register R3 (low order 8 nibbles)\n 
 * 0x948                 Saturn register R3 (high order 8 nibbles)\n 
 * 0x94C                 Saturn register R4 (low order 8 nibbles)\n 
 * 0x950                 Saturn register R4 (high order 8 nibbles)\n 
 * \n
 * 0x954                 Saturn register d0\n
 * 0x958                 Saturn register d1\n
 * 0x95C                 Saturn register P\n
 * 0x968                 Saturn register ST\n
 * 0x96C                 Saturn register HST\n
 * 0x970                 Saturn CARRY flag\n
 * 0x974                 Saturn DECIMAL_MODE flag\n
 * 
 *
 */
 


// GLOBALS CONVERTED TO CONSTANTS

#define _mmu_table_addr 0x01100000
#define _code_base_addr 0x01220000
#define _ram_base_addr  0x01101000
#define _ram_size   ((__mg->_mmu_table_entries-4)<<10)


// HARDWARE CONSTANTS
#define MEM_REGS	0x07000000
#define SD_REGS		0x07e00000
#define IO_REGS 	0x07a00000
#define LCD_REGS    0x07300000
#define CLK_REGS    0x07200000
#define RTC_REGS    0x07B00000 // < ibl ; 2005-06-23 >
#define TMR_REGS	0x07600000
#define INT_REGS	0x07100000


// CLOCK MODE CONSTANTS
#define CLK_1MHZ 0x16000000
#define CLK_6MHZ 0x11000000
#define CLK_12MHZ 0x10000000
#define CLK_48MHZ 0x78023
#define CLK_75MHZ 0x43012
#define CLK_120MHZ 0x5c080
#define CLK_152MHZ 0x44011
#define CLK_192MHZ 0x58011




//ARM interrupts
void cpu_intoff(); 
void cpu_inton();
//! Save interrupt controller state to a buffer (11 words)
void cpu_intsave(unsigned int *buffer);
//! Restore interrupt controller state from a buffer (11 words)
void cpu_intrestore(unsigned int *buffer);

// power management routines
//void sys_slowOn(); 
//void sys_slowOff();
//int sys_is_slowOn();

// CPU CLOCK SAVE/RESTORE
//! Save CPU clock state to a buffer (8-words)
void cpu_clksave(unsigned int *buf);
//! Restore CPU clock from a buffer (8-words)
void cpu_clkrestore(unsigned int *buf);
//! Set CPU speed in Hz, change to closest supported speed and return new frequency.
int cpu_setspeed(int hz);
//! Get CPU speed in Hz.
int cpu_getspeed();
//! Low-level cpu speed control
int __cpu_setspeed(int PLLCON);
//! Get CPU clock speed in Hz. Same as cpu_getspeed.
int __cpu_getFCLK();
//! Get peripherals clock (HCLK) speed in Hz.
int __cpu_getHCLK();
//! Get peripherals clock (PCLK) speed in Hz.
int __cpu_getPCLK();
//! Flush write-back caches on the specified block of memory
void cpu_flush_cache(int *ptr,int size);

// *********************************
//    TIMER MACHINERY DEFINITIONS
// *********************************

#define NUM_EVENTS  5   // NUMBER OF SIMULTANEOUS TIMED EVENTS

// INTERNAL USE ONLY
typedef struct {
__interrupt__ eventhandler;
ULONGLONG ticks;
unsigned int  delay;
unsigned int status; // 1=ACTIVE, 2=AUTORELOAD
} timed_event;

//! Handle to refer to a timed event.
typedef int HEVENT;

//! Save all timers state to a buffer (13-word)
void tmr_save(unsigned int *tmrbuffer);
//! Restore saved timers state from buffer
void tmr_restore(unsigned int *tmrbuffer);
//! Setup system timers and event scheduler - automatically called at startup
void tmr_setup();

//! Type to use with all timer routines: ticks and frequencies are 64-bit
typedef ULONGLONG tmr_t;

//! Get the frequency of the system timer in ticks per second, normally 100 KHz or 125KHz
tmr_t tmr_getsysfreq();

//! Get system timer count in ticks since program started.
tmr_t tmr_ticks();
//! Do not return until the specified time has passed
void tmr_delayms(int milliseconds);
//! Do not return until the specified time has passed (in microseconds). Accuracy is +/- 10 microseconds.
void tmr_delayus(int microseconds);

//! Calculate elapsed time in milliseconds between before and after (both given in ticks)
int tmr_ticks2ms(tmr_t before, tmr_t after);
//! Calculate elapsed time in microseconds between before and after (both given in ticks)
int tmr_ticks2us(tmr_t before, tmr_t after);
//! Add/subtract an interval in milliseconds to the given time in ticks
tmr_t tmr_addms(tmr_t time, int ms);
//! Add/subtract an interval in microseconds to the given time in ticks
tmr_t tmr_addus(tmr_t time, int us);
//! Wait until the system timer reaches the given time in ticks
void tmr_waituntil(tmr_t time);

//! Macro to convert milliseconds to ticks
#define tmr_ms2ticks(a) tmr_addms((tmr_t)0,(a))

//! Macro to convert milliseconds to ticks
#define tmr_us2ticks(a) tmr_addus((tmr_t)0,(a))



/*!
    \brief Create a timed event. 
    Create a new timed event, specifying a callback function that will be called after
    the given time has passed. The autorepeat feature allows the event to be automatically
    rescheduled for another interval of time. Autorepeated events need to be manually removed
    by tmr_eventkill. One-shot events will remove themselves after completion (no need to explicitly
    call tmr_eventkill).
    
    \param handler  The function that will be called back on every interval
    \param ms       Time interval in milliseconds after which the handler will be called
    \param autorepeat If TRUE, the event will be repeated every 'ms' milliseconds, if FALSE
                      the event will be executed only once after 'ms' milliseconds have passed
                      since the event was created.
                      
    \return An event handler, or -1 if there are no more handles available (see notes).                   
    
    \note A maximum of NUM_EVENTS (typically 5) can be created simultaneously. 

 \sa tmr_eventkill
*/

HEVENT tmr_eventcreate(__interrupt__ handler,unsigned int ms,BOOL autorepeat);

/*!
    \brief Kill a timed event. 
    Stops an autoreloading event.
    
    \param event The event handler obtained from tmr_eventcreate

 \sa tmr_eventcreate
*/

void tmr_eventkill(HEVENT event);



/*
// REMOVED BY CLAUDIO - FUNCTIONALITY IS NOW OBSOLETE
typedef struct
{
    ULONGLONG current;
    unsigned last_val : 16;
    unsigned timer : 3;
    unsigned inited : 1;
} sys_timer_t;

#define SYS_TIMER_INITIALIZER { 0, 0, 0, 0 }
#define SYS_TIMER_INITIALIZER_NUM(n) { 0, 0, n, 0 }

ULONGLONG sys_updateTimer(sys_timer_t *timer);




void sys_sleep(unsigned int millis);

//! numers of ticks at 750kHz to wait for
void sys_waitTicks(unsigned int time); 
void sys_setupTimers();
void sys_restoreTimers();

//! Get PWM counter 0
unsigned short int sys_getPWMCounter0();
*/


//! Get date in seconds from RTC
/**    
 *    \return    The current time in seconds
 *
 *    The total number of seconds 3600*H+60*M+S.
 */
int sys_RTC_seconds();



// VIDEO MODE CONSTANTS
#define MODE_MONO 0
#define MODE_4GRAY 1
#define MODE_16GRAY 2

//! Wait for LCD synchro
void lcd_sync();
//! Fix LCD clock rate at any CPU speed
void __lcd_fix();
//! Save LCD controller state (buffer size must be 17 words min.)
void lcd_save(unsigned int *buffer);
//! Restore LCD controller state from buffer (saved by lcd_save)
void lcd_restore(unsigned int *buffer);
/*!    \fn    int sys_lcdsetmode(int mode,int *physicalbuffer)
    \brief    Sets the corresponding video mode. Returns scanline width in bytes.
    \param    mode = MODE_MONO (=0), MODE_4GRAY (=1) or MODE_16GRAY (=2)
    \param    physicalbuffer = Video frame memory, pointing to physical address
*/
int lcd_setmode(int mode,int *physicalbuffer);
// ! Returns the screen height in pixels
int lcd_getheight();
//! Turn screen off
void lcd_off();
//! Turn screen on
void lcd_on();

/*! \brief    Set the contrast level.
    \param    level Number from 0 to 15, being 15=darkest, 0=lightest
*/
void lcd_setcontrast(int level);



// mmu mapping functions
//! Convert a virtual address to its physical bus address
EXTERN int sys_map_v2p(unsigned int vaddr);
//! Convert a physical bus address to its virtual address 
EXTERN int sys_map_p2v(unsigned int paddr);
//! Classify a memory address 0=System; 1=Port1; 2=Main 
int sys_mem_classify(int addr);

// mmu independent physical malloc

void *sys_phys_malloc(size_t);


// particular functions

/*!    \fn    void playTone(unsigned freq, unsigned duration)
    \brief    Toggle the speaker according to the input parameters.
    \param    freq frequency
    \param    duration duration
    \warning Not yet fully implemented, parameter interpretation may be wrong
*/
EXTERN void sys_playTone(unsigned int tone, unsigned int duration);
EXTERN void sys_waitTicks (unsigned int time);
EXTERN void setDisplayPlane(unsigned char * plane);

EXTERN void sys_playTone2(unsigned int time, unsigned int duration);

//! Returns '1' if an RTC Interrupt Tick has occured since it was last called
int sys_isRTCTick();
//! Ajdusts the rate of the RTC Tick. Remember to restore the default when done!
unsigned char sys_setRTCTickPeriod(unsigned char n);
//! Waits for 'count' RTC Ticks to elapse.
void sys_waitRTCTicks(int count);


// < ibl >

/*!
    \fn int sys_delta_seconds(int t_start, int t_end)
    \brief Compute the time span (in seconds) between t_start and t_end. 
    Account for possible wrap around at midnight.
    \param t_start Start time
    \param t_end End time
*/

    
int sys_delta_seconds(int t_start, int t_end);

// < ibl ; 2005-02-08 >

/*!
    \fn void sys_error(const char *msg, int code)
    \brief Output msg to screen, set errno=code and exit if code < 0
    \param msg The message text or \c NULL for none
    \param code The error code, forwarded to exit()
    
*/

void sys_error(const char *msg, int code);

// < ibl ; 2005-05-16 >

/*!
    \fn void sys_chkptr(void *p)
    \brief Check pointer after malloc'ing and exit via sys_error() if NULL.
    \param p the pointer to be checked
    
*/

void *sys_chkptr(void *p);

// MAIN EXCEPTION HANDLER


/*!
    \brief Options for throw_exception
*/
#define __EX_CONT 1		// SHOW CONTINUE OPTION
/*!
    \brief Options for throw_exception
*/
#define __EX_EXIT 2		// SHOW EXIT OPTION
/*!
    \brief Options for throw_exception
*/
#define __EX_WARM 4		// SHOW WARMSTART OPTION
/*!
    \brief Options for throw_exception
*/
#define __EX_RESET 8	// SHOW RESET OPTION


/*!
    \brief Throw a user exception
    Cause a user exception to be thrown. It displays the requested message and offer the user several
    options to treat the exception.
    
    \param message The string that will be displayed.
    \param options One or more of the following constants:
    \li __EX_CONT Display the "Continue" option, allowing the program to continue.
    \li __EX_EXIT Display the "Exit" option, which immediately exits the program.
    \li __EX_WARM Display the "Warmstart" option, which exits the program and causes a
                  restart of the calculator, similar to On-C.
    \li __EX_RESET Display the "Reset" option, which exits the program and then reset the calculator
                   in a way equivalent to a paperclip. It is similar to a Warmstart but also restarts
                   all the ARM hardware.
    \note If the options parameter is passed as 0, the calculator will display the exception message
          and show no options, staying in an infinite loop until the user resets the calc using a
          paperclip. Only use this option if it is not safe to attempt to exit the program.
                   
    \return The function does not return a value, and it may not return at all depending on the
            user's choice to handle the exception. If the user chooses to exit, warmstart or reset
            the program will exit first as if the exit() function was called.
*/
void throw_exception(char * message, unsigned int options);

void throw_dbgexception(char * message, unsigned int options);


// *****************************************************
// **************  IRQ MACHINERY ***********************
// *****************************************************


/*!
    \brief Install an IRQ handler 
	Set a routine to service interrupt requests from a specific device. It does not change the
	state of the interrupt controller, so the user needs to manually unmask the interrupt and
	configure the rest of the hardware to generate the proper IRQ request.
    
    \param service_number Identifies the device that is causing the interrupt. It's a 
    					  number from 0 to 31 according to the list below (see Samsung S3C2410X manual).
    \param serv_routine   Address of a service routine. The service routine has no special requirements, other
                          than returning as fast as possible to allow other IRQ's to be processed.
    \note The interrupt service number is as follows:
\liINT_ADC  = [31]
\liINT_RTC  = [30]
\liINT_SPI1 = [29]
\liINT_UART0= [28]
\liINT_IIC  = [27]
\liINT_USBH = [26]
\liINT_USBD = [25]
\liReserved = [24] Not used
\liINT_UART1= [23]
\liINT_SPI0 = [22]
\liINT_SDI  = [21]
\liINT_DMA3 = [20]
\liINT_DMA2 = [19]
\liINT_DMA1 = [18]
\liINT_DMA0 = [17]
\liINT_LCD  = [16]
\liINT_UART2= [15]
\li\liINT_TIMER4=[14]
\liINT_TIMER3=[13]
\liINT_TIMER2=[12] [Used for HPGCC3 sound routines, do not use]
\liINT_TIMER1=[11] [Used for HPGCC3 timed events, do not use]
\liINT_TIMER0=[10] [Used for HPGCC3 system timer, do not use]
\liINT_WDT  = [9]
\liINT_TICK = [8]
\linBATT_FLT= [7]
\liReserved = [6] Not used
\liEINT8_23 = [5] [Used for HPGCC3 keyboard routines, do not use]
\liEINT4_7  = [4] [Used for HPGCC3 keyboard routines, do not use]
\liEINT3    = [3]
\liEINT2    = [2]
\liEINT1    = [1]
\liEINT0    = [0] [Used for HPGCC3 exception handler, do not use]

 \sa __irq_releasehook
*/

void __irq_addhook(int service_number,__interrupt__ serv_routine);


/*!
    \brief Uninstall an IRQ handler 
	Removes a service routine that handles interrupt requests from a specific device. It does not change the
	state of the interrupt controller, so the user needs to manually mask the interrupt and
	configure the rest of the hardware to stop generating IRQ requests. If an IRQ is generated
	after this routine is called, it will be serviced by a do-nothing routine.
    
    \param service_number Identifies the device that is causing the interrupt. It's a 
    					  number from 0 to 31 according to the list below (see Samsung S3C2410X manual).
    \note See __irq_addhook for a list of interrupt service numbers

 \sa __irq_addhook
*/

void __irq_releasehook(int service_number);




#endif 

