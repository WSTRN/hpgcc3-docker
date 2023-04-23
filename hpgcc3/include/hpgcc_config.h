//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


#ifndef __HPGCC_CONFIG_H
#define __HPGCC_CONFIG_H

/*!
  * \file hpgcc_config.h
  * \brief Configuration constants for the hpgcc runtime libraries
  *
  */

/*!
  \page buildinfo HPGCC3 build process and executable configuration.

 HPGCC3 introduced a simpler way to configure the execution of a C program
 from within the program, rather than the traditional method of modifying the
 command line arguments passed to the compiler, linker or elf2armc.
 
 With HPGCC3, all execution parameters can be altered by declaring special variables
 in your code, as shown below. This method turns the Default_Makefile used by the dmake
 tool into a universal Makefile that works with virtually 100% of the programs.

 You can use one of the following convenience macros for the most common configurations:

To run programs that make use of the stack and manipulate calculator objects, include in your source code... 
 \code
 ARMCODE_CONFIG_STACK_STANDARD
 \endcode
This standard option will set the most common options for programs that don't interact with the user, plus it will
reserve 16 kbytes of TEMPOB and 16 kbytes of USEROB for calculator object manipulation.
The program will run at the default speed of 75 MHz.

\code
ARMCODE_CONFIG_STACK_SLOW
\endcode
This option is similar to the standard, but running at 12 MHz.

\code
ARMCODE_CONFIG_STACK_OVERCLOCK
\endcode
This option is similar to the standard, but running at 192 MHz, for speed-critical routines.

\code
ARMCODE_CONFIG_STACK_FULLMEMORY
\endcode
This option is similar to the standard, but it's configured to leave the entire main ram untouched.
This gives the program more freedom to manipulate large calculator obejcts, but leaving less memory
available for the C heap. It runs at 75 MHz.

\code
ARMCODE_CONFIG_STACK_FULLMEMORY_SLOW
\endcode
This option is similar to the full memory, but runs at 12 MHz.

\code
ARMCODE_CONFIG_STACK_FULLMEMORY_OVERCLOCK
\endcode
This option is similar to the full memory, but runs at 192 MHz.

If no configuration statement is included, the code will execute with all modules enabled and
at a speed of 12 MHz by default. This is well suited for programs with a user interface.

Alternatively, to fully control the way a program is executed, follow the instructions below.  
In your code, include one or more lines as follows (in a global scope):
 
\code  
  ARMCODE_CONFIG(property) = value;
\endcode
  
  where 'property' is one of the following (case sensitive):
  
 slack_space  ==> Controls the slack space left for program alignment.
                  Zero is the default value and means no slack space. Normally, no slack space is needed
                  to execute a program, but if the program is large it is better to include a slack space
                  so it can be stored and executed directly from flash memory, releasing more ram. 
  				  Use a value between 2 and 12 to reserve 2^n bytes of
 				  slack space. The only recommended value is n=10, to
 				  reserve 2^10=1024 bytes for page-alignment. Less than
				  1024 bytes does not guarantee alignment under all
                  conditions, using more than 1024 bytes is not needed.
				  n=1 is reserved and should NOT be used in any case.
  
  memory_req  ==> Controls the memory requirements of the program. Use
                  any combination of the following constants:
				  
				  USE_MAINRAM | USE_KOSRAM | USE_ERAM |
				  RESERVE_TEMPOB(xx) | RESERVE_USEROB(xx)
				  
				  The USE_XXX constans indicate whether or not the
                  corresponding memory region will be used by the program.
                  
                  IMPORTANT: At least one of the three regions must be
                  used, otherwise the program cannot be executed.
				  Default value is to use all memory regions.
				  
				  RESERVE_TEMPOB(xx) indicate how much memory (in kilobytes)
				  should be reserved for TEMPOB growth during execution.
				  Use if you are planning to create large objects in TEMPOB.
				  The default value is 4 kbytes reserved.
  
				  RESERVE_USEROB(xx) indicate how much memory (in kilobytes)
				  should be reserved for stack and USEROB growth during
                  execution. Use if you are planning to create objects in
                  USEROB or push many objects to the stack.
				  The default value is 1 kbytes reserved.
				  
				  NOTE: The default values given here apply only if 
				  this property is not set within the program.
				  When setting this property, any constant not included
				  will default to "don't use" and "don't reserve". Typically a definition must
				  include one or more USE_XXX constants plus both 
				  RESERVE_XXX().

  romlib_version ==> Typically this property does not need to be overriden.
                  Use this property to override the required minimum
				  ROMLIB version number. By default, a program requires to
                  have a ROMLIB version on the calculator greater or equal
                  than the one used to compile the executable. Only change
				  this property to make a program backwards compatible, but
				  it can cause severe crashes if misused.
				  
				  Use macro MAKE_ROMLIB_VER(xxx) to create a proper constant,
				  where xxx is a packed BCD number. For example:
				  \code
				  ARMCODE_CONFIG(romlib_version) = MAKE_ROMLIB_VER(0x123);
				  \endcode
				  
				  will require version number "R123" to be on the calc to run.
				  
  startup     ==> Specify several different options for startup.
				  Options available are:
				  
				  a) Primary options, which control individual functions/modules:
				  
				  _ENABLE_INTS  = Leave interrupts enabled with default handlers. By default
				                  interrupts are overriden with hpgcc handlers. Interrupts can
								  be enabled w/default handlers ONLY on ARM programs. Thumb
								  code cannot be used when this option is specified.
								  
				  _SPEED_6MHZ   = Run the program using 6MHz cpu speed. Preserve clock status on exit.
				  
				  _SPEED_12MHZ  = Run the program using 12MHz cpu speed (default).  Restore clock status on exit.
				  
				  _SPEED_48MHZ  = Run the program at 48MHz cpu speed. Restore clock status on exit.
				  
				  _SPEED_75MHZ  = Run the program at 75MHz cpu speed. Restore clock status on exit.
				  
				  _SPEED_120MHZ = Run the program at 120MHz cpu speed. Restore clock status on exit.

				  _SPEED_152MHZ = Run the program at 152MHz cpu speed. Restore clock status on exit.

				  _SPEED_192MHZ = Run the program at 192MHz cpu speed. Restore clock status on exit.

				  _NO_SPEEDCHANGE = Do not change clock at all, and don't preserve/restore clock status.
				  
				  _NO_WAITKEY   = Do not wait for all keys to be released
				                  before returning to the emulator.
				                  
				  _NO_GUI_CONSOLE = Do not initialize the grayscale console module.
				  
				  _NO_SCREEN    = Do not preserve/modify screen registers at all.
				  
				  _NO_TIMERS    = Do not preserve/modify timers at all.
				  
				  _NO_KEYBOARD  = Do not install keyboard handlers.
				  
				  _NO_EXCEPTIONS= Do not install exception handlers.
				  
				  b) Secondary options, which combine some of the above for specific uses:
				  
				  _RAW_EXECUTE  = Combines all typical options for simple routines with minimal support. It avoids screen flicker and
                                                  improves startup speed. All modules are disabled and interrupts are enabled.

				  _STACK_ONLY   = Combines all typical options for programs that only
                                                  interact through the stack. It avoids screen flicker and
                                                  improves startup speed. All modules are disabled, but exceptions are trapped and interrupts are overriden.

				  _STACK_SPEED  = Combines all typical options for programs that only
                                                  interact through the stack. It avoids screen flicker and
                                                  improves startup speed. All modules are disabled, but exceptions are trapped, interrupts are overriden and clock status is preserved/restored.
                                                  A program can therefore set its own speed through cpu_setspeed().

				  _STACK_SPEED_6MHZ  = Same as _STACK_SPEED but also set the speed at 6MHz on startup.
				  _STACK_SPEED_12MHZ  = Same as _STACK_SPEED but also set the speed at 12MHz on startup.
				  _STACK_SPEED_48MHZ  = Same as _STACK_SPEED but also set the speed at 48MHz on startup.
				  _STACK_SPEED_75MHZ  = Same as _STACK_SPEED but also set the speed at 75MHz on startup.
				  _STACK_SPEED_120MHZ  = Same as _STACK_SPEED but also set the speed at 120MHz on startup.
				  _STACK_SPEED_152MHZ  = Same as _STACK_SPEED but also set the speed at 152MHz on startup.
				  _STACK_SPEED_192MHZ  = Same as _STACK_SPEED but also set the speed at 192MHz on startup.
				  

				  
 \note IMPORTANT: Startup modules are not independent. To use some of the options above you need
 to combine with others to guarantee:
				  _ENABLE_INTS  = Requires _NO_EXCEPTIONS | _NO_KEYBOARD | _NO_TIMERS
			  
				  _NO_TIMERS    = Requires _NO_KEYBOARD
				  
				  _NO_EXCEPTIONS= Requires _NO_KEYBOARD | _NO_TIMERS

Each option disables portions of hpgcc libraries, which will not be available to use:

                  _NO_TIMERS      Can't use any tmr_xxx function
				  _NO_KEYBOARD    Can't use the keyb_getmsg() function, other keyb functions will still work.
				  _NO_EXCEPTIONS  Can't throw exceptions, and the CPU won't catch crashes.
				  _NO_SCREEN      Can't use printf(), HPG or any function dealing with the screen.
				  _NO_SPEEDCHANGE Can't use cpu_setspeed().

 
				 
*/



/*!
  * \brief The library revision number
  */
  
#define HPGCC_VERSION 3.0

/*!
  * \brief The library major revision number
  */
 

#define HPGCC_VERSION_MAJOR 3

/*!
  * \brief The library minor revision number
  */
 
#define HPGCC_VERSION_MINOR 0

/*!
  \brief Activate patch for broken kos_fopen() mode "a" workaround
  
  The kos_fopen() is broken when called with the "a" (append) mode paramter,
  in that respect, that it doesn't create the file, if it doesn't exist.
  This define controls the workaround patch.
  Set to 1 to activate.

*/

/*!
  \brief ROMLib build number
  
  Minimum ROMLib build number required to execute.
  This number is increased by 1 every time a new ROM
  is released. Max. count is 999. The number is 
  packed BCD (0x000 to 0x999)

*/

#define ROMLIB_BUILD 0x004



/*!
  \brief Executable configuration macro.
  
  Use this macro to configure memory requirements of your executable. See \ref buildinfo
*/

#define ARMCODE_CONFIG(var) unsigned int __##var __attribute__ ((section(".config")))


/*!
  \brief Instruct the launcher to allocate and use main ram for the heap
*/

#define USE_MAINRAM 0x00010000
/*!
  \brief Instruct the launcher to allocate and use ERAM for the heap
*/
#define USE_ERAM    0x00020000
/*!
  \brief Instruct the launcher to allocate and use KOS ram for the heap
*/

#define USE_KOSRAM  0x00040000
/*!
  \brief Instruct the launcher to reserve memory (kbytes) for TEMPOB growth
*/
#define RESERVE_TEMPOB(a) (a&0xff)
/*!
  \brief Instruct the launcher to reserve memory for stack and USEROB growth
*/

#define RESERVE_USEROB(a) ((a&0xff)<<8)
/*!
  \brief Convert BCD version number into version word
*/
#define MAKE_ROMLIB_VER(n) ( ((n&0xf0)<<12) | ((n&0xf00)) | ((n&0xf)<<24) | 0x30303052)

/*!
  \brief Instruct the launcher to leave interrupts enabled at startup
*/
#define _ENABLE_INTS  0x00000001
/*!
  \brief Instruct the launcher to switch to 1 MHz mode at startup
*/
//#define _SPEED_1MHZ   0x00000010

/*!
  \brief Instruct the launcher to switch to 6 MHz mode at startup
*/
#define _SPEED_6MHZ   0x00000020
/*!
  \brief Instruct the launcher to switch to 12 MHz mode at startup (default)
*/
#define _SPEED_12MHZ  0x00000000
/*!
  \brief Instruct the launcher to switch to 48 MHz mode at startup
*/
#define _SPEED_48MHZ  0x00000030
/*!
  \brief Instruct the launcher to switch to 75 MHz mode at startup
*/
#define _SPEED_75MHZ  0x00000040
/*!
  \brief Instruct the launcher to switch to 120 MHz mode at startup
*/
#define _SPEED_120MHZ 0x00000050

/*!
  \brief Instruct the launcher to switch to 152 MHz mode at startup
*/
#define _SPEED_152MHZ 0x00000060

/*!
  \brief Instruct the launcher to switch to 192 MHz mode at startup
*/
#define _SPEED_192MHZ 0x00000070

/*!
  \brief Instruct the launcher not to change clock speed
*/
#define _NO_SPEEDCHANGE 0x000000f0
/*!
  \internal
  \brief Mask for clock modes
*/
#define _CLK_MODE_MSK 0x000000f0
/*!
  \brief Instruct the launcher not to wait for all keys released before returning.
*/
#define _NO_WAITKEY    0x00000002
/*!
  \brief Instruct the launcher not to initialize the stdio module
*/
#define _NO_GUI_CONSOLE 0x00000004
/*!
  \brief Instruct the launcher not to initialize the screen
*/
#define _NO_SCREEN     0x00000008
/*!
  \brief Instruct the launcher not to initialize the timers
*/
#define _NO_TIMERS     0x00000100

/*!
  \brief Instruct the launcher not to catch exceptions
*/
#define _NO_EXCEPTIONS     0x00000200

/*!
  \brief Instruct the launcher not to install a keyboard handler
*/
#define _NO_KEYBOARD     0x00000400

/*!
  \brief Use typical options to execute programs with minimal support from hpgcc libraries. Timers, keyboard, graphics, exceptions, stdio, cpu speed are all disabled and cannot be used. Interrupts are left enabled, therefore code cannot run in Thumb mode.
*/
#define _RAW_EXECUTE (_NO_KEYBOARD | _NO_EXCEPTIONS | _NO_TIMERS | _NO_SCREEN | _NO_GUI_CONSOLE | _NO_WAITKEY | _NO_SPEEDCHANGE | _ENABLE_INTS)

/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Timers, keyboard, graphics, stdio, cpu speed are disabled and cannot be used. Exceptions are trapped and Thumb code can run normally.
*/
#define _STACK_ONLY (_NO_KEYBOARD | _NO_TIMERS | _NO_SCREEN | _NO_GUI_CONSOLE | _NO_WAITKEY | _NO_SPEEDCHANGE)
/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Similat to _STACK_ONLY, but also allows to change the cpu speed.
*/
#define _STACK_SPEED (_NO_KEYBOARD | _NO_TIMERS | _NO_SCREEN | _NO_GUI_CONSOLE | _NO_WAITKEY)

/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Same as _STACK_SPEED, but including automatic speed change to the desired speed.
*/
#define _STACK_SPEED_120MHZ (_STACK_SPEED | _SPEED_120MHZ)
/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Same as _STACK_SPEED, but including automatic speed change to the desired speed.
*/
#define _STACK_SPEED_152MHZ (_STACK_SPEED | _SPEED_152MHZ)
/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Same as _STACK_SPEED, but including automatic speed change to the desired speed.
*/
#define _STACK_SPEED_192MHZ (_STACK_SPEED | _SPEED_192MHZ)
/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Same as _STACK_SPEED, but including automatic speed change to the desired speed.
*/
#define _STACK_SPEED_75MHZ (_STACK_SPEED | _SPEED_75MHZ)
/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Same as _STACK_SPEED, but including automatic speed change to the desired speed.
*/
#define _STACK_SPEED_48MHZ (_STACK_SPEED | _SPEED_48MHZ)
/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Same as _STACK_SPEED, but including automatic speed change to the desired speed.
*/
#define _STACK_SPEED_12MHZ (_STACK_SPEED | _SPEED_12MHZ)
/*!
  \brief Use typical option for programs that run on the stack only (no user interaction). Same as _STACK_SPEED, but including automatic speed change to the desired speed.
*/
#define _STACK_SPEED_6MHZ (_STACK_SPEED | _SPEED_6MHZ)


/*!
  \brief Convenience configuration options and memory requirements for stack programs.
*/
#define ARMCODE_CONFIG_STACK_STANDARD ARMCODE_CONFIG(startup)=_STACK_SPEED | _NO_SPEEDCHANGE; ARMCODE_CONFIG(memory_req)=USE_MAINRAM|USE_ERAM|USE_KOSRAM|RESERVE_TEMPOB(16)|RESERVE_USEROB(16);
/*!
  \brief Convenience configuration options and memory requirements for stack programs.
*/
#define ARMCODE_CONFIG_STACK_SLOW ARMCODE_CONFIG(startup)=_STACK_SPEED_12MHZ; ARMCODE_CONFIG(memory_req)=USE_MAINRAM|USE_ERAM|USE_KOSRAM|RESERVE_TEMPOB(16)|RESERVE_USEROB(16);
/*!
  \brief Convenience configuration options and memory requirements for stack programs.
*/
#define ARMCODE_CONFIG_STACK_OVERCLOCK ARMCODE_CONFIG(startup)=_STACK_SPEED_192MHZ; ARMCODE_CONFIG(memory_req)=USE_MAINRAM|USE_ERAM|USE_KOSRAM|RESERVE_TEMPOB(16)|RESERVE_USEROB(16);
/*!
  \brief Convenience configuration options and memory requirements for stack programs.
*/
#define ARMCODE_CONFIG_STACK_FULLMEMORY ARMCODE_CONFIG(startup)=_STACK_SPEED | _NO_SPEEDCHANGE; ARMCODE_CONFIG(memory_req)=USE_ERAM|USE_KOSRAM;
/*!
  \brief Convenience configuration options and memory requirements for stack programs.
*/
#define ARMCODE_CONFIG_STACK_FULLMEMORY_SLOW ARMCODE_CONFIG(startup)=_STACK_SPEED_12MHZ; ARMCODE_CONFIG(memory_req)=USE_ERAM|USE_KOSRAM;
/*!
  \brief Convenience configuration options and memory requirements for stack programs.
*/
#define ARMCODE_CONFIG_STACK_FULLMEMORY_OVERCLOCK ARMCODE_CONFIG(startup)=_STACK_SPEED_192MHZ; ARMCODE_CONFIG(memory_req)=USE_ERAM|USE_KOSRAM;


#endif
