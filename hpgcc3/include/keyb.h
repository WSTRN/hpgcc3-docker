//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

#ifndef _KEYB_H
#define _KEYB_H

// LONG TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05
#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif

/*!
 * \file keyb.h
 * \brief Keyboard management routines for the HP49.
 *
 * Interaction with the calculator keyboard can be done at a high level, or a
 * low level.  Functions defined in this file are used to interact at a low
 * level with the calculator keyboard.  For higher-level user input routines,
 * see ::getchar, ::gets, ::getkey, ::scanf.
 * 
 * There are 2 ways to access the keyboard:
 * 1) Instantaneous state. All keyb_isXXX functions and keyb_getmatrix check the state of the keyboard
 * at a particular instant of time. Therefore, they can only detect whether a key is
 * pressed or not at the moment of the call (no keyplanes). This method is more often
 * used in games.
 * 2) Asynchronous using keyboard messages. Functions keyb_XXXmsg use a message queue.
 * The keyboard is monitored automatically and messages are posted to the queue.
 * A program can later retrieve those messages to know which keys and in which order
 * were pressed. This set of functions can detect shift planes, and can autorepeat
 * keys when the user holds the key. This method is more appropriate for text-inputting
 * applications, and is the method used by the higher level functions.
 * 
 * 
 *
*/





// KEYBOARD MATRIX HANDLING ROUTINE, ADDED BY CLAUDIO

/* 

KEYBOARD BIT MAP
----------------
This is the bit number in the 64-bit keymatrix.
Bit set means key is pressed.

    A]-+  B]-+  C]-+  D]-+  E]-+  F]-+  
    |41|  |42|  |43|  |44|  |45|  |46|
    +--+  +--+  +--+  +--+  +--+  +--+  

    G]-+  H]-+  I]-+        UP]+        
    |47|  |53|  |54|        |49|      
    +--+  +--+  +--+  LF]+  +--+  RT]+  
                      |50|  DN]+  |52|
    J]-+  K]-+  L]-+  +--+  |51|  +--+  
    |55|  |57|  |58|        +--+    
    +--+  +--+  +--+                    

    M]--+  N]--+  O]--+  P]--+  BKS]+
    | 33|  | 25|  | 17|  | 09|  | 01|
    +---+  +---+  +---+  +---+  +---+

    Q]--+  R]--+  S]--+  T]--+  U]--+
    | 34|  | 26|  | 18|  | 10|  | 02|
    +---+  +---+  +---+  +---+  +---+

    V]--+  W]--+  X]--+  Y]--+  /]--+
    | 35|  | 27|  | 19|  | 11|  | 03|
    +---+  +---+  +---+  +---+  +---+
    
    AL]-+  7]--+  8]--+  9]--+  *]--+
    | 60|  | 28|  | 20|  | 12|  | 04|
    +---+  +---+  +---+  +---+  +---+
    
    LS]-+  4]--+  5]--+  6]--+  -]--+
    | 61|  | 29|  | 21|  | 13|  | 05|
    +---+  +---+  +---+  +---+  +---+
    
    RS]-+  1]--+  2]--+  3]--+  +]--+
    | 62|  | 30|  | 22|  | 14|  | 06|
    +---+  +---+  +---+  +---+  +---+
    
    ON]-+  0]--+  .]--+  SP]-+  EN]-+
    | 63|  | 31|  | 23|  | 15|  | 07|
    +---+  +---+  +---+  +---+  +---+

*/

//! Constant for the F1 (A) key
#define KB_A   41
//! Constant for the F2 (B) key
#define KB_B   42
//! Constant for the F3 (C) key
#define KB_C   43
//! Constant for the F4 (D) key
#define KB_D   44
//! Constant for the F5 (E) key
#define KB_E   45
//! Constant for the F6 (F) key
#define KB_F   46
//! Constant for the APPS (G) key
#define KB_G   47
//! Constant for the MODE (H) key
#define KB_H   53
//! Constant for the TOOL (I) key
#define KB_I   54
//! Constant for the VAR (J) key
#define KB_J   55
//! Constant for the STO (K) key
#define KB_K   57
//! Constant for the NXT (L) key
#define KB_L   58
//! Constant for the HIST (M) key
#define KB_M   33
//! Constant for the EVAL (N) key
#define KB_N   25
//! Constant for the ' (O) key
#define KB_O   17
//! Constant for the SYMB (P) key
#define KB_P    9
//! Constant for the Y^X (Q) key
#define KB_Q   34
//! Constant for the Sqrt (R) key
#define KB_R   26
//! Constant for the SIN (S) key
#define KB_S   18
//! Constant for the COS (T) key
#define KB_T   10
//! Constant for the TAN (U) key
#define KB_U    2
//! Constant for the EEX (V) key
#define KB_V   35
//! Constant for the +/- (W) key
#define KB_W   27
//! Constant for the X (X) key
#define KB_X   19
//! Constant for the 1/X (Y) key
#define KB_Y   11
//! Constant for the / (Z) key
#define KB_Z KB_DIV
//! Constant for the 0 key
#define KB_0   31
//! Constant for the 1 key
#define KB_1   30
//! Constant for the 2 key
#define KB_2   22
//! Constant for the 3 key
#define KB_3   14
//! Constant for the 4 key
#define KB_4   29
//! Constant for the 5 key
#define KB_5   21
//! Constant for the 6 key
#define KB_6   13
//! Constant for the 7 key
#define KB_7   28
//! Constant for the 8 key
#define KB_8   20
//! Constant for the 9 key
#define KB_9   12
//! Constant for the backspace key
#define KB_BKS  1
//! Constant for the / (Z) key
#define KB_DIV  3
//! Constant for the * key
#define KB_MUL  4
//! Constant for the + key
#define KB_ADD  6
//! Constant for the - key
#define KB_SUB  5
//! Constant for the . key
#define KB_DOT 23
//! Constant for the SPC key
#define KB_SPC 15
//! Constant for the ENT key
#define KB_ENT  7
//! Constant for the up arrow key
#define KB_UP  49
//! Constant for the down arrow key
#define KB_DN  51
//! Constant for the left arrow key
#define KB_LF  50
//! Constant for the right arrow key
#define KB_RT  52
//! Constant for the ALPHA key
#define KB_ALPHA   60
//! Constant for the left shift key
#define KB_LSHIFT  61
//! Constant for the right shift key
#define KB_RSHIFT  62
//! Constant for the ON key
#define KB_ON  63

// MACROS TO CREATE KEYBOARD MASKS
/*!
 * \brief Create a bitmask representing the given key.
 *
 * The bitmask returned by this macro can be compared with the keymatrix result.
 */
#define KB_MASK(a) (((ULONGLONG)1)<<(a))

// COMMON KEYBOARD MASKS
// ALL CURSOR KEYS

/*!
 * \brief A bitmask to represent the arrow keys.
 *
 * The keymatrix can be combined with this mask
 * via a bitwise and (&) to eliminate everything except the state of the
 * arrow keys (aka cursor keys).  This is a convenience constant, and its
 * value is equivalent to: \c KB_MASK(KB_UP) \c | \c KB_MASK(KB_DN)
 * \c | \c KB_MASK(KB_LF) \c | \c KB_MASK(KB_RT).
 */
#define KB_CURS ((ULONGLONG)0x001e000000000000)

// ALL FUNCTION KEYS (A-F)
/*!
 * \brief A bitmask to represent the function keys.
 *
 * The keymatrix can be combined with this mask
 * via a bitwise and (&) to eliminate everything except the state of the
 * function keys.  This is a convenience constant, and its value is equivalent
 * to: \c KB_MASK(KB_A) \c | \c KB_MASK(KB_B) \c | \c KB_MASK(KB_C)
 * \c | \c KB_MASK(KB_D) \c | \c KB_MASK(KB_E) \c | \c KB_MASK(KB_F).
 */
#define KB_FUNC ((ULONGLONG)0x00007e0000000000)


// SHIFT CONSTANTS FOR HIGH-LEVEL KEYBOARD FUNCTIONS

//! Shift constant to use in a combined shiftcode. Left Shift.
#define SHIFT_LS          0x40
//! Shift constant to use in a combined shiftcode. Hold-Left Shift.
#define SHIFT_LSHOLD      0x80
//! Shift constant to use in a combined shiftcode. Right Shift.
#define SHIFT_RS         0x100
//! Shift constant to use in a combined shiftcode. Hold-Right Shift.
#define SHIFT_RSHOLD     0x200

//! Shift constant to use in a combined shiftcode. Alpha.
#define SHIFT_ALPHA      0x400
//! Shift constant to use in a combined shiftcode. Hold-Alpha.
#define SHIFT_ALPHAHOLD  0x800

//! Shift constant to use in a combined shiftcode. Hold-On key.
#define SHIFT_ONHOLD    0x1000

//! Shift constant to use in a combined shiftcode. Any Shift or ON.
#define SHIFT_ANY    0x1fc0    


//! Extracts a keycode from a combined shiftcode.
#define KEYCODE(SHIFTcode) ((SHIFTcode)&0x3f)
//! Extracts shift plane from a combined shiftcode.
#define SHIFTPLANE(SHIFTcode) ((SHIFTcode)&SHIFT_ANY)


// KEYMATRIX TYPE DEFINITION
/*!
 * \brief A matrix of simultaneous key states.
 *
 * This data type is a 64-bit integer used to represent the complete state of the keyboard.
 *
 */
typedef ULONGLONG keymatrix;

// SCANS THE KEYBOARD AND STORES THE 64-BIT MATRIX
/*!
 * \brief Retrieves the state of the complete keyboard.
 *
 * This function retrieves the state of the entire keyboard in one
 * operation. The result can then be inspected using the various \c KB_*
 * preprocessor macros and constants.
 *
 * \return A ::keymatrix, which will hold the result.
 */
keymatrix keyb_getmatrix();

/*!
 * \brief Tests the current state of a key.
 *
 * This macro detects if a key is pressed.
 *
 * \param keycode The KB_... constant of a key.
 * \return TRUE if the key is pressed; FALSE otherwise
 * 
 */
#define keyb_isKeyPressed(keycode) (keyb_getkey(0)==keycode)

/*!
 * \brief Tests whether any key is being pressed on the keyboard.
 * \return TRUE if a key is pressed; FALSE otherwise
 * 
 */

#define keyb_isAnyKeyPressed() (keyb_getmatrix()!=0LL)

/*!
 * \brief Tests if the left arrow key is down.
 *
 * This is a convenience macro.
 *
 * \return TRUE if the key is pressed; FALSE otherwise
 */
#define keyb_isLeft() ((keyb_getmatrix()&KB_MASK(KB_LF))!=0)

/*!
 * \brief Tests if the right arrow key is down.
 *
 * This is a convenience macro.
 * 
 * \return TRUE if the key is pressed; FALSE otherwise
 */
#define keyb_isRight() ((keyb_getmatrix()&KB_MASK(KB_RT))!=0)

/*!
 * \brief Tests if the up arrow key is down.
 *
 * This is a convenience macro.
 *
 * \return TRUE if the key is pressed; FALSE otherwise
 */
#define keyb_isUp() ((keyb_getmatrix()&KB_MASK(KB_UP))!=0)

/*!
 * \brief Tests if the down arrow key is down.
 *
 * This is a convenience macro.
 *
 * \return TRUE if the key is pressed; FALSE otherwise
 */
#define keyb_isDown() ((keyb_getmatrix()&KB_MASK(KB_DN))!=0)

/*!
 * \brief Tests if the alpha key is down.
 * 
 * This is a convenience macro.
 * 
 * \return TRUE if the key is pressed; FALSE otherwise
 *
 */
#define keyb_isAlpha() ((keyb_getmatrix()&KB_MASK(KB_ALPHA))!=0)

/*!
 * \brief Tests if the left-shift key is down.
 * 
 * This is a convenience macro.
 * 
 * \return TRUE if the key is pressed; FALSE otherwise
 *
 */
#define keyb_isLS() ((keyb_getmatrix()&KB_MASK(KB_LSHIFT))!=0)

/*!
 * \brief Tests if the right-shift key is down.
 * 
 * This is a convenience macro.
 * 
 * \return TRUE if the key is pressed; FALSE otherwise
 *
 */
#define keyb_isRS() ((keyb_getmatrix()&KB_MASK(KB_RSHIFT))!=0)
/*!
 * \brief Tests if the ON key is down.
 * 
 * This is a convenience macro.
 * 
 * \return TRUE if the key is pressed; FALSE otherwise
 *
 */
#define keyb_isON() ((keyb_getmatrix()&KB_MASK(KB_ON))!=0)


/*!
 * \brief Returns the key constant of the first key pressed.
 *
 * Optionally waits for a non-shift key to be pressed, and then returns a 
 * shiftplane specifying which key was pressed and the shift state.
 *
 * If the wait parameter is non-zero, this function does not return until a
 * key has been completely pressed and released (only the key, shift can remain
 * pressed). If multiple keys are pressed simultaneously, the function does
 * not return until all keys have been released; but the return value will be
 * the identifier of the first key.
 * If the wait parameter is zero, the function will wait neither for a key
 * to be pressed or released.
 * The ON key is detected as a normal key, but Shift-ON is not detected.
 *
 * \param wait If 0, return a 0 identifier if no key is pressed;
 *             If non-zero, wait until a key is pressed.
 *
 * \return A shiftcode for the key pressed, or 0 if no key was pressed and
 *         the wait parameter was zero. The shiftcode will be comprised of
 *         a KB_... constant and a combination of the SHIFT_... constants.
 *         Use the KEYCODE() macro to extract the keycode from the shiftcode.
 *         For example,
 *                    int key=keyb_getkey(1);
 *         To check for A regardless of shift state:
 *                    if(KEYCODE(key)==KB_A) ...
 *         To check for LS-A (LS only):
 *                    if(key==(KB_A | SHIFT_LS | SHIFT_LSHOLD)) ...
 *         or         if(KEYCODE(key)==KB_A && SHIFTPLANE(key)==SHIFT_LS|SHIFT_LSHOLD)
 *
 * \note Because this function uses instantaneous keyboard readings, it can only detect
 *       shift-hold planes. Therefore, it always return a combination of (SHIFT_LS|SHIFT_LSHOLD),
 *       (SHIFT_RS|SHIFT_RSHOLD), or (SHIFT_ALPHA|SHIFT_ALPHAHOLD) for the shift plane.
 */
int keyb_getkey(int wait);


// convenience macro
// < ibl ; 2005-05-08 >

#define WAIT_CANCEL {while(keyb_isON()); while(! keyb_isON());}
#define SLOW_WAIT_CANCEL {cpu_setspeed(6*1000000);while(keyb_isON()); while(! keyb_isON());}

// *************************************
//  NEW HPGCC3 KEYBOARD MACHINERY
// *************************************

//! \brief Keyboard message constant, to be combined with one of the KB_XXX key constants
#define KM_PRESS  0x0000
//! \brief Keyboard message constant, to be combined with one of the KB_XXX key constants
#define KM_KEYDN  0x4000
//! \brief Keyboard message constant, to be combined with one of the KB_XXX key constants
#define KM_KEYUP  0x8000
//! \brief Keyboard message constant, to be combined with one of the KB_XXX key constants
#define KM_SHIFT 0xc000

//! \brief Mask to isolate the key shift plane bits
#define KM_SHIFTMASK 0x1fc0
//! \brief Mask to isolate the key value bits
#define KM_KEYMASK  0x003f
//! \brief Mask to isolate the key message bits
#define KM_MSGMASK  0xc000


//! \brief Keyboard message queue size (# of messages)
#define KEYB_BUFFER 128
//! \brief Keyboard scanning speed in milliseconds
#define KEYB_SCANSPEED 20

//! \brief Convenience macro to extract message type from a message
#define KM_MESSAGE(a) ( (a) & KM_MSGMASK) 
//! \brief Convenience macro to extract pure key code from a message
#define KM_KEY(a) ( (a) & KM_KEYMASK) 
//! \brief Convenience macro to extract shifted key code from a message
#define KM_SHIFTEDKEY(a) ( (a) & (KM_KEYMASK|KM_SHIFTMASK)) 
//! \brief Convenience macro to extract shift plane from a message
#define KM_SHIFTPLANE(a) ( (a) & KM_SHIFTMASK) 



/*!
 * \brief Inserts a key message into the keyboard buffer.
 *
 * Use this function to simulate a keystroke by inserting the proper messages into the keyboard queue.
 * A valid keypress sequence is composed of a KM_KEYDN message, followed by a KM_PRESS and a KM_KEYUP.
 * 
 * \param msg A keyboard message, composed of KM_KEYUP, KM_KEYDN or KM_PRESS
 * plus a key value (a KB_XXX constant). KM_PRESS messages can optionally have a SHIFT_XXX shift plane.
 * 
 * For example, to simulate pressing the key A in Alpha mode, the sequence of messages
 * can be formed as: (KM_KEYDN | KB_A), then (KM_PRESS | KB_A | SHIFT_ALPHA) and finally
 * (KM_KEYUP | KB_A). If the client program does not require the complete sequence to work, 
 * sending only the KM_PRESS message will do the job. 
 *
 */
void keyb_postmsg(unsigned int msg);


/*!
 * \brief Get a key message from the keyboard buffer.
 *
 * Use this function to extract key messages from the queue. This function gives access
 * to the most basic keyboard functions. Consider using other higher level keyboard functions
 * for more advanced features, like functions from the stdio module.
 * When a key is pressed, 2 messages are immediately generated: KM_KEYDN and KM_PRESS. Then,
 * if the user keeps the key pressed for a long period of time (see keyb_settiming), an additional
 * KM_PRESS message will be posted. If the user continues to hold the key, an extra KM_PRESS
 * message will be generated at the keyboard repeat rate (see keyb_settiming).
 * When the user releases the key, a KM_KEYUP message will be posted.
 * Notice that Alpha, Left Shift, Right Shift and ON do not generate KM_PRESS messages, only
 * KM_KEYDN/KM_KEYUP pairs.
 * Every time the shift plane changes, a KM_SHIFT message is generated, indicating the new
 * plane. Each KM_PRESS message already carries shift plane information, the KM_SHIFT message
 * is posted to do other tasks like updating the annunciator icons.
 *  
 * 
 * \return A keyboard message, composed of KM_KEYUP, KM_KEYDN or KM_PRESS
 * plus a key value (a KB_XXX constant). To isolate the key value in the message use
 * the bit mask KM_KEYMASK (like in key=full_msg&KM_KEYMASK). Similarly, to isolate the message type
 * use the bit mask KM_MSGMASK (msg=full_msg&KM_MSGMASK).
 * The KM_PRESS and KM_SHIFT messages include shift planes, that can be isolated 
 * using the KM_SHIFTMASK bit mask.
 *
 */
unsigned int keyb_getmsg();


/*!
 * \brief Set all keyboard timing constants.
 *
 * This function defines keyboard timing through 3 parameters.
 * 
 *  \param repeat Time in milliseconds between KM_PRESS messages when the user holds
 *                down a key.
 *  \param longpress Time in milliseconds the user needs to hold down a key to generate
 *                   the first KM_LPRESS message and start autorepeating the keypress.
 *  \param debounce Time in milliseconds measured from the moment the user releases a key,
 *                  in which the keyboard handler will ignore additional keypresses of the
 *                  same key
 * 
 * 
 */
void keyb_settiming(int repeat,int longpress,int debounce);


/*!
 * \brief Activate/deactivate keyboard autorepeat.
 *
 *  \param repeat  Nonzero to activate autorepeat, zero to disable the feature.
 * 
 */
void keyb_setrepeat(int repeat);


/*!
 * \brief Activate/deactivate single-alhpa lock mode.
 *
 *  \param single_alpha_lock  Nonzero to lock alpha mode with a single
 *  alpha keypress, zero to require double alpha keypress to lock.
 * 
 */
void keyb_setalphalock(int single_alpha_lock);

/*!
 * \brief Manualy set the shift plane.
 *
 *  \param leftshift Nonzero to activate left shift plane.
 *  \param rightshift Nonzero to activate right shift plane.
 *  \param alpha Nonzero to activate alpha shift plane for the next keypress only.
 *  \param alphalock Nonzero to activate alpha mode for all subsequent keypresses.
 * 
 * \note This function will wait until all keys have been released prior to changing
 * the shift plane.
 * 
 */
void keyb_setshiftplane(int leftshift,int rightshift,int alpha,int alphalock);


#endif 
