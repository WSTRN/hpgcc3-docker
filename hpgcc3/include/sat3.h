//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


// $Id: sat3.h 578 2008-08-04 23:59:44Z claudio $

#ifndef __SAT3_H
#define __SAT3_H

#ifndef DECNUMBER
#include <decNumber.h>
#endif

#ifndef _XREAL_H
#include <extreal.h>
#endif


#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif


/*!
 * \file sat3.h
 * 
 * Interface module with the calculator.
 * 
 * The Saturn module enables interaction with the calculator, access to variables, the stack, etc.
 * 
 * 
 * \defgroup satmodule Saturn Module
 * 
 * For a more detailed explanation and basic concepts, see \ref satmoddocs.
 * 
 * \page satmoddocs Saturn Module in detail
 *
 * This module provides access to many calculator OS features.
 *
 * \section overview Overview
 *
 * The saturn module provides access to calculator objects, directories, stack and other features.
 * For clarity, this documentation has been divided in the following topics:
 *
 * \subpage archoview
 * \subpage sat3oview
 * \subpage fncreate
 * \subpage funcref
 *
 * \page archoview Saturn architecture overview
 * 
 * Saturn architecture is object oriented, and each object is represented with a data structure analog to a C-style struct.
 * One of the main differences between modern processors and the old Saturn is that memory is accessed in half-bytes, in other words, 4-bit
 * chunks of data called nibbles. All data in memory is nibble-aligned therefore it cannot be accessed from the byte-oriented C environment using pointers.
 * 
 * RAM in the Saturn architecture is divided in 3 main areas: TEMPOB, USEROB and PORTS.
 *
 * TEMPOB stands for TEMPorary OBjects, and it contains all temporary objects created during normal calculator
 * operation. Every object in the stack, including all intermediate objects used during calculations that are not
 * visible on the stack, are stored in TEMPOB.
 *
 * USEROB stands for USER Objects, and contains objects that the user wants to keep. This region of memory contains objects organized in
 * directories and subdirectories, being HOME the root directory. Objects stored in this area of memory are referred to by a name, as opposed
 * to objects in TEMPOB which are all unnamed.
 *
 * PORT memory is similar to USEROB, except there are restrictions to its usage: There can only be 2 types of objects (LIB's and BACKUP's) and
 * there are no subdirectories in PORT memory. There are several PORT areas, numbered as 0, 1 and 2. PORT 0 and PORT 1 are in RAM, and PORT 2 
 * is in flash memory. The difference between PORT 1 and PORT 0 is their physical location in memory. In the old Saturn architecture, PORT 0 was
 * directly accessible while PORT 1 was in a 'paged' area of memory, and was therefore only temporarily accessible (slower).
 *
 * \section layout Physical memory layout
 *
 * Both HP49G+ and HP50G have identical memory layout, with one single bank of 512 KB of RAM.
 *
 * Memory was divided by convenience in 128KB banks, which were used as follows:
 *
 *\li The first 128 KB were reserved for the Saturn emulator, and are invisible to the calculator user. HPGCC can use about 90KB from this area.
 *\li The next 128 KB was assigned to PORT 1 (ERAM). HPGCC will take as much as it is available from this area.
 *\li The following 256KB are the main calculator RAM, which contains TEMPOB, USEROB and PORT 0. HPGCC can use all free memory in this area.
 *
 * Within the main RAM block (the last 256 KB), the first area is used by TEMPOB, then it is followed by free memory, and at the end of the block
 * there is USEROB, followed by PORT 0. This layout is of critical importance for the Saturn module, because the developer needs to be aware that
 * if HPGCC uses all free memory in this area, there will be no space left to either create new objects in TEMPOB (therefore making impossible to
 * push a result to the stack from a C program), or to STO new objects in USEROB. HPGCC provides configuration options to reserve some free space
 * for TEMPOB growth, and also for USEROB growth. C programs using the stack (or USEROB) to store limited amount of data can use these options to
 * reserve some memory and take the rest of the main ram for the heap. For C programs that make intensive use of this area, it is recommended to
 * instruct HPGCC to avoid using any memory from the main block (also through specific configuration options). By doing this, the C program will
 * not interfere at all with the Saturn environment, at the expense of reducing the available memory in the C heap.
 *
 * \section objects Saturn Objects
 *
 * Calculator objects are data structures similar to a C-style struct but accessed in nibbles. The first field in this structure is always
 * a 'Prolog'. A prolog is a 5-nibble (20-bits) number that indicates object type. The prolog is what allows the user to identify objects.
 * The rest of the data is different for each object, and can be found in multiple references so it won't be explained here.
 * In general, the calculator handles objects by making extensive use of pointers.
 *
 * An object can exist individually in TEMPOB memory, in USEROB memory, or embedded within another object (like lists for example).
 *
 * Next: \ref sat3oview
 *
 * \page sat3oview Saturn module basic concepts
 *
 * \section datatypes Data types
 * The entire module uses one data type to refer to calculator objects: the SAT_OBJECT type.
 * A SAT_OBJECT represents a Saturn (calculator) object of any type, which can be stored in any area of memory (TEMPOB, USEROB, or even the
 * C-heap). The type SAT_OBJECT is in fact a form of nibble-aligned pointer, therefore pointer arithmetics work (with certain restrictions) but
 * the usual C operators * and [] will not. This is hardly ever needed since there are high-level functions to perform
 * most of the operations on Saturn objects.
 *
 * \section heapaccess Memory management
 * 
 * Calculator objects can be created in TEMPOB just like the calculator would normally do, but HPGCC can now also create Saturn objects in the C
 * heap. Most functions can work with objects in any memory location in a completely transparent way.
 * There is, however a small difference. Objects in TEMPOB are usually created, used and abandoned there until the next Garbage Collection (GC)
 * is performed. Objects in the C heap need to be free'd when they are not needed anymore, just like any 'malloc'ed memory block in the heap.
 * For this task, the Saturn module includes the function sat3_free(), which will free a memory block in the heap. sat3_free() will also physically
 * free a TEMPOB object, so it is recommended that programmers use it for all objects, regardless of memory location. This will avoid memory
 * leaks in the heap and at the same time it will keep TEMPOB clean, so no GC is needed.
 *
 * One important aspect the programmer needs to consider is the monolithic nature of TEMPOB and USEROB. If an object is free'd in TEMPOB, or an
 * object is PURGE'd from a directory in USEROB, or a new object is STOred, a large memory region is moved to 'close the gap'. The immediate
 * consequence is that after STO, PURGE or sat3_free(), any SAT_OBJECT pointer may become invalid. It is the user's responsibility to keep track
 * of these events, and discard his pointers every time one of the mentioned operations is performed. This doesn't affect objects created in the
 * C heap, where memory blocks are never moved.
 * In TEMPOB, only a GC or sat3_free() can move blocks of memory. In USEROB, creating, deleting or changing the name or size of any object or directory
 * will cause pointers to become invalid. The effect is contained within the memory region being modified, so changes to USEROB will not invalidate pointers
 * in TEMPOB and viceversa.
 *
 * Next: \ref fncreate Prev: \ref archoview
 *
 * \page fncreate Object creation and manipulation
 *
 * The Saturn module includes functions to create Saturn objects from most C-style data types. All object creation functions share a common form:
 *
 * \code
 * SAT_OBJECT sat3_createXXX_YYY(SAT_OBJECT where, ... );
 * \endcode
 *
 * Where XXX is a calculator object type (for example a real), and YYY is a C data type to use as a source to create the object (for example a double).
 *
 * All sat3_create... require as a first argument the parameter 'where' to specify the location where the object will be created. It can be one of the
 * constants \bALLOC_HEAP, \bALLOC_TEMPOB or a pointer to a specific location. When one of the constants is used, a block of memory from the specified region
 * will be allocated (the user will be responsible for freeing the block). A pointer can be used to force the creation of the object at a specific (preallocated)
 * memory location. It is normally used to create objects directly within a container object (a list, for instance), for which memory has been previously allocated.
 *
 * All sat3_create... functions return a SAT_OBJECT pointing to the newly created object or NULL if object creation failed for any reason.
 *
 *\code
 * // EXAMPLES OF sat3_create... FUNCTIONS
 *
 * // CREATE A real FROM A dbl IN TEMPOB, WITH THE VALUE 3.141592
 * SAT_OBJECT myreal = sat3_createreal_dbl(ALLOC_TEMPOB,3.141592);
 *
 * // CREATE A real FROM A int IN THE HEAP, WITH THE VALUE 2007
 * SAT_OBJECT myreal = sat3_createreal_int(ALLOC_HEAP,2007);
 *\endcode
 *
 * The creation of composite objects (list, matrix, and others) for which the size is unknown at creation time requires to specify the maximum size in nibbles.
 * The specified amount of memory will be reserved even if not used. Later, as objects are inserted into (or appended to) the composite object,
 * its actual size will increase up to the maximum size specified at creation. When the maximum size is reached, no more objects can be inserted and
 * such operation will fail. An allocated object cannot be expanded, so if more space is needed a new object needs to be allocated. This operation is not
 * made automatically to ensure all references to the composite and its contents are properly updated (which is left to the user).
 * 
 * \b IMPORTANT: The calculator does not support working with objects in TEMPOB that don't use the entire memory block allocated for them. This means
 * the user must be very careful when creating lists, matrices and other composite objects in TEMPOB. It is therefore recommended practice to create those
 * objects in the C heap, and only copy them to TEMPOB once the composite is finished. If it is needed to create composites directly in TEMPOB, the C program must
 * execute a garbage collection before returning to the calculator. This can be easily achieved by calling sat3_garbcol which uses a special garbage collection procedure
 * that also shrinks all oversized objects to the proper size. Composite objects allocated in the C heap don't need to call for garbage collection.
 * 
 * 
 *
 * There is a complete family of sat3_createXXX_YYY functions for each calculator object and each C data type.
 *
 * \anchor calctypes The syntax for calculator object names (XXX) is:
 * \li \ref grbint 
 * \li \ref grzint 
 * \li \ref grreal 
 * \li \ref grcplx 
 * \li \ref gridnt 
 * \li \ref grcstr 
 * \li \ref grhxs  
 * \li \ref grchar 
 * \li \ref grrarray
 * \li \ref grcarray
 * \li \ref grtag
 * \li \ref grlist
 * \li \ref grmatrix
 *
 *
 * \anchor ctypes The syntax for C data types (YYY) is:
 * \li int
 * \li llong (for LONGLONG)
 * \li dbl (for double)
 * \li xreal (for special XREAL type - see XREAL module documentation)
 * \li decn (for special decNumber type - see decNumber library documentation)
 * \li char
 * \li str (for a C text string char[])
 * \li substr (for a substring within a C string)
 * \li bytes (for an arbitrary set of bytes in a buffer)
 *
 *
 * \section fnconvert Object Conversion functions
 *
 * The complement of the sat3_create... family of functions are the sat3_convert... functions, which can extract information from 
 * calculator objects and convert it to C data types. The general syntax is in one of the forms:
 *
 * \code
 *
 * C-type sat3_convertXXX_YYY(SAT_OBJECT object, ...);
 *
 * BOOL sat3_convertXXX_YYY(SAT_OBJECT object, C-type *ptr, ...);
 *
 * \endcode
 *
 * In the first case, the function converts \ref calctypes "calculator type XXX" into \ref ctypes "type YYY", returning the C-type corresponding to YYY. For example:
 *
 * \code
 * double sat3_convertreal_dbl(myreal);
 * \endcode
 *
 * The second form is used when the converted object cannot be returned directly, either because multiple values need to be returned, as in the case of
 * a complex number, or because memory needs to be allocated and a possible error condition needs to be returned.
 * In any case, the function performs the conversion, storing the result in the pointers passed, and returns TRUE. In the event of an error, it returns
 * FALSE, and no value is written to the given pointers.
 *
 * There is one sat3_convert... routine for every sat3_create... routine.
 *
 * Next: \ref funcref Prev: \ref fncreate
 *
 *
 * \page funcref Function Reference
 *
 * Constants, variables and functions grouped by subject:
 *
 * \li \ref grbint
 * \li \ref grzint
 * \li \ref grreal
 * \li \ref grcplx
 * \li \ref gridnt
 * \li \ref grcstr
 * \li \ref grhxs 
 * \li \ref grchar
 * \li \ref grrarray
 * \li \ref grcarray
 * \li \ref grtag
 * \li \ref grlist
 * \li \ref grmatrix
 * \li \ref grstack
 * \li \ref grtype
 * \li \ref grdir
 * \li \ref grmman
 * \li \ref grerror
 * \li \ref gradv
 * \li \ref grconst
 *
*/

/*!
 * \ingroup satmodule
 * \defgroup grconst General constants, macros, etc.
 *
 * This group includes all general purpose type definitions, constants and preprocessor macros needed
 * by the Saturn module
 *
 * \{
 */
/*!
 * \brief Swap low and high nibbles in a byte
 */
 
#define SWAP4(x) (((x) << 4) | ((x) >> 4))

/*!
 * \brief Align an address \a 'x' to a \a 'a' bit boundary
 */
 
#define ALIGN(x,a) (((unsigned)(x)+(a)-1) & (~((unsigned)(a)-1)))

/*!
 * \brief Standard definition for BOOLEAN type 
 */
 
#ifndef BOOL
#define BOOL int
#endif

/*!
 * \brief The BOOLEAN constant \c FALSE
 */

#ifndef FALSE
#define FALSE 0
#endif

/*!
 * \brief The BOOLEAN constant \c TRUE
 */

#ifndef TRUE
#define TRUE !FALSE
#endif



/*!
 * \ingroup gradv
 * \brief The emulated Saturn CPU registers and status
 */
 
typedef

struct _sat_cpu {
    
    unsigned int     Read[257];
    unsigned int     Write[257];
    unsigned char    unknown_data[260];
    unsigned int    A;
    unsigned int    Ah;
    unsigned int    B;
    unsigned int    Bh;
    unsigned int    C;
    unsigned int    Ch;
    unsigned int    D;
    unsigned int    Dh;
    
    unsigned int    R0;
    unsigned int    R0h;
    unsigned int    R1;
    unsigned int    R1h;
    unsigned int    R2;
    unsigned int    R2h;
    unsigned int    R3;
    unsigned int    R3h;
    unsigned int    R4;
    unsigned int    R4h;
    
    unsigned int    D0;
    unsigned int    D1;
    unsigned int    P;
    unsigned int    ST;
    unsigned int    HST;
    unsigned int    CARRY;
    unsigned int    DECIMAL_MODE;
    
} SAT_CPU, *SAT_PCPU;


/*!
 * \brief Saturn object type.
 *
 * This type defines a unique type that can accomodate all different Saturn objects.
 * All saturn objects are created and manipulated through their SAT_OBJECT descriptor.
 * 
 * \internal
 * The following information is for low-level programmers and not
 * required for final users.
 * This type does not contain the actual object. It can point to Saturn RAM,
 * the C-heap, or the Saturn ROM (for predefined objects).
 * 
 * SAT_OBJECT is a generic address container. This address can be a Saturn
 * pointer or a C-heap pointer, with one of the following formats:
 *
 *     0x000AAAAA     20-bit Saturn address
 *     0x2FFAAAAA     20-bit Saturn address (AAAAA) + Flash bank number (FF)
 *     0x4HHHHHHH     29-bit ARM domain address, shifted left to accomodate 1 extra bit for
 *                    nibble-alignment 
 *     0x5HHHHHHH     29-bit ARM physical ram address, shifted left to accomodate 1 extra bit for
 *                    nibble-alignment 
 *
*/

typedef unsigned int SAT_OBJECT;

// **************************************************************
//                          CONSTANTS
// **************************************************************



/*!
 * \brief Memory allocation constant used by sat3_alloc
 */
#define ALLOC_TEMPOB 0xffffffff
/*!
 * \brief Memory allocation constant used by sat3_alloc
 */
#define ALLOC_HEAP   0xfffffffe

/*!
 * \brief Memory allocation constant used by sat3_free
 */
#define BLOCK_UNREF   -1
/*!
 * \brief Memory allocation constant used by sat3_free
 */
#define BLOCK_REF   1



// ALL PROLOGUE CONSTANTS RENAMED BY CLAUDIO
// TO MATCH THE SYSRPL NAMES 01/08/05

/*!
 * \brief Saturn data prologue: ZINT (Typical Exact Integer)
 */
#define SAT_DOINT 0x2614
/*!
 * \brief Saturn data prologue: STRING
 */
#define SAT_DOCSTR        0x2a2c
/*!
 * \brief Saturn data prologue: REAL
 */
#define SAT_DOREAL        0x2933

/*!
 * \brief Saturn data prologue: EXTENDED REAL
 */
#define SAT_DOEREL        0x2955
 
 /*!
 * \brief Saturn data prologue: UINT (HEX STRING)
 */
#define SAT_DOHXS        0x2a4e
/*!
 * \brief Saturn data prologue: BINT
 */
#define SAT_DOBINT        0x2911
/*!
 * \brief Saturn data prologue: COMPLEX
 */
#define SAT_DOCMP        0x2977
/*!
 * \brief Saturn data prologue: EXTENDED COMPLEX
 */
#define SAT_DOECMP        0x299D
/*!
 * \brief Saturn data prologue: CHAR
 */
#define SAT_DOCHAR        0x29BF
/*!
 * \brief Saturn data prologue: ARRAY
 */
#define SAT_DOARRY        0x29e8
/*!
 * \brief Saturn data prologue: LINKED ARRAY
 */
#define SAT_DOLNKARRY        0x29e8
/*!
 * \brief Saturn data prologue: ID (variable name)
 */
#define SAT_DOIDNT        0x2e48
/*!
 * \brief Saturn data prologue: LAM ID (variable name)
 */
#define SAT_DOLAM        0x2e6D
/*!
 * \brief Saturn data prologue: XLIB (ROM Pointer)
 */
#define SAT_DOROMP        0x2e92
/*!
 * \brief Saturn data prologue: RRP (Directory objects)
 */
#define SAT_DORRP        0x2a96
/*!
 * \brief Saturn data prologue: SYMBOLIC
 */
#define SAT_DOSYMB        0x2AB8
/*!
 * \brief Saturn data prologue: LIST
 */
#define SAT_DOLIST        0x2A74
/*!
 * \brief Saturn data prologue: SECONDARY (Programs)
 */
#define SAT_DOCOL        0x2D9D
/*!
 * \brief Saturn data prologue: GROB
 */
#define SAT_DOGROB        0x2B1E
/*!
 * \brief Saturn composite object terminator: SEMI
 */
#define SAT_SEMI        0x312b
/*!
 * \brief Saturn data prologue: BACKUP
 */
#define SAT_DOBAK        0x2B62
/*!
 * \brief Saturn data prologue: CODE
 */
#define SAT_DOCODE        0x2DCC
/*!
 * \brief Saturn data prologue: UNIT OBJECT
 */
#define SAT_DOEXT        0x2ADA
/*!
 * \brief Saturn data prologue: FLASH PTR
 */
#define SAT_DOFLASHP    0x26AC
/*!
 * \brief Saturn data prologue: LIBRARY
 */
#define SAT_DOLIB        0x2B40
/*!
 * \brief Saturn data prologue: SYMBOLIC ARRAY
 */
#define SAT_DOMATRIX    0x2686
/*!
 * \brief Saturn data prologue: TAGGED OBJECT
 */
#define SAT_DOTAG        0x2AFC
/*!
 * \brief Saturn data prologue: DOAPLET
 */
#define SAT_DOAPLET        0x26D5
/*!
 * \brief Saturn data prologue: LIBDATA
 */
#define SAT_DOEXT0        0x2B88
/*!
 * \brief Saturn data prologue: ACPTR (internal object)
 */
#define SAT_DOACPTR        0x2BAA
#define SAT_DOEXT1      SAT_DOACPTR
/*!
 * \brief Saturn data prologue: DOEXT2
 */
#define SAT_DOEXT2        0x2BCC
/*!
 * \brief Saturn data prologue: DOFONT
 */
#define SAT_DOFONT        0x2BCC

/*!
 * \brief Saturn data prologue: DOEXT3
 */
#define SAT_DOEXT3        0x2BEE
/*!
 * \brief Saturn data prologue: ARMCODE, (ex DOEXT4)
 */
#define SAT_DOARMC        0x2C10
#define SAT_DOEXT4        0x2C10

/*!
 * \brief Saturn data prologue: LNGREAL
 */
#define SAT_DOLNGREAL        0x263A
/*!
 * \brief Saturn data prologue: LNGCOMP
 */
#define SAT_DOLNGCOMP        0x2660


/*!
 * \brief Constant to indicate a tagged object
 */
#define SAT_TAGGED        0x20000000

/*!
 * \brief Constant to indicate an optionally tagged argument
 */
#define SAT_OPTIONAL_TAG  0x40000000

/*!
 * \brief Constant to indicate a any object
 */
#define SAT_ANY        0

/*!
 * \brief Constant to indicate any numeric object (bint, zint, hxs string or real) 
 */
#define SAT_NUMERIC   0x10000000 




// PREDEFINED REALS
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_0        0x2f937
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_1        0x2f94c
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_2        0x2f961
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_3        0x2f976
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_4        0x2f98b
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_5        0x2f9a0
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_6        0x2f9b5
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_7        0x2f9ca
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_8        0x2f9df
/*!
 * \brief Predefined real number in ROM
 */
#define REAL_9        0x2f9f4

// Saturn register save area

/*!
 * \brief Saturn CPU storage area: D0
 */
 
#define SAT_INTRPPTR 0x8076B     /* D0 */

/*!
 * \brief Saturn CPU storage area: D1
 */
 
#define SAT_DSKTOP   0x806F8    /* D1 */

/*!
 * \brief Saturn CPU storage area: B
 */
 
#define SAT_RSKTOP   0x806F3    /* B */

/*!
 * \brief Saturn CPU storage area: D
 */
 
#define SAT_AVMEM    0x80E9B    /* D */

/*!
 * \brief Saturn emu storage area: TEMPTOP
 */
 
#define SAT_TEMPTOP  0x806EE

/*!
 * \brief Saturn emu storage area: TEMPOB
 */
 
#define SAT_TEMPOB  0x806E9

/*!
 * \brief Saturn emu storage area: EDITLINE
 */
 
#define SAT_EDITLINE  0x806FD

/*!
 * \brief Saturn error number: ERROR
 */
 
#define SAT_ERROR 0x80EA5

/*!
 * \brief Saturn emu storage area: TEMPENV
 */
 
#define SAT_TEMPENV 0x80702

/*!
 * \brief Saturn emu storage area: TOUCHTAB
 */
 
#define SAT_TOUCHTAB 0x8070c

/*!
 * \brief Saturn emu storage area: FSTVGERPTR
 */
 
#define SAT_FSTVGERPTR 0x8072f

/*!
 * \brief Saturn emu storage area: CONTEXT
 */
 
#define SAT_CONTEXT 0x8071b

/*!
 * \brief Saturn emu storage area: USEROB
 */
#define SAT_USEROB  0x80711


// OTHER USEFUL SATURN OBJECTS

/*!
 * \brief SysRPL flags: TRUE
 */
#define SAT_TRUE 0x3a81

/*!
 * \brief SysRPL flags: FALSE
 */
#define SAT_FALSE 0x3ac0

/*!
 * \brief Calculator error: Too Few Arguments
 */
#define SATERR_TOOFEWARGS 0x201

/*!
 * \brief Calculator error: Bad Argument Type
 */
#define SATERR_BADARGTYPE 0x202

/*!
 * \brief Calculator error: Bad Argument Value
 */
#define SATERR_BADARGVALUE 0x203

// TODO: ADD ALL CALCULATOR ERRORS HERE!
#define SATERR_INSUFFMEMORY 		0x001
#define SATERR_UNDEFLOCALNAME 		0x003
#define SATERR_UNDEFXLIBNAME 		0x004
#define SATERR_MEMORYCLEAR 			0x005
#define SATERR_INVALIDCARDDATA 		0x008
#define SATERR_OBJECTINUSE 			0x009
#define SATERR_PORTNOTAVAILABLE		0x00A
#define SATERR_NOROOMINPORT			0x00B
#define SATERR_NOROOMTOSAVESTACK	0x101
#define SATERR_INVALIDUSERFUNCTION	0X103
#define SATERR_INVALIDSYNTAX		0X106
#define SATERR_WRONGARGUMENTCOUNT	0X128
#define SATERR_NONEMPTYDIR			0X12B
#define SATERR_INVALIDDEFINITION	0X12C
#define SATERR_MISSINGLIBRARY		0X12D
#define SATERR_UNABLETOISOLATE		0X130
#define SATERR_OUTOFMEMORY			0X135
/*!
 * \brief Calculator error: Too Few Arguments
 */
#define SATERR_TOOFEWARGS			0x201

/*!
 * \brief Calculator error: Bad Argument Type
 */
#define SATERR_BADARGTYPE			0x202

/*!
 * \brief Calculator error: Bad Argument Value
 */
#define SATERR_BADARGVALUE			0x203

#define SATERR_UNDEFINEDNAME		0X204
#define SATERR_POSITIVEUNDERFLOW	0X301
#define SATERR_NEGATIVEUNDERFLOW	0X302
#define SATERR_OVERFLOW				0X303
#define SATERR_UNDEFINEDRESULT		0X304
#define SATERR_INFINITERESULT		0X305
#define SATERR_INVALIDDIMENSION		0X501
#define SATERR_INVALIDARRAYELEM		0X502


/*!
 * \}
*/


// **************************************************************
//               GENERAL PURPOSE LOW-LEVEL API
// **************************************************************

/*!
 * \ingroup satmodule
 * \defgroup gradv Advanced and low-level
 *
 * All functions, constants and macros in this group are not part of the user API and its use is NOT recommended.
 * Documentation is provided for sysRPL programmers that need low-level access to Saturn objects.
 * Most of these functions have no error checking. Use at your own risk.
 *
 */
/*!
 * \ingroup satmodule
 * \defgroup grerror Error handling
 *
 * This group includes functions and constants used in error-handling in the Saturn environment
 *
 */
/*!
 * \ingroup satmodule
 * \defgroup grmman Memory management
 *
 * This group includes all functions used to manipulate Saturn memory directly. Most of them are needed only for advanced users.
 *
 * \{
 */


/*!
 * \brief Poke up to 8 nibbles of data to Saturn domain
 *
 * This function stores up to 32 bit data in 4 bit chunks (nibbles) to Saturn memory.
 * \param sat_addr Address to write to in SAT_OBJECT format.
 * \param val Nibbles to write, LSB (little endian) format.
 * \param nibbles Number of nibbles to write
 *
 *
 * \sa sat3_peek sat3_memmove SAT_OBJECT
 */

void sat3_poke(SAT_OBJECT sat_addr,unsigned int val,int nibbles);

/*!
 * \brief Peek up to 8 nibbles of data from Saturn domain
 *
 * This function reads up to 32 bit data in 4 bit chunks (nibbles) from Saturn memory.
 * \param sat_addr Address to read from in SAT_OBJECT format.
 * \param nibbles Number of nibbles to read.
 *
 * \return Nibbles read, in LSB (little endian) format, packed in a 32-bit number.
 *  If less than 8 nibbles were read, the rest are zero-padded.
 *
 * \sa sat3_poke sat3_memmove SAT_OBJECT
 */
 
unsigned int sat3_peek(SAT_OBJECT sat_addr,int nibbles);

/*!
 * \brief Move arbitrary number of nibbles from one location to another.
 *
 * This function moves (copies) the specified number of nibbles from source to
 * destination. Automatically chooses direction of copy to avoid overwriting if
 * dest and source regions overlap.
 *
 * \param dest Destination address in SAT_OBJECT format.
 * \param source Source address in SAT_OBJECT format
 * \param nibbles Number of nibbles to move
 *
 * \sa sat3_memset sat3_poke sat3_peek SAT_OBJECT
*/
 
void sat3_memmove(SAT_OBJECT dest,SAT_OBJECT source,int nibbles);

/*!
 * \brief Fill an area of memory with the given nibble pattern.
 *
 * Use this function to fill an area of memory with a given number. The fill number can have a variable number of nibbles.
 *
 * \param dest Destination address in SAT_OBJECT format.
 * \param value 32-bit number to use as fill
 * \param nibvalue Number of significant nibbles in \a value
 * \param nibbles Number of nibbles to fill
 *
 * \sa sat3_memmove sat3_poke sat3_peek SAT_OBJECT
*/
 
void sat3_memset(SAT_OBJECT dest,int value,int nibvalue,int nibbles);

/*!
 * \brief Allocate arbitrary number of nibbles at the specified location.
 *
 * This function allocates memory either from the Saturn memory or from the C-heap,
 * as requested.
 *
 * \param where Defines where to allocate memory from. Must be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 * \param nibbles Number of nibbles to allocate.
 *
 * \return Allocated address in SAT_OBJECT format, or zero if it fails.
 *
 * \sa sat3_free SAT_OBJECT
*/
 
SAT_OBJECT sat3_alloc(SAT_OBJECT where,int nibbles);

/*!
 * \brief Releases a block of memory allocated by sat3_alloc.
 *
 * If the allocated block is in TEMPOB, it verifies if the block is not referenced and then
 * it removes the block from memory (increasing the available memory).
 * If the allocated block is in the C heap, it calls free() to release its memory.
 *
 * \param block Memory block to release
 *
 * \return It returns BLOCK_FREE on success, BLOCK_USED if block is referenced or INVALID_PTR if error.
 *
 * \sa sat3_alloc sat3_getfreetempob SAT_OBJECT
*/
int sat3_free(SAT_OBJECT block);


/*!
 * \brief Copy an object to the specified location.
 *
 * Copy a valid saturn object from one location to another. If destination is
 * ALLOC_TEMPOB or ALLOC_HEAP, memory will be allocated to contain the new object.
 * When a SAT_OBJECT is given, the copy may fail if there's not enough allocated
 * space at the destination. It's the user responsibility to verify the copy was
 * successful by checking the returned value.
 *
 * \param dest  Where to copy to. Can be a SAT_OBJECT or one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 * \param source Object to copy. Must be a valid saturn object.
 *
 * \return Destination object, or zero if it fails (not enough memory).
 *
 * \sa sat3_memmove SAT_OBJECT
*/
 
SAT_OBJECT sat3_copyobject(SAT_OBJECT dest,SAT_OBJECT source);

/*!
 * \brief Get size in nibbles of a calculator object.
 *
 * \param obj Object to determine size. Must be a valid object.
 * \return Total size of object in nibbles, including prolog.
 */

int sat3_sizeof(SAT_OBJECT obj);

/*!
 * \brief Return the amount of usable tempob memory, in nibbles.
 *
 * The amount of memory returned depends on how the HPGCC program was configured to run, and does not necessarily
 * matches the available memory obtained from the calculator.
 *
 * \sa layout
 *
*/
int sat3_getfreetempob();
/*!
 * \brief Return the amount of usable userob memory, in nibbles.
 *
 * The amount of memory returned depends on how the HPGCC program was configured to run, and does not necessarily
 * matches the available memory obtained from the calculator. This function also gives the free space available for stack
 * growth, since USEROB and the stack allocate memory from the same location.
 *
 * \sa layout
 *
*/
int sat3_getfreeuserob();
/*!
 * \ingroup gradv
 * \brief Create a memory block in TEMPOB.
 *
 * ** LOW-LEVEL ROUTINE - SUPERSEDED BY sat3_alloc **
 * C implementation of the HP49's ROM library routine.
 *
 * \param objsize Size of object in nibbles
 * \return SAT_OBJECT pointer to newly created memory block or \c NULL on error
 * \sa sat3_alloc
 */
SAT_OBJECT sat3_createtemp(int objsize);

/*!
 * \ingroup gradv
 * \brief Skip an object, find next object within a composite.
 *
 * ** LOW-LEVEL ROUTINE - USE HIGHER LEVEL FUNCTIONS FOR COMPOSITES **
 * C implementation of the HP49's ROM library routine SKIPOB.
 *
 * \param object Object to skip. Must be a valid object.
 * \return SAT_OBJECT after the given object
 */
SAT_OBJECT sat3_skipob(SAT_OBJECT object);

/*!
 * \ingroup gradv
 * \brief Skip the name on a named object.
 *
 * ** LOW-LEVEL ROUTINE - DO NOT USE **
 *
 * \param name Object name to skip.
 * \return SAT_OBJECT after the given name
 */
SAT_OBJECT sat3_sknameup(SAT_OBJECT name);

/*!
 * \ingroup gradv
 * \brief Skip the name on a named object, reverse direction.
 *
 * ** LOW-LEVEL ROUTINE - DO NOT USE **
 *
 * \param name Object name to skip.
 * \return SAT_OBJECT before the given name
 */
SAT_OBJECT sat3_sknamedn(SAT_OBJECT name);

/*!
 * \ingroup gradv
 * \brief Read a 4-bit packed BCD number.
 *
 * Read a BCD number of up to 18 digits and converts it to LONGLONG format
 *
 * ** LOW-LEVEL ROUTINE **
 *
 * \param data  SAT_OBJECT pointer to bcd data.
 * \param digits Number of nibbles (digits) to read.
 * \return 64-bit number read (LONGLONG)
 */
LONGLONG sat3_bcd2llong(SAT_OBJECT data,int digits);

/*!
 * \ingroup grerror
 * \brief Set calculator error number.
 *
 * Set the calculator error number, such that when ERRJMP is called the proper
 * error message will be displayed. SysRPL programs can read this error number using ERROR@.
 *
 * ** LOW-LEVEL ROUTINE **
 *
 * \param errno  Calculator error number
 */
void sat3_errorsto(int errno);

/*!
 * \ingroup grerror
 * \brief Set calculator error number and exit the program generating an error in the calculator.
 *
 * Set the calculator error number, then call ERRJMP. 
 *
 * ** LOW-LEVEL ROUTINE **
 *
 * \param errno  Calculator error number
 */

void sat3_exiterror(int errno);



/*!
 * \brief Perform a garbage collection in TEMPOB memory.
 *
 * The user doesn't need to call this function explicitly unless intensive work was done in TEMPOB and cleanup is
 * needed before program exit. However, every time an arbitrary-sized composite object is allocated directly in TEMPOB,
 * this function must be called to shrink such objects to the proper size. 
 *
 * \internal All functions that allocate arbitrary sized objects in TEMPOB must call this function on exit or the calculator will
 * crash. This function repares the TEMPOB object chain to avoid crashing the calculator.
 *
 */
void sat3_garbcol();


/*!
 * \ingroup gradv
 * \brief Convert a SAT_OBJECT pointer into a valid ARM domain address
 *
 * ** LOW-LEVEL ROUTINE - DO NOT USE **
 * This function interprets the value of a SAT_OBJECT pointer and convert it to a suitable byte-aligned ARM address for direct memory access.
 *
 * \param sat_addr Any SAT_OBJECT address, pointing to Saturn RAM, ERAM, FLASH or the C heap.
 *
 * \return 32-bit unsigned integer with the ARM address (can be type-casted directly to a (unsigned char *)
 *
 */
unsigned int sat3_s2a(SAT_OBJECT sat_addr);

/*!
 * \ingroup gradv
 * \brief Get the size in nibbles for objects of a given type, similar to sizeof() in C.
 *
 * 
 * \param prologtype Any SAT_DOXXX constant corresponding to an object prolog.
 *
 * \return Size in nibbles of the given type if the type is fixed-size, or 0 if a 
 *         prolog of a variable-size object is given (or an invalid prolog).
 *
 */

int sat3_typesize(int prologtype);




/*!
 * \}
 */


// **************************************************************
//                      OBJECT CREATION API
// **************************************************************

// *******************************
//         System BINT
// *******************************


/*!
 * \ingroup satmodule
 * \defgroup grbint Bint functions
 *
 * This group includes all functions used to create/convert bints.
 *
 * \{
 */

/*!
 * \brief Create a system bint from an int.
 *
 * This function creates a BINT object (20-bit integer sysRPL BINT) from a 32-bit integer.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value Initial value to assign to the newly created bint. Only the lower 20-bits are used,
 *              the rest is ignored.
 *
 * \return SAT_OBJECT with the new bint, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_convertbint_int \ref fncreate
*/
 
SAT_OBJECT sat3_createbint_int(SAT_OBJECT where,int value);

/*!
 * \brief Convert a system bint into a 32-bit int.
 *
 * This function converts a sysRPL BINT object to an int.
 * 
 * \param bint A SAT_OBJECT bint number.
 *
 * \return int with the given number. If bint < 0 the sign is extended to 32-bit.
 *
 * \sa sat3_createbint_int sat3_convertbint_uint \ref fncreate
*/
 
int sat3_convertbint_int(SAT_OBJECT bint);

/*!
 * \brief Convert a system bint into a 32-bit unsigned integer.
 *
 * This function converts a sysRPL BINT object to an (unsigned int).
 * 
 * \param bint A SAT_OBJECT bint number.
 *
 * \return int with the given number. No sign extension, returned number is < 0x100000
 *
 * \sa sat3_createbint_int sat3_convertbint_int \ref fncreate
*/
 
unsigned int sat3_convertbint_uint(SAT_OBJECT bint);

/*!
 * \}
 */



// *******************************
//              REAL
// *******************************
/*!
 * \ingroup satmodule
 * \defgroup grreal Real functions
 *
 * This group includes all functions used to create/convert reals.
 *
 * \{
 */

/*!
 * \brief Create a real from extended precision real numbers (XREAL).
 *
 * This function creates a real object from an XREAL number.
 * 
 * \param where   Defines where to allocate memory from. Can be one of the following constants:
 *                ALLOC_HEAP     Allocates from the C-heap using malloc()
 *                ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *                or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *                or any other).
 *
 * \param value   Initial value to assign to the newly created real.
 *
 * \return SAT_OBJECT with the new real number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned.
 *
 * \sa sat3_createreal_dbl sat3_createreal_int sat3_createreal_llong sat3_createreal_decn \ref fncreate
*/
 
SAT_OBJECT sat3_createreal_xreal(SAT_OBJECT where,XREAL *value);

/*!
 * \brief Convert a real into an extended precision real (XREAL).
 *
 * This function converts a real object to an XREAL.
 * 
 * \param real    A SAT_OBJECT real number.
 * \param result  Pointer to a previously allocated XREAL.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertreal_dbl sat3_convertreal_int sat3_convertreal_llong sat3_convertreal_decn \ref fncreate
*/
 
BOOL sat3_convertreal_xreal(SAT_OBJECT real, XREAL *result);

/*!
 * \brief Create a real from variable precision real numbers (decNumber).
 *
 * This function creates a real object from a decNumber.
 * 
 * \param where   Defines where to allocate memory from. Can be one of the following constants:
 *                ALLOC_HEAP     Allocates from the C-heap using malloc()
 *                ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *                or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *                or any other).
 *
 * \param value   Initial value to assign to the newly created real.
 *
 * \return SAT_OBJECT with the new real number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned, or zero if it fails.
 *
 * \sa sat3_createreal_dbl sat3_createreal_int sat3_createreal_llong sat3_createreal_xreal \ref fncreate
*/
 
SAT_OBJECT sat3_createreal_decn(SAT_OBJECT where,decNumber *value);

/*!
 * \brief Convert a real into a variable precision real (decNumber).
 *
 * This function converts a real object to a decNumber.
 * 
 * \param real    A SAT_OBJECT real number.
 * \param result  Pointer to a previously allocated decNumber. The number must have been
 *                allocated for 12 digits precision minimum.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertreal_dbl sat3_convertreal_int sat3_convertreal_llong sat3_convertreal_xreal \ref fncreate
*/
 
BOOL sat3_convertreal_decn(SAT_OBJECT real, decNumber *result);

/*!
 * \brief Create a real from double.
 *
 * This function creates a real object from a double.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value Initial value to assign to the newly created real.
 *
 * \return SAT_OBJECT with the new real number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createreal_decn sat3_createreal_int sat3_createreal_llong sat3_createreal_xreal \ref fncreate
*/
 
SAT_OBJECT sat3_createreal_dbl(SAT_OBJECT where,double value);

/*!
 * \brief Convert a real into a double.
 *
 * This function converts a real object to a double.
 * 
 * \param real A SAT_OBJECT real number.
 *
 * \return double with the given real number. When conversion fails it returns 0.0.
 *
 * \sa sat3_convertreal_decn sat3_convertreal_int sat3_convertreal_llong sat3_convertreal_xreal \ref fncreate
*/
 
double sat3_convertreal_dbl(SAT_OBJECT real);

/*!
 * \brief Create a real from an int.
 *
 * This function creates a real object from a 32-bit integer.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value Initial value to assign to the newly created real.
 *
 * \return SAT_OBJECT with the new real number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createreal_decn sat3_createreal_dbl sat3_createreal_llong sat3_createreal_xreal \ref fncreate
*/
 
SAT_OBJECT sat3_createreal_int(SAT_OBJECT where,int value);

/*!
 * \brief Convert a real into a 32-bit int.
 *
 * This function converts a real object to an int. Fractional part is discarded (no rounding).
 * 
 * \param real A SAT_OBJECT real number.
 *
 * \return int with the given real number. When conversion fails it returns 0.
 *
 * \sa sat3_convertreal_decn sat3_convertreal_dbl sat3_convertreal_llong sat3_convertreal_xreal \ref fncreate
*/
 
int sat3_convertreal_int(SAT_OBJECT real);

/*!
 * \brief Create a real from a LONGLONG.
 *
 * This function creates a real object from a 64-bit integer (LONGLONG).
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value Initial value to assign to the newly created real.
 *
 * \return SAT_OBJECT with the new real number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createreal_decn sat3_createreal_dbl sat3_createreal_int sat3_createreal_xreal \ref fncreate
*/
 
SAT_OBJECT sat3_createreal_llong(SAT_OBJECT where,LONGLONG value);

/*!
 * \brief Convert a real into a 64-bit int.
 *
 * This function converts a real object to a LONGLONG integer. Fractional part is discarded (no rounding).
 * 
 * \param real A SAT_OBJECT real number.
 *
 * \return LONGLONG with the given real number. When conversion fails it returns 0.
 *
 * \sa sat3_convertreal_decn sat3_convertreal_dbl sat3_convertreal_int sat3_convertreal_xreal \ref fncreate
*/
 
LONGLONG sat3_convertreal_llong(SAT_OBJECT real);

/*!
 * \}
 */



// *******************************
//              COMPLEX
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grcplx Complex functions
 *
 * This group includes all functions used to create/convert complex.
 *
 * \{
 */

/*!
 * \brief Create a complex from extended precision real numbers (XREAL).
 *
 * This function creates a complex object from an XREAL number.
 * 
 * \param where   Defines where to allocate memory from. Can be one of the following constants:
 *                ALLOC_HEAP     Allocates from the C-heap using malloc()
 *                ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *                or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *                or any other).
 *
 * \param real   Value to assign to the real part of the complex number.
 * \param imag   Value to assign to the imaginary part of the complex number.
 *
 * \return SAT_OBJECT with the new complex number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned, or zero if it fails.
 *
 * \sa sat3_createcplx_decn sat3_createcplx_dbl sat3_createcplx_int sat3_createcplx_llong \ref fncreate
*/
 
SAT_OBJECT sat3_createcplx_xreal(SAT_OBJECT where,XREAL *real,XREAL *imag);

/*!
 * \brief Convert a complex into two extended precision reals (XREAL).
 *
 * This function converts a complex object to XREAL imaginary and real parts.
 * 
 * \param cplx    A SAT_OBJECT complex number.
 * \param real  Pointer to a previously allocated XREAL that will receive the real part.
 * \param imag  Pointer to a previously allocated XREAL that will receive the imaginary part.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertcplx_decn sat3_convertcplx_dbl sat3_convertcplx_int sat3_convertcplx_llong \ref fncreate
*/
 
BOOL sat3_convertcplx_xreal(SAT_OBJECT cplx, XREAL *real,XREAL *imag);

/*!
 * \brief Create a complex from variable precision real numbers (decNumber).
 *
 * This function creates a complex object from two decNumber's (real and imaginary parts).
 * 
 * \param where   Defines where to allocate memory from. Can be one of the following constants:
 *                ALLOC_HEAP     Allocates from the C-heap using malloc()
 *                ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *                or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *                or any other).
 *
 * \param real    Value to assign to the real part.
 * \param imag    Value to assign to the imaginary part.
 *
 * \return SAT_OBJECT with the new complex number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned, or zero if it fails.
 *
 * \sa sat3_createcplx_xreal sat3_createcplx_dbl sat3_createcplx_int sat3_createcplx_llong \ref fncreate
*/
 
SAT_OBJECT sat3_createcplx_decn(SAT_OBJECT where,decNumber *real,decNumber *imag);

/*!
 * \brief Convert a complex into a variable precision real (decNumber).
 *
 * This function converts a complex object to a pair of decNumber's (real and imaginary parts).
 * 
 * \param cplx    A SAT_OBJECT complex number.
 * \param real    Pointer to a previously allocated decNumber that will receive the real part.
 *                The number must have been allocated for 12 digits precision minimum.
 * \param imag    Pointer to a previously allocated decNumber that will receive the imaginary part.
 *                The number must have been allocated for 12 digits precision minimum.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertcplx_xreal sat3_convertcplx_dbl sat3_convertcplx_int sat3_convertcplx_llong \ref fncreate
*/
 
BOOL sat3_convertcplx_decn(SAT_OBJECT cplx, decNumber *real,decNumber *imag);

/*!
 * \brief Create a complex from two double's (real and imaginary parts).
 *
 * This function creates a complex object from a pair of double's.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param real  Value to assign to the real part.
 * \param imag  Value to assign to the imaginary part.
 *
 * \return SAT_OBJECT with the new complex number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createcplx_xreal sat3_createcplx_decn sat3_createcplx_int sat3_createcplx_llong \ref fncreate
*/
 
SAT_OBJECT sat3_createcplx_dbl(SAT_OBJECT where,double real,double imag);

/*!
 * \brief Convert a complex into a pair of double's (real and imaginary parts).
 *
 * This function converts a complex object to two double numbers.
 * 
 * \param cplx A SAT_OBJECT complex number.
 * \param real Pointer to a previously allocated double that will receive the real part.
 * \param imag Pointer to a previously allocated double that will receive the imaginary part.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertcplx_xreal sat3_convertcplx_decn sat3_convertcplx_int sat3_convertcplx_llong \ref fncreate
*/
 
BOOL sat3_convertcplx_dbl(SAT_OBJECT cplx,double *real, double *imag);

/*!
 * \brief Create a complex from two integers.
 *
 * This function creates a complex object from two 32-bit integer.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param real  Value to assign to the real part.
 * \param imag  Value to assign to the imaginary part.
 *
 * \return SAT_OBJECT with the new complex number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createcplx_xreal sat3_createcplx_decn sat3_createcplx_dbl sat3_createcplx_llong \ref fncreate
*/
 
SAT_OBJECT sat3_createcplx_int(SAT_OBJECT where,int real,int imag);

/*!
 * \brief Convert a complex into two 32-bit integers.
 *
 * This function converts a complex object to a pair of int's. Fractional part is discarded (no rounding).
 * 
 * \param cplx A SAT_OBJECT complex number.
 * \param real Pointer to a previously allocated int that will receive the real part.
 * \param imag Pointer to a previously allocated int that will receive the imaginary part.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertcplx_xreal sat3_convertcplx_decn sat3_convertcplx_dbl sat3_convertcplx_llong \ref fncreate
*/

BOOL sat3_convertcplx_int(SAT_OBJECT cplx,int *real,int *imag);

/*!
 * \brief Create a complex from two LONGLONG integers (real and imaginary parts).
 *
 * This function creates a complex object from two 64-bit integer (LONGLONG).
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param real  Value to assign to the real part.
 * \param imag  Value to assign to the imaginary part.
 *
 * \return SAT_OBJECT with the new complex number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createcplx_xreal sat3_createcplx_decn sat3_createcplx_dbl sat3_createcplx_int \ref fncreate
*/
 
SAT_OBJECT sat3_createcplx_llong(SAT_OBJECT where,LONGLONG real,LONGLONG imag);

/*!
 * \brief Convert a complex into a pair of 64-bit integers.
 *
 * This function converts a complex object to two LONGLONG integers. Fractional part is discarded (no rounding).
 * 
 * \param cplx A SAT_OBJECT complex number.
 * \param real Pointer to a previously allocated LONGLONG that will receive the real part.
 * \param imag Pointer to a previously allocated LONGLONG that will receive the imaginary part.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertcplx_xreal sat3_convertcplx_decn sat3_convertcplx_dbl sat3_convertcplx_int \ref fncreate
*/

BOOL sat3_convertcplx_llong(SAT_OBJECT cplx,LONGLONG *real,LONGLONG *imag);


/*!
 * \}
 */

// *******************************
//              ZINT
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grzint ZINT functions
 *
 * This group includes all functions used to create/convert zints.
 *
 * \{
 */

/*!
 * \brief Create a ZINT from extended precision real numbers (XREAL).
 *
 * This function creates a zint object from an XREAL number.
 * 
 * \param where   Defines where to allocate memory from. Can be one of the following constants:
 *                ALLOC_HEAP     Allocates from the C-heap using malloc()
 *                ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *                or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *                or any other).
 *
 * \param value  Pointer to the value to assign to the newly created number.
 *
 * \return SAT_OBJECT with the new zint number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned, or zero if it fails.
 *
 * \sa sat3_createzint_decn sat3_createzint_dbl sat3_createzint_int sat3_createzint_llong \ref fncreate
*/
SAT_OBJECT sat3_createzint_xreal(SAT_OBJECT where,XREAL *value); 

/*!
 * \brief Convert a zint into an extended precision real (XREAL).
 *
 * This function converts a zint object to XREAL.
 * 
 * \param zint  A SAT_OBJECT zint number.
 * \param value Pointer to a previously allocated XREAL that will receive the real part.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertzint_decn sat3_convertzint_dbl sat3_convertzint_int sat3_convertzint_llong \ref fncreate
*/
BOOL sat3_convertzint_xreal(SAT_OBJECT zint, XREAL *value);

/*!
 * \brief Create a zint from variable precision real numbers (decNumber).
 *
 * This function creates a zint object from a decNumber.
 * 
 * \param where   Defines where to allocate memory from. Can be one of the following constants:
 *                ALLOC_HEAP     Allocates from the C-heap using malloc()
 *                ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *                or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *                or any other).
 *
 * \param value  Pointer to the value to assign to the newly created number.
 *
 * \return SAT_OBJECT with the new zint number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned, or zero if it fails.
 *
 * \sa sat3_createzint_xreal sat3_createzint_dbl sat3_createzint_int sat3_createzint_llong \ref fncreate
*/
SAT_OBJECT sat3_createzint_decn(SAT_OBJECT where,decNumber *value);

/*!
 * \brief Convert a zint into an variable precision real (decNumber).
 *
 * This function converts a zint object to decNumber. Due to the variable precision nature of zints,
 * a decNumber of the proper size will be allocated from the heap.
 * 
 * \param zint   A SAT_OBJECT zint number.
 * \param result Pointer to a decNumber pointer that will receive the address of a newly allocated
 *               decNumber.
 *
 * \return BOOL with TRUE if conversion was successful. When conversion fails it returns FALSE.
 *
 * \sa sat3_convertzint_xreal sat3_convertzint_dbl sat3_convertzint_int sat3_convertzint_llong \ref fncreate
*/

BOOL sat3_convertzint_decn(SAT_OBJECT zint,decNumber **result);

/*!
 * \brief Create a zint from double precision real numbers.
 *
 * This function creates a zint object from a double.
 * 
 * \param where   Defines where to allocate memory from. Can be one of the following constants:
 *                ALLOC_HEAP     Allocates from the C-heap using malloc()
 *                ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *                or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *                or any other).
 *
 * \param value  Value to assign to the newly created number.
 *
 * \return SAT_OBJECT with the new zint number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned, or zero if it fails.
 *
 * \sa sat3_createzint_xreal sat3_createzint_decn sat3_createzint_int sat3_createzint_llong \ref fncreate
*/
 
SAT_OBJECT sat3_createzint_dbl(SAT_OBJECT where,double value);

/*!
 * \brief Convert a zint into a double precision real.
 *
 * This function converts a zint object to a double
 * 
 * \param zint  A SAT_OBJECT zint number.
 *
 * \return double with value if conversion was successful. When conversion fails it returns 0.0
 *
 * \sa sat3_convertzint_decn sat3_convertzint_xreal sat3_convertzint_int sat3_convertzint_llong \ref fncreate
*/
double sat3_convertzint_dbl(SAT_OBJECT zint);


/*!
 * \brief Create a zint from an int.
 *
 * This function creates a zint object from a 32-bit integer.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value Value to assign to the newly created zint.
 *
 * \return SAT_OBJECT with the new zint number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createzint_decn sat3_createzint_dbl sat3_createzint_llong sat3_createzint_xreal \ref fncreate
*/
 
SAT_OBJECT sat3_createzint_int(SAT_OBJECT where,int value);

/*!
 * \brief Convert a zint into a 32-bit int.
 *
 * This function converts a zint object to an int. If the number
 * is greater than (2**31-1), the largest possible integer is returned (2**31-1), similar thing for negatives.
 * 
 * \param zint A SAT_OBJECT zint number.
 *
 * \return int with the given number. When conversion fails (invalid object) it returns 0.
 *
 * \sa sat3_convertzint_decn sat3_convertzint_dbl sat3_convertzint_llong sat3_convertzint_xreal \ref fncreate
*/
 
int sat3_convertzint_int(SAT_OBJECT zint);

/*!
 * \brief Create a zint from a LONGLONG.
 *
 * This function creates a zint object from a 64-bit integer.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value Value to assign to the newly created zint.
 *
 * \return SAT_OBJECT with the new zint number, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createzint_decn sat3_createzint_dbl sat3_createzint_int sat3_createzint_xreal \ref fncreate
*/
 
SAT_OBJECT sat3_createzint_llong(SAT_OBJECT where,LONGLONG value);

/*!
 * \brief Convert a zint into a 64-bit integer.
 *
 * This function converts a real object to an int. If the number
 * is greater than (2**63-1), the largest possible integer is returned (2**63-1), similar thing for negatives.
 * 
 * \param zint A SAT_OBJECT zint number.
 *
 * \return LONGLONG with the given number. When conversion fails (invalid object) it returns 0.
 *
 * \sa sat3_convertzint_decn sat3_convertzint_dbl sat3_convertzint_int sat3_convertzint_xreal \ref fncreate
*/
 
LONGLONG sat3_convertzint_llong(SAT_OBJECT zint);

/*!
 * \brief Get the number of digits in a ZINT
 * 
 * Obtain the number of digits in a zint, not including the sign digit.
 * 
 * \param zint A ZINT integer.
 *
 * \return Number of digits in the given zint, or 0 if not a zint.
 *
*/

#define sat3_zintdigits(zint) (sat3_strsize(zint)-1)

/*!
 * \}
 */

// *******************************
//              CHAR
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grchar CHAR functions
 *
 * This group includes all functions used to create/convert char objects.
 *
 * \{
 */

/*!
 * \brief Create a char object from a char.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value Value to assign to the newly created char object.
 *
 * \return SAT_OBJECT with the new char, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa \ref fncreate
*/

SAT_OBJECT sat3_createchar_char(SAT_OBJECT where,char value);

/*!
 * \brief Convert a char into a calculator char object.
 *
 * \param ochar A SAT_OBJECT char.
 *
 * \return char with the given character. When conversion fails (invalid object) it returns 0.
 *
 * \sa \ref fncreate
*/

char sat3_convertchar_char(SAT_OBJECT ochar);

/*!
 * \}
 */


// *******************************
//              CSTR
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grcstr String functions
 *
 * This group includes all functions used to create/convert (character) string objects.
 *
 * \{
 */

/*!
 * \brief Create a string object from a sequence of bytes.
 * 
 * Useful for strings that include the null character, or to include arbitrary data
 * on a calculator string.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param data  Pointer to the buffer containing the bytes to extract.
 * \param nbytes Number of bytes to include on the string.
 *
 * \return SAT_OBJECT with the new string, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createcstr_str sat3_createcstr_substr \ref fncreate
*/

SAT_OBJECT sat3_createcstr_bytes(SAT_OBJECT where,char *data,int nbytes);

/*!
 * \brief Extract a sequence of bytes from a calculator string.
 * 
 * Useful for strings that include the null character, or to read arbitrary data
 * from a calculator string.
 *
 * \param cstr Calculator string to get bytes from.
 * \param data Pointer to a preallocateed buffer that is to receive the bytes.
 * \param maxbytes Size of the preallocated buffer.
 *
 * \return TRUE if successful, FALSE otherwise. If the size of the string is greater
 * than the buffer size, only up to maxbytes will be extracted.
 *
 * \sa sat3_convertcstr_str sat3_convertcstr_substr sat3_strlen \ref fncreate
*/

BOOL sat3_convertcstr_bytes(SAT_OBJECT cstr,char *data, int maxbytes);

/*!
 * \brief Create a string object from a C-style string.
 * 
 * Create a string object from the given string, without the ending null char.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param string  Pointer to a null-terminated string.
 *
 * \return SAT_OBJECT with the new string, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createcstr_bytes sat3_createcstr_substr \ref fncreate
*/

SAT_OBJECT sat3_createcstr_str(SAT_OBJECT where,char *string);

/*!
 * \brief Extract a C-style string from a calculator string.
 * 
 * Stores the string on a preallocated buffer and appends a null char at the end.
 *
 * \param cstr Calculator string.
 * \param string Pointer to a preallocateed buffer that is to receive the string.
 * \param maxchars Size of the preallocated buffer.
 *
 * \return TRUE if successful, FALSE otherwise. If the size of the string is greater
 * than the buffer size, the string will be truncated at maxchars.
 *
 * \sa sat3_convertcstr_bytes sat3_convertcstr_substr sat3_strlen \ref fncreate
*/

BOOL sat3_convertcstr_str(SAT_OBJECT cstr,char *string, int maxchars);

/*!
 * \brief Create a string object from a C-style string.
 * 
 * Create a string object from a portion of the given string, starting at the given position
 * (0-based index) and with the given length.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param string  Pointer to a null-terminated string.
 * \param startpos Position of the first character to extract. First character is 0.
 * \param nchars Number of characters to extract.
 *
 * \return SAT_OBJECT with the new string, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createcstr_bytes sat3_createcstr_str \ref fncreate
*/

SAT_OBJECT sat3_createcstr_substr(SAT_OBJECT where,char *string, int startpos,int nchars);

/*!
 * \brief Extract a C-style string from a calculator string.
 * 
 * Stores the string on a preallocated buffer and appends a null char at the end. It
 * starts at the given position (0-based index) and copies up to the given number of
 * characters. The buffer size must be be at least nchars+1. 
 * 
 * \param cstr Calculator string.
 * \param string Pointer to a preallocateed buffer that is to receive the string.
 * \param startpos Position of the first character to copy. First char on the string
 *                 is at position 0.
 * \param nchars Number of characters to extract.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \sa sat3_convertcstr_bytes sat3_convertcstr_str \ref fncreate
*/

BOOL sat3_convertcstr_substr(SAT_OBJECT cstr,char *string, int startpos,int nchars);


/*!
 * \brief Get the number of nibbles in a string.
 * 
 * \param obj Calculator string.
 *
 * \return Number of nibbles in the given string, or 0 if not a string.
 *
*/
int sat3_strsize(SAT_OBJECT obj);

/*!
 * \brief Get the number of characters in a string
 * 
 * \param cstr Calculator string.
 *
 * \return Number of characters in the given string, or 0 if not a string.
 *
*/

#define sat3_strlen(cstr)  (sat3_strsize(cstr)>>1)


/*!
 * \}
 */


// *******************************
//              HXS
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grhxs Hex string functions
 *
 * This group includes all functions used to create/convert hex string objects.
 *
 * \{
 */

/*!
 * \brief Create a hex string object from a 32-bit integer.
 * 
 * Creates an 8-nibble hex string containing the integer.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value  Number to insert into the hxs string.
 *
 * \return SAT_OBJECT with the new hexxx string, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createhxs_llong sat3_createchxs_bytes \ref fncreate
*/

SAT_OBJECT sat3_createhxs_int(SAT_OBJECT where,int value);

/*!
 * \brief Extract an integer from a hex string.
 * 
 * Reads the first 8 nibbles of a hex string as a 32-bit integer. If the hex string contains
 * less than 8 nibbles, the number will be zero-padded in the upper nibbles (no sign extension).
 *
 * \param hxs Hex string.
 *
 * \return Number extracted from the hex string, or zero if the object is invalid.
 *
 * \sa sat3_converthxs_bytes sat3_converthxs_llong \ref fncreate
*/

int sat3_converthxs_int(SAT_OBJECT hxs);

/*!
 * \brief Create a hex string object from a 64-bit integer.
 * 
 * Creates an 16-nibble hex string containing the integer.
 * 
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param value  Number to insert into the hxs string.
 *
 * \return SAT_OBJECT with the new hexxx string, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createhxs_int sat3_createchxs_bytes \ref fncreate
*/

SAT_OBJECT sat3_createhxs_llong(SAT_OBJECT where,LONGLONG value);

/*!
 * \brief Extract a 64-bit integer from a hex string.
 * 
 * Reads the first 16 nibbles of a hex string as a 64-bit integer. If the hex string contains
 * less than 16 nibbles, the number will be zero-padded in the upper nibbles (no sign extension).
 *
 * \param hxs Hex string.
 *
 * \return Number extracted from the hex string, or zero if the object is invalid.
 *
 * \sa sat3_converthxs_bytes sat3_converthxs_int \ref fncreate
*/

LONGLONG sat3_converthxs_llong(SAT_OBJECT hxs);

/*!
 * \brief Create a hex string object from a sequence of bytes.
 * 
 * Useful to include arbitrary data on a calculator hex string.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param data  Pointer to the buffer containing the bytes to extract.
 * \param nbytes Number of bytes to include on the hex string.
 *
 * \return SAT_OBJECT with the new hex string, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa sat3_createhxs_int sat3_createhxs_llong \ref fncreate
*/
SAT_OBJECT sat3_createhxs_bytes(SAT_OBJECT where,char *data,int nbytes);

/*!
 * \brief Extract a sequence of bytes from a hex string.
 * 
 * Useful to read arbitrary data from a calculator string.
 *
 * \param hxs Hex string to get bytes from.
 * \param data Pointer to a preallocateed buffer that is to receive the bytes.
 * \param maxbytes Size of the preallocated buffer.
 *
 * \return TRUE if successful, FALSE otherwise. If the size of the string is greater
 * than the buffer size, only up to maxbytes will be extracted.
 *
 * \sa sat3_converthxs_int sat3_converthxs_llong sat3_hxssize \ref fncreate
*/

BOOL sat3_converthxs_bytes(SAT_OBJECT hxs,char *data, int maxbytes);

/*!
 * \brief Get the number of nibbles in a hex string
 * 
 * \param hxs Hex string.
 *
 * \return Number of nibbles in the given hex string, or 0 if not a string.
 *
*/
#define sat3_hxssize(hxs) sat3_strsize(hxs)


/*!
 * \}
 */


// *******************************
//              IDNT
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup gridnt Variable name (ID object) functions
 *
 * This group includes all functions used to create/convert ID objects.
 *
 * \{
 */
 
 
/*!
 * \brief Create an ID object from a C-style string.
 * 
 * Create an ID object from the given string, without the ending null char.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param string  Pointer to a null-terminated string.
 *
 * \return SAT_OBJECT with the new string, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa \ref fncreate
*/
 
SAT_OBJECT sat3_createid_str(SAT_OBJECT where,char *string);


/*!
 * \brief Extract a C-style string from a calculator ID.
 * 
 * Stores the string on a preallocated buffer and appends a null char at the end.
 *
 * \param cstr Calculator string.
 * \param string Pointer to a preallocateed buffer that is to receive the string.
 * \param maxchars Size of the preallocated buffer.
 *
 * \return TRUE if successful, FALSE otherwise. If the size of the ID is greater
 * than the buffer size, the ID will be truncated at maxchars.
 *
 * \sa sat3_convertcstr_bytes sat3_convertcstr_substr sat3_idlen \ref fncreate
*/

BOOL sat3_convertid_str(SAT_OBJECT cstr,char *string, int maxchars);


/*!
 * \brief Get the number of characters in an ID
 * 
 * \param idnt Variable name (ID object).
 *
 * \return Number of characters in the given ID, or 0 if not an ID.
 *
*/

int sat3_idlen(SAT_OBJECT idnt);


/*!
 * \}
 */

// *******************************
//              TAG
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grtag Tagged objects functions
 *
 * This group includes all functions used to create/convert tagged objects.
 *
 * \{
 */


/*!
 * \brief Create a tagged object from a C-style string and an arbitrary object.
 * 
 * Creates a new object, consisting of a copy of the given object, tagged with the
 * given string.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param tag  Pointer to a null-terminated string.
 * \param obj  Object to tag. Must be a valid object.
 *
 * \return SAT_OBJECT with the new tagged object, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa \ref fncreate
*/

SAT_OBJECT sat3_createtag_str(SAT_OBJECT where,char *tag,SAT_OBJECT obj);

/*!
 * \brief Extract a C-style string from a tagged object.
 * 
 * Stores the string on a preallocated buffer and appends a null char at the end.
 *
 * \param tagged Object to extract the tag from.
 * \param string Pointer to a preallocateed buffer that is to receive the string.
 * \param maxchars Size of the preallocated buffer.
 *
 * \return TRUE if successful, FALSE otherwise. If the size of the tag is greater
 * than the buffer size, the tag will be truncated at maxchars.
 *
 * \sa sat3_taglen \ref fncreate
*/

BOOL sat3_converttag_str(SAT_OBJECT tagged,char *string, int maxchars);

/*!
 * \brief Get the number of characters in the tag of a tagged object.
 * 
 * \param tag Tagged object.
 *
 * \return Number of characters in the given tag, or 0 if not a tagged object.
 *
*/
#define sat3_taglen(tag) sat3_idlen(tag)


/*!
 * \brief Isolate the object on a tagged object.
 * 
 * Get a SAT_OBJECT pointer to the object being tagged. The tagged object is not modified in
 * memory, the returned pointer will be within the same tagged object. Only one tag level
 * is stripped, nested tags need to be stripped one by one.
 *
 * \param obj Tagged object to strip tag.
 * \param string Pointer to a preallocateed buffer that is to receive the string.
 * \param maxchars Size of the preallocated buffer.
 *
 * \return SAT_OBJECT pointing to the object, or null if not a tagged object.
 *
*/

SAT_OBJECT sat3_striptag(SAT_OBJECT obj);

/*!
 * \}
 */


// *******************************
//           REAL ARRAYS
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grrarray Real array functions
 *
 * This group includes all functions used to create/convert/manipulate real arrays.
 *
 * \{
 */

/*!
 * \brief Create a real array object with the given dimensions.
 * 
 * Creates a new real array of the given dimensions. To create 1-dimensional vectors set one
 * of the dimensions to zero. If one of the dimensions is one, it creates a 2-dimensional array
 * with one row or one column, instead of a vector. Initially, the array is filled with zeroes.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param rows Number of rows, use zero to create a one-dimensional vector.
 * \param cols Number of columns, use zero to create a one-dimensional vector.
 *
 * \return SAT_OBJECT with the new array object, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa \ref fncreate
*/
SAT_OBJECT sat3_createrarray(SAT_OBJECT where,int rows,int cols);

/*!
 * \brief Set the element of an array to the given value (as a double).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param value Real value to store.
 *
 * \sa sat3_rarrayput_xreal sat3_rarrayput_int sat3_rarrayput_llong sat3_rarrayput_decn 
 * 
 */
void sat3_rarrayput_dbl(SAT_OBJECT array,int row,int col,double value);
/*!
 * \brief Set the element of an array to the given value (XREAL).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param value Real value to store (pointer to a XREAL).
 *
 * \sa sat3_rarrayput_dbl sat3_rarrayput_int sat3_rarrayput_llong sat3_rarrayput_decn 
 * 
 */

void sat3_rarrayput_xreal(SAT_OBJECT array,int row,int col,XREAL *value);

/*!
 * \brief Set the element of an array to the given value (int).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param value Integer value to store.
 *
 * \sa sat3_rarrayput_xreal sat3_rarrayput_dbl sat3_rarrayput_llong sat3_rarrayput_decn 
 * 
 */
void sat3_rarrayput_int(SAT_OBJECT array,int row,int col,int value);
/*!
 * \brief Set the element of an array to the given value (long long).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param value Integer value to store.
 *
 * \sa sat3_rarrayput_xreal sat3_rarrayput_dbl sat3_rarrayput_int sat3_rarrayput_decn 
 * 
 */
void sat3_rarrayput_llong(SAT_OBJECT array,int row,int col,LONGLONG value);
/*!
 * \brief Set the element of an array to the given value (as a decNumber).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param value Real value to store (pointer to a decNumber).
 *
 * \sa sat3_rarrayput_xreal sat3_rarrayput_dbl sat3_rarrayput_llong sat3_rarrayput_int 
 * 
 */
void sat3_rarrayput_decn(SAT_OBJECT array,int row,int col,decNumber *value);

/*!
 * \brief Get the element of an array as a double.
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 *
 * \return (double) with the requested value, zero if coordinates fall outside the array
 * 
 * \sa sat3_rarrayget_xreal sat3_rarrayget_decn sat3_rarrayget_llong sat3_rarrayget_int 
 * 
 */
double sat3_rarrayget_dbl(SAT_OBJECT array,int row,int col);
/*!
 * \brief Get the element of an array as a XREAL.
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param result Pointer to a previously allocated XREAL that will receive the value.
 *
 * \return On return, *result contains the value, or zero if coordinates fall outside the array. The function returns FALSE
 * if coordinates fall outside of array or the array is invalid. Otherwise returns TRUE.
 * 
 * \sa sat3_rarrayget_dbl sat3_rarrayget_decn sat3_rarrayget_llong sat3_rarrayget_int 
 * 
 */
BOOL sat3_rarrayget_xreal(SAT_OBJECT array,int row,int col,XREAL *result);
/*!
 * \brief Get the element of an array as an (int).
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 *
 * \return (int) with the requested value, zero if coordinates fall outside the array.
 * Non-integer values are truncated towards zero.
 * 
 * \sa sat3_rarrayget_xreal sat3_rarrayget_decn sat3_rarrayget_llong sat3_rarrayget_dbl 
 * 
 */
int sat3_rarrayget_int(SAT_OBJECT array,int row,int col);
/*!
 * \brief Get the element of an array as an (long long).
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 *
 * \return (long long) with the requested value, zero if coordinates fall outside the array.
 * Non-integer values are truncated towards zero.
 * 
 * \sa sat3_rarrayget_xreal sat3_rarrayget_decn sat3_rarrayget_dbl sat3_rarrayget_int 
 * 
 */
LONGLONG sat3_rarrayget_llong(SAT_OBJECT array,int row,int col);
/*!
 * \brief Get the element of an array as a variable precision decNumber.
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A real array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param result Pointer to a previously allocated decNumber that will receive the value. 
 * The number should have been allocated for at least for 12 digits precision minimum.
 *
 * \return On return, *result contains the value, or zero if coordinates fall outside the array. The function returns FALSE
 * if coordinates fall outside of array or the array is invalid. Otherwise returns TRUE.
 * 
 * \sa sat3_rarrayget_dbl sat3_rarrayget_xreal sat3_rarrayget_llong sat3_rarrayget_int 
 * 
 */
BOOL sat3_rarrayget_decn(SAT_OBJECT array,int row,int col,decNumber *result);

/*!
* \brief Get the size of a real array object.
* 
* Obtain the number of rows and columns in an array object. A one-dimensional array will return
* 0 columns (one-dimensional arrays are treated as column vectors). A two-dimensional array with a
* single column will return 1 column.
*
* \param array  An array object.
* \param rowptr A pointer to an integer variable that will receive the number of rows.
* \param colptr A pointer to an integer variable that will receive the number of columns.
*
* \return TRUE on a valid real array object. When TRUE, *rowptr and *colptr
* contain a valid number of rows and columns respectively. It returns FALSE otherwise.
*
* \sa sat3_rarrayget_dbl sat3_rarrayget_decn sat3_rarrayget_llong sat3_rarrayget_xreal sat3_rarrayget_int
*/
BOOL sat3_rarraysize(SAT_OBJECT array,int *rowptr,int *colptr);


/*!
 * \}
 */


// *******************************
//          COMPLEX ARRAYS
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grcarray Complex array functions
 *
 * This group includes all functions used to create/convert/manipulate complex arrays.
 *
 * \{
 */

/*!
 * \brief Create a complex array object with the given dimensions.
 * 
 * Creates a new complex array of the given dimensions. To create 1-dimensional vectors set one
 * of the dimensions to zero. If one of the dimensions is one, it creates a 2-dimensional array
 * with one row or one column, instead of a vector. Initially, the array is filled with (0+0i).
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param rows Number of rows, use zero to create a one-dimensional vector.
 * \param cols Number of columns, use zero to create a one-dimensional vector.
 *
 * \return SAT_OBJECT with the new array object, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 *
 * \sa \ref fncreate
*/
SAT_OBJECT sat3_createcarray(SAT_OBJECT where,int rows,int cols);
/*!
 * \brief Set the element of an array to the given value (as a double).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Real part of the value to store.
 * \param ipart Imaginary part of the value to store.
 *
 * \sa sat3_carrayput_xreal sat3_carrayput_int sat3_carrayput_llong sat3_carrayput_decn 
 * 
 */
void sat3_carrayput_dbl(SAT_OBJECT array,int row,int col,double rpart,double ipart);
/*!
 * \brief Set the element of an array to the given value (as XREAL's).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Real part of the value to store (pointer to a XREAL).
 * \param ipart Imaginary part of the value to store (pointer to a XREAL).
 *
 * \sa sat3_carrayput_dbl sat3_carrayput_int sat3_carrayput_llong sat3_carrayput_decn 
 * 
 */
void sat3_carrayput_xreal(SAT_OBJECT array,int row,int col,XREAL *rpart,XREAL *ipart);
/*!
 * \brief Set the element of an array to the given value (as int's).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Real part of the value to store.
 * \param ipart Imaginary part of the value to store.
 *
 * \sa sat3_carrayput_dbl sat3_carrayput_xreal sat3_carrayput_llong sat3_carrayput_decn 
 * 
 */
void sat3_carrayput_int(SAT_OBJECT array,int row,int col,int rpart,int ipart);
/*!
 * \brief Set the element of an array to the given value (as long long's).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Real part of the value to store.
 * \param ipart Imaginary part of the value to store.
 *
 * \sa sat3_carrayput_dbl sat3_carrayput_xreal sat3_carrayput_int sat3_carrayput_decn 
 * 
 */
void sat3_carrayput_llong(SAT_OBJECT array,int row,int col,LONGLONG rpart,LONGLONG ipart);
/*!
 * \brief Set the element of an array to the given value (as decNumber's).
 * 
 * Set the element at the given row and column to the specified value. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Real part of the value to store (pointer to a decNumber).
 * \param ipart Imaginary part of the value to store (pointer to a decNumber).
 *
 * \sa sat3_carrayput_dbl sat3_carrayput_int sat3_carrayput_llong sat3_carrayput_xreal
 * 
 */
void sat3_carrayput_decn(SAT_OBJECT array,int row,int col,decNumber *rpart,decNumber *ipart);
/*!
 * \brief Get the element of an array in double format.
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Pointer to a variable that will receive the real part.
 * \param ipart Pointer to a variable that will receive the imaginary part.
 *
 * \return On return, *rpart and *ipart contain the requested value, or (0+0i) if coordinates fall outside the array.
 * The function returns FALSE if cordinates fall outside of array or the array is invalid. Otherwise returns TRUE.
 * 
 * \sa sat3_carrayget_xreal sat3_carrayget_decn sat3_carrayget_llong sat3_carrayget_int 
 * 
 */

BOOL sat3_carrayget_dbl(SAT_OBJECT array,int row,int col,double *rpart,double *ipart);
/*!
 * \brief Get the element of an array in XREAL format.
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Pointer to a variable that will receive the real part.
 * \param ipart Pointer to a variable that will receive the imaginary part.
 *
 * \return On return, *rpart and *ipart contain the requested value, or (0+0i) if coordinates fall outside the array.
 * The function returns FALSE if cordinates fall outside of array or the array is invalid. Otherwise returns TRUE.
 * 
 * \sa sat3_carrayget_dbl sat3_carrayget_decn sat3_carrayget_llong sat3_carrayget_int 
 * 
 */

BOOL sat3_carrayget_xreal(SAT_OBJECT array,int row,int col,XREAL *rpart,XREAL *ipart);
/*!
 * \brief Get the element of an array in (int) format.
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Pointer to a variable that will receive the real part.
 * \param ipart Pointer to a variable that will receive the imaginary part.
 *
 * \return On return, *rpart and *ipart contain the requested value, or (0+0i) if coordinates fall outside the array.
 * The function returns FALSE if cordinates fall outside of array or the array is invalid. Otherwise returns TRUE.
 * 
 * \sa sat3_carrayget_dbl sat3_carrayget_decn sat3_carrayget_llong sat3_carrayget_xreal
 * 
 */
BOOL sat3_carrayget_int(SAT_OBJECT array,int row,int col,int *rpart,int *ipart);
/*!
 * \brief Get the element of an array in (long long) format.
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Pointer to a variable that will receive the real part.
 * \param ipart Pointer to a variable that will receive the imaginary part.
 *
 * \return On return, *rpart and *ipart contain the requested value, or (0+0i) if coordinates fall outside the array.
 * The function returns FALSE if cordinates fall outside of array or the array is invalid. Otherwise returns TRUE.
 * 
 * \sa sat3_carrayget_dbl sat3_carrayget_decn sat3_carrayget_int sat3_carrayget_xreal
 * 
 */
BOOL sat3_carrayget_llong(SAT_OBJECT array,int row,int col,LONGLONG *rpart,LONGLONG *ipart);
/*!
 * \brief Get the element of an array in (decNumber) format.
 * 
 * Get the element at the given row and column. Array coordinates
 * are zero-based, the uppermost element is (0,0). One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 * 
 * \param array A complex array object.
 * \param row  Row coordinate of the element,zero-based.
 * \param col  Column coordinate, zero-based.
 * \param rpart Pointer to a variable that will receive the real part. The number must have been
 * allocated for at least 12 digits minimum precision.
 * \param ipart Pointer to a variable that will receive the imaginary part.  The number must have been
 * allocated for at least 12 digits minimum precision.
 *
 * \return On return, *rpart and *ipart contain the requested value, or (0+0i) if coordinates fall outside the array.
 * The function returns FALSE if cordinates fall outside of array or the array is invalid. Otherwise returns TRUE.
 * 
 * \sa sat3_carrayget_dbl sat3_carrayget_llong sat3_carrayget_int sat3_carrayget_xreal
 * 
 */

BOOL sat3_carrayget_decn(SAT_OBJECT array,int row,int col,decNumber *rpart,decNumber *ipart);

/*!
* \brief Get the size of a complex array object.
* 
* Obtain the number of rows and columns in an array object. A one-dimensional array will return
* 0 columns (one-dimensional arrays are treated as column vectors). A two-dimensional array with
* a single column will return 1 column.
*
* \param array  An array object.
* \param rowptr A pointer to an integer variable that will receive the number of rows.
* \param colptr A pointer to an integer variable that will receive the number of columns.
*
* \return TRUE on a valid complex array object. When TRUE, *rowptr and *colptr
* contain a valid number of rows and columns respectively. It returns FALSE otherwise.
*
* \sa sat3_carrayget_dbl sat3_carrayget_decn sat3_carrayget_llong sat3_carrayget_xreal sat3_carrayget_int
*/
BOOL sat3_carraysize(SAT_OBJECT array,int *rowptr,int *colptr);



/*!
 * \}
 */


// *******************************
//            MATRICES
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grmatrix Matrix functions
 *
 * This group includes all functions used to create/convert/manipulate matrices.
 *
 * \{
 */
 
 /*!
 * \brief Create a matrix object with the given dimensions.
 * 
 * Creates a new matrix of the given dimensions. To create 1-dimensional vectors set one
 * of the dimensions to zero. If one of the dimensions is one, it creates a 2-dimensional array
 * with one row or one column, instead of a vector. Initially, the matrix is filled with the given
 * fill object.
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 *
 * \param rows Number of rows, use zero to create a one-dimensional vector.
 * \param cols Number of columns, use zero to create a one-dimensional vector.
 * \param fillobj Object to fill all the elements of the new matrix
 * \param allocsize Size in nibbles to allocate for the matrix. 
 *
 * \return SAT_OBJECT with the new array object, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 * 
 * \note \b IMPORTANT - This function requires a garbage collection before program exit if ALLOC_TEMPOB was used.
 * See \ref fnncreate and sat3_garbcol for details.
 *
 * \sa \ref fncreate
*/
 
SAT_OBJECT sat3_creatematrix(SAT_OBJECT where,int rows,int cols,SAT_OBJECT fillobj,int allocsize );

 /*!
 * \brief Get a pointer to a specific row of a matrix.
 * 
 * Get a SAT_OBJECT pointing to a specific row within a matrix object. Each row is itself a one-dimensional matrix object,
 * which can be manipulated with the same functions as its matrix container.
 * If the matrix given is one-dimensional (a vector), it returns the nth element of the vector.
 *
 * \param matrix A matrix object.
 * \param row Row number, first row is zero.
 *
 * \return SAT_OBJECT with the requested row, or zero if it fails.
 * It can fail by giving an invalid matrix, a valid matrix with invalid dimensions, or an invalid index.
 *
 * \sa sat3_matrixget sat3_matrixfirst sat3_matrixnext
*/
SAT_OBJECT sat3_matrixgetrow(SAT_OBJECT matrix,int row);

 /*!
 * \brief Get a pointer to a specific element of a matrix.
 * 
 * Get a SAT_OBJECT pointing to a specific element within a matrix object. One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 *
 * \param matrix A matrix object.
 * \param row Row number, first row is zero.
 * \param col Column number, first column is zero.
 *
 * \return SAT_OBJECT with the requested element, or zero if it fails.
 * It can fail by giving an invalid matrix, a valid matrix with invalid dimensions, or invalid indices.
 *
 * \sa sat3_matrixgetrow sat3_matrixfirst sat3_matrixnext sat3_matrixput
*/
SAT_OBJECT sat3_matrixget(SAT_OBJECT matrix,int row, int col);

 /*!
 * \brief Get a pointer to the first element of a matrix.
 * 
 * Get a SAT_OBJECT pointing to the first element within a matrix object. If the matrix is one-dimensional
 * it returns the first element, if it's two-dimensional it returns the first row (the matrix is treated as
 * a one-dimensional array of one-dimensional vector elements).
 * 
 *
 * \param obj A matrix object, can be a row of a matrix.
 *
 * \return SAT_OBJECT with the first element, or zero if it fails.
 * It can fail by giving an invalid object.
 *
 * \sa sat3_matrixgetrow sat3_matrixget sat3_matrixnext
*/
SAT_OBJECT sat3_matrixfirst(SAT_OBJECT obj);

 /*!
 * \brief Get a pointer to the next element of a matrix.
 * 
 * Get a SAT_OBJECT pointing to the next element within a matrix object. Two-dimensional matrices are
 * treated as an array of rows, which are in turn one-dimensional matrices with elements. Therefore,
 * sat3_matrixnext(any_element) will return the next element within the same row, null if the end of
 * the row is reached. sat3_matrixnext(any_row) will return the next row, or null if the end of the 
 * matrix is reached.
 *
 * \param obj A matrix element, can be a row of a matrix or an element within a row.
 *
 * \return SAT_OBJECT with the first element, or zero if it fails.
 * It can fail by giving an invalid object.
 *
 * \sa sat3_matrixgetrow sat3_matrixget sat3_matrixfirst
*/
SAT_OBJECT sat3_matrixnext(SAT_OBJECT obj);
 /*!
 * \brief Get the size of a matrix object.
 * 
 * Obtain the number of rows and columns in a matrix object. A one-dimensional matrix will return
 * 0 columns (one-dimensional matrices are treated as column vectors).
 *
 * \param matrix A matrix object.
 * \param rowptr A pointer to an integer variable that will receive the number of rows.
 * \param colptr A pointer to an integer variable that will receive the number of columns.
 *
 * \return TRUE on a valid matrix object with valid dimensions. When TRUE, *rowptr and *colptr
 * contain a valid number of rows and columns respectively. It returns FALSE otherwise.
 *
 * \sa sat3_matrixgetrow sat3_matrixget sat3_matrixget sat3_matrixnext
*/
BOOL sat3_matrixsize(SAT_OBJECT matrix,int *rowptr,int *colptr);

 /*!
 * \brief Insert a row on a matrix
 * 
 * Add a row in a matrix at the specified position, filled with the specified filler object.
 *
 * \param matrix A matrix object.
 * \param rowpos Row at which the new row will be inserted. The new row will be inserted before the given row.
 * \param fillobj An arbitrary filler object that will be used to populate the new row.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \sa sat3_matrixrowremove
*/

BOOL sat3_matrixrowinsert(SAT_OBJECT matrix,int rowpos,SAT_OBJECT fillobj);

 /*!
 * \brief Delete a row on a matrix
 * 
 * Remove a row in a matrix at the specified position.
 *
 * \param matrix A matrix object.
 * \param rowpos Row that will be deleted.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \sa sat3_matrixrowinsert
*/
BOOL sat3_matrixrowremove(SAT_OBJECT matrix,int rowpos);
 /*!
 * \brief Insert a column on a matrix
 * 
 * Add a column in a matrix at the specified position, filled with the specified filler object.
 *
 * \param matrix A matrix object.
 * \param colpos Column at which the new column will be inserted. The new column will be inserted before the given one. Zero-based index.
 * \param fillobj An arbitrary filler object that will be used to populate the new column.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \sa sat3_matrixcolremove
*/

BOOL sat3_matrixcolinsert(SAT_OBJECT matrix,int colpos,SAT_OBJECT fillobj);

/*!
 * \brief Delete a column on a matrix
 * 
 * Remove a column in a matrix at the specified position.
 *
 * \param matrix A matrix object.
 * \param colpos Column that will be deleted. Zero-based index.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \sa sat3_matrixcolinsert
*/
BOOL sat3_matrixcolremove(SAT_OBJECT matrix,int colpos);

/*!
 * \brief Put an element on the matrix.
 * 
 * Replace the element at the specified coordinates with the given object. One-dimensional vectors can be treated as
 * either column-vectors or row-vectors. Simply set the row or column coordinate to zero.
 *
 * \param matrix A matrix object.
 * \param row Row number of the element. Zero-based index.
 * \param col Column number of the element. Zero-based index.
 * \param obj Arbitrary object to store inside the matrix.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \sa sat3_matrixget
*/
BOOL sat3_matrixput(SAT_OBJECT matrix,int row,int col,SAT_OBJECT obj);


/*!
 * \}
 */


// *******************************
//            SYMBOLICS
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grsymb Symbolic object functions
 *
 * This group includes all functions used to create/convert/manipulate symbolic expressions.
 *
 * \{
 */

/*!
 * \}
 */


// *******************************
//            GROBS
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grgrob Graphic object functions
 *
 * This group includes all functions used to create/convert grobs.
 *
 * \{
 */


/*!
 * \}
 */


// *******************************
//            LISTS
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grlist List functions
 *
 * This group includes all functions used to create/convert/manipulate lists.
 *
 * \{
 */

/*!
 * \brief Create an empty list object.
 * 
 * Creates a new list, initially empty, and allocates the specified amount of memory to let the list grow
 * as new objects are inserted/appended.
 * Lists can be filled in two different ways. One involves inserting/appending existing objects into the list, the other method
 * creates the object directly within a list. Sample code for both methods follows.
 * 
 *
 * \param where Defines where to allocate memory from. Can be one of the following constants:
 *              ALLOC_HEAP     Allocates from the C-heap using malloc()
 *              ALLOC_TEMPOB   Allocates memory from the TEMPOB calculator area
 *              or... it can be an element (SAT_OBJECT) within a composite object, (array, list
 *              or any other).
 * \param allocsize Size in nibbles to allocate for the list. 
 *
 * \return SAT_OBJECT with the new list object, or zero if it fails. When a SAT_OBJECT is
 *         given to the 'where' parameter, the same SAT_OBJECT is returned or 0 if error.
 * 
 * \note \b IMPORTANT - This function requires a garbage collection before program exit if ALLOC_TEMPOB was used.
 * See \ref fnncreate and sat3_garbcol for details.
 *
 * \sa \ref fncreate
 * 
 * \code
// LIST CREATION: DIRECT OBJECT CREATION METHOD
// THIS SAMPLE CODE WILL CREATE A LIST WITH 3 REAL NUMBERS { 1. 2. 3. }

SAT_OBJECT mylist=sat3_createlist(ALLOC_HEAP,1000);  // create the list on the C heap and reserve 1000 nibbles
if(!mylist) exit(0);      // failed to create

// HERE: mylist = { }

SAT_OBJECT position=sat3_listhead(mylist);		// obtain the head of the list

if(!sat3_createreal_int(position,1)) exit(0);          // create the real directly within the list, check for failure

// HERE: mylist = { 1. [no closing bracket, the list is now invalid until creation is finished]

position=sat3_listnext(position);           // advance to the next position (after the number 1.)

if(!sat3_createreal_int(position,2)) exit(0);          // create the real directly within the list, check for failure

// HERE: mylist = { 1. 2. [no closing bracket]

position=sat3_listnext(position);           // advance to the next position (after the number 2.)

if(!sat3_createreal_int(position,3)) exit(0);          // create the real directly within the list, check for failure

// HERE: mylist = { 1. 2. 3. [no closing bracket]

position=sat3_listnext(position);           // advance to the next position (after the number 3.)

if(!sat3_listclose(position)) exit(0);          // close the list, check for failure

// HERE: mylist = { 1. 2. 3. }

// FROM THIS POINT ON, THE LIST IS VALID AGAIN AND ALL sat3_list... FUNCTIONS CAN BE SAFELY USED
// DON'T USE ANY OF THE sat3_list... FUNCTIONS UNTIL THE LIST IS VALID OR RESULTS WILL BE UNPREDICTABLE

\endcode
 *
 * \code
// LIST CREATION: EXISTING OBJECTS CREATION METHOD
// THIS SAMPLE CODE WILL CREATE A LIST WITH 3 REAL NUMBERS { 1. 2. 3. }

SAT_OBJECT mylist=sat3_createlist(ALLOC_HEAP,1000);  // create the list on the C heap and reserve 1000 nibbles
if(!mylist) exit(0);      // failed to create

// HERE: mylist = { }

// FIRST, CREATE THE NUMBERS

SAT_OBJECT one=sat3_createreal_int(ALLOC_HEAP,1);
SAT_OBJECT two=sat3_createreal_int(ALLOC_HEAP,2);
SAT_OBJECT three=sat3_createreal_int(ALLOC_HEAP,3);

if(!one || !two || !three) exit(0);		// check for failure

if(!sat3_listappend(mylist,one)) exit(0);          // append the number, check for failure

// HERE: mylist = { 1. } [the list is still valid]

if(!sat3_listappend(mylist,two)) exit(0);          // append the number, check for failure

// HERE: mylist = { 1. 2. } [the list is still valid]

if(!sat3_listappend(mylist,three)) exit(0);          // append the number, check for failure

// HERE: mylist = { 1. 2. 3. } [the list is still valid]

// USING THIS PROCEDURE, THE LIST IS VALID AT ALL TIMES. THE FUNCTIONS sat3_listremove, sat3_listinsert
// CAN ALSO BE USED DURING THIS PROCESS, AS WELL AS ALL OTHERR sat3_list... FUNCTIONS

// NOW RELEASE THE TEMPORARY MEMORY ALLOCATED FOR THE NUMBERS

sat3_free(one);
sat3_free(two);
sat3_free(three);


\endcode

 * 
*/
SAT_OBJECT sat3_createlist(SAT_OBJECT where,int allocsize );

/*!
 * \brief Get the first object (head) on a list.
 * 
 * Obtain the first object on a list. If the list is empty, the
 * returned SAT_OBJECT pointer will be pointing to the closing bracket, and will be equal to
 * the pointer returned by the sat3_tail function.
 *
 * \param list A list object. 
 *
 * \return SAT_OBJECT with the first object on the list, or zero if not a list.
 * 
 * \sa \ref sat3_listnext sat3_listtail
*/
SAT_OBJECT sat3_listhead(SAT_OBJECT list);
/*!
 * \brief Get the next object on a list.
 * 
 * Obtain the object that follows a given object on a list. If the given object is the last object on the list,
 * a pointer to the closing bracket will be returned. If the given object is the closing bracket, then null will
 * be returned.
 * 
 * The correct sequence to scan a list is:
 * 
 * \code
// ASSUME mylist IS A SAT_OBJECT POINTING TO A LIST CONTAINING { 1 2 3 }

SAT_OBJECT position,tail;

tail=sat3_listtail(mylist);		// tail POINTS TO THE CLOSING BRACKET  { 1 2 3 [tail->] }
position=sat3_listhead(mylist); // position POINTS TO THE HEAD { [position->] 1 2 3 }

while(position!=tail) {

// DO SOMETHING WITH THE OBJECTS HERE

position=sat3_listnext(position);  // position WILL POINT TO THE NEXT OBJECT, ON THE FIRST PASS WILL BE { 1 [position->] 2 3 }

}

 *\endcode
 * 
 *
 * \param obj An object, which must be embedded within a list. 
 *
 * \return SAT_OBJECT with the next object on the list, or zero if at the end of the list.
 * 
 * \sa \ref sat3_listhead sat3_listtail
*/
SAT_OBJECT sat3_listnext(SAT_OBJECT obj);
/*!
 * \brief Get the tail of a list.
 * 
 * Obtain a SAT_OBJECT pointer to the position of the closing bracket on a list. If the list is empty, the
 * returned SAT_OBJECT pointer will be equal to the pointer returned by the sat3_head function.
 *
 * \param list A list object. 
 *
 * \return SAT_OBJECT with the closing bracket of the list, or zero if not a list.
 * 
 * \sa \ref sat3_listnext sat3_listhead
*/

SAT_OBJECT sat3_listtail(SAT_OBJECT list);

/*!
 * \brief Get a pointer to the specified object within a list.
 * 
 * Obtain a specific object from a list. The position is an ordinal, where the first
 * object is zero, one is the second object and so on. The tail (closing bracket) is a valid position.
 * For example:
 * 
 * \code
// ASSUME mylist IS A VALID LIST CONTAINING { 1 2 3 }

SAT_OBJECT one=sat3_listget(mylist,0); // returns a pointer to the first object { [one->] 1 2 3 }
SAT_OBJECT two=sat3_listget(mylist,1); // returns a pointer to the second object { 1 [two->] 2 3 }
SAT_OBJECT three=sat3_listget(mylist,2); // returns a pointer to the third object { 1 2 [three->] 3 }
SAT_OBJECT tail=sat3_listget(mylist,3); // returns a pointer to the fourth object (the tail in this case) { 1 2 3 [tail->] }

// TO SCAN A LIST FROM START TO END, IT'S FASTER TO USE sat3_listnext (RECOMMENDED)
// THE CORRECT CODE TO SCAN A LIST USING sat3_listget IS:

int total=sat3_listcount(mylist);  // return 3 elements on the list
int i;
SAT_OBJECT myelement;

for(i=0;i<total;++i) {             // count from 0 to (total-1) since total would be the closing bracket position
myelement=sat3_listget(mylist,i);  // get all elements sequentially
// DO SOMETHING WITH THE ELEMENTS
}

\endcode
 *
 * \param list A list object. 
 *
 * \return SAT_OBJECT with the requested object on the list, or zero if not a list or invalid position indexx.
 * 
 * \sa \ref sat3_head sat3_listnext sat3_listtail sat3_listcount
*/
SAT_OBJECT sat3_listget(SAT_OBJECT list, int position);
/*!
 * \brief Get the number of elements on a list.
 * 
 * Obtain the number of objects in the given list.
 *
 * \param list A list object. 
 *
 * \return Integer with the number of elements on the list, zero if the list is empty and -1 if not a list.
 * 
 * \sa \ref sat3_listget
*/
int sat3_listcount(SAT_OBJECT list);
/*!
 * \brief Truncate a list.
 * 
 * Inserts a closing bracket at the given position. The position is given as a SAT_OBJECT pointer
 * obtained using sat3_listhead, sat3_listtail, sat3_listnext, sat3_listget. Truncating at the tail
 * has no effect, while truncating at the head turns the list into an empty list.
 *
 * \param position A SAT_OBJECT pointer within a list object. (MUST be within a list, otherwise results are unpredictable) 
 *
 * \return Returns true if successful, false otherwise.
 * 
 * \note \b IMPORTANT - Truncating a list that resides in TEMPOB can cause the calculator to crash, unless a garbage collection
 * is performed before program exit. Working on the C heap does not require garbage collection.
 * See \ref fnncreate and sat3_garbcol for details.
 * \sa \ref sat3_listinsert sat3_listremove sat3_listappend
*/

BOOL sat3_listclose(SAT_OBJECT position);
/*!
 * \brief Insert an object into a list.
 * 
 * Inserts a copy of the given object at the given position. The position is given as a SAT_OBJECT pointer
 * obtained using sat3_listhead, sat3_listtail, sat3_listnext, sat3_listget. Inserting at the tail
 * causes the same effect as sat3_listappend, while inserting at the head makes the given object the first in the list.
 *
 * \param position A SAT_OBJECT pointer within a list object. (MUST be within a list, otherwise results are unpredictable)
 * \param object Any arbitrary object to insert into the list. 
 *
 * \return Returns true if successful, false otherwise.
 * 
 * \sa \ref sat3_listclose sat3_listremove sat3_listappend
*/
BOOL sat3_listinsert(SAT_OBJECT position,SAT_OBJECT object);
/*!
 * \brief Remove an object from a list.
 * 
 * Remove the object at the given position. The position is given as a SAT_OBJECT pointer
 * obtained using sat3_listhead, sat3_listtail, sat3_listnext, sat3_listget. Removing at the tail
 * will fail.
 *
 * \param position A SAT_OBJECT pointer within a list object. (MUST be within a list, otherwise results are unpredictable)
 *
 * \return Returns true if successful, false otherwise.
 * 
 * \sa \ref sat3_listclose sat3_listinsert sat3_listappend
*/
BOOL sat3_listremove(SAT_OBJECT position);

/*!
 * \brief Insert an object at the end of a list.
 * 
 * Inserts a copy of the given object at the end of the list.
 *
 * \param list A list object.
 * \param object Any arbitrary object to apendd to the list. 
 *
 * \return Returns true if successful, false otherwise.
 * 
 * \sa \ref sat3_listclose sat3_listremove sat3_listappend
*/
BOOL sat3_listappend(SAT_OBJECT list,SAT_OBJECT object);

/*!
 * \}
 */


// *******************************
//            DIRS
// *******************************

/*!
 * \ingroup satmodule
 * \defgroup grdir Directory functions
 *
 * This group includes all functions used to manage directories.
 *
 * \{
 */
 
 
/*!
 * \brief Get HOME directory
 * \return A SAT_OBJECT pointing to the HOME directory
 */
 
SAT_OBJECT sat3_home();

/*!
 * \brief Get the Hidden directory
 * \return A SAT_OBJECT pointing to the Hidden directory
 */
SAT_OBJECT sat3_hiddendir();
/*!
 * \brief Get current work directory
 * \return A SAT_OBJECT pointing to the current directory
 */
SAT_OBJECT sat3_getcwd();
/*!
 * \brief Set the current work directory.
 * 
 * Set the current work directory to the given one.
 *
 * \param dir A directory object.
 *
 * \return Returns true if successful, false otherwise.
 * 
 * \sa sat3_getcwd sat3_chdir
*/
BOOL sat3_setcwd(SAT_OBJECT dir);
/*!
 * \brief Get first object in a directory.
 * 
 * \param dir A directory object.
 *
 * \return Returns a SAT_OBJECT with the first object, or 0 if dir is empty or not a dir.
 * 
 * \sa sat3_findnext
*/
SAT_OBJECT sat3_findfirst(SAT_OBJECT dir);
/*!
 * \brief Get next object in a directory.
 * 
 * Obtain the object that follows a given object in a directory.
 * 
 * \param dir A directory object.
 * \param diritem An item within the directory
 *
 * \return Returns a SAT_OBJECT with the next object, or 0 if end-of-dir is reached or not a dir.
 * 
 * \sa sat3_findfirst
*/
SAT_OBJECT sat3_findnext(SAT_OBJECT dir, SAT_OBJECT diritem);
/*!
 * \brief Get next name of an object in a directory.
 * 
 * Given an object, obtain its name if the object is within a directory.
 * 
 * \param obj An item within a directory
 * \param namebuffer A preallocated buffer that will receive the name
 * \param max_chars Maximum number of characters to copy to the buffer. 
 *
 * \return True if successful, in such case namebuffer contain the null-terminated name. If max_chars is
 * insufficient to contain the full name, the function will fail. It returns false on error.
 * 
 * \sa sat3_getvarnamelen
*/
BOOL sat3_getvarname(SAT_OBJECT obj,char *namebuffer, int max_chars);
/*!
 * \brief Get next length of the name of an object in a directory.
 * 
 * Given an object, obtain its name length if the object is within a directory.
 * 
 * \param obj An item within a directory
 *
 * \return Integer with the number of characters in the name. Null-named variables are legal and will return 0. On error returns -1.
 * 
 * \sa sat3_getvarname
*/
int sat3_getvarnamelen(SAT_OBJECT obj);

/*!
 * \brief Get parent directory.
 * 
 * Obtain the parent directory of a directory. It does not alter the current work directory. Use sat3_setcwd for that.
 * 
 * \param dir A directory object.
 *
 * \return Returns a SAT_OBJECT with the parent dir, or 0 if not a dir.
 * 
 * \sa sat3_setcwd sat3_getcwd
*/
SAT_OBJECT sat3_updir(SAT_OBJECT dir);
/*!
 * \brief Find an object within a dir given its name.
 * 
 * Scan the given directory for the requested name. If found it returns a
 * SAT_OBJECT pointing to the object. The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the given
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param dir A directory object.
 * \param name A variable name.
 *
 * \return Returns a SAT_OBJECT that corresponds to the given name, 0 if not found.
 * 
 * \sa sat3_rclhere sat3_rcl
*/
SAT_OBJECT sat3_rclindir(SAT_OBJECT dir,char *name);

/*!
 * \brief Find an object in the current work directory.
 * 
 * Scan the current work directory for the requested name. If found it returns a
 * SAT_OBJECT pointing to the object. The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the current work
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param name A variable name.
 *
 * \return Returns a SAT_OBJECT that corresponds to the given name, 0 if not found.
 * 
 * \sa sat3_rclindir sat3_rcl
*/
SAT_OBJECT sat3_rclhere(char *name);

/*!
 * \brief Find an object in the current work directory or its parent directories.
 * 
 * Scan the current work directory for the requested name. If found it returns a
 * SAT_OBJECT pointing to the object, if not found it scans the parent directory for the name until the name is found
 * or the HOME directory is reached. The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the current work
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param name A variable name.
 *
 * \return Returns a SAT_OBJECT that corresponds to the given name, 0 if not found.
 * 
 * \sa sat3_rclindir sat3_rclhere
*/
SAT_OBJECT sat3_rcl(char *name);

/*!
 * \brief Get the full path and name of an object.
 * 
 * Given an object in a directory, extract full path and name information.
 * 
 * \param obj An object within a directory.
 * \param pathbuffer A preallocated buffer that will receive the path string.
 * \param maxchars Maximum number of chars that can be stored in the buffer.
 *
 * \return Returns a true if successful, in such case pathbuffer contains the requested null-terminated path string.
 * On error it returns false. If the path is longer than maxchars the function will fail.
 * 
 */
BOOL sat3_getpath(SAT_OBJECT obj,char *pathbuffer,int maxchars);

/*!
 * \brief Delete an object in a directory.
 * 
 * Purge (delete) an object in a directory. It can delete non-empty directories in one call.
 * The function will fail if trying to delete a referenced object, or if the given object contains
 * an object that is referenced from another location (like the stack).
 * 
 * \param varobject An object within a directory.
 *
 * \return Returns a true if successful. On error it returns false, which may happen because the given object
 * is not on a directory or because it's being referenced from another location.
 * 
 * \sa sat3_purge sat3_purgeindir
 */
 
BOOL sat3_purgeobj(SAT_OBJECT varobject);
/*!
 * \brief Delete an object in the current work directory.
 * 
 * Scan the current work directory for the requested name. If found it deletes the object.
 * The function will fail if trying to delete a referenced object, or if the given object contains
 * an object that is referenced from another location (like the stack).
 * The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the current work
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param varpath A variable name, which may contain path information.
 *
 * \return Returns a true if successful. On error it returns false, which may happen because the given object
 * is not on a directory or because it's being referenced from another location.
 * 
 * \sa sat3_purgeindir sat3_purgeobj
*/
BOOL sat3_purge(char *varpath);
/*!
 * \brief Delete an object in the given directory.
 * 
 * Scan the given directory for the requested name. If found it deletes the object.
 * The function will fail if trying to delete a referenced object, or if the given object contains
 * an object that is referenced from another location (like the stack).
 * The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the current work
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param dir A directory object.
 * \param varpath A variable name, which may contain path information.
 *
 * \return Returns a true if successful. On error it returns false, which may happen because the given object
 * is not on a directory or because it's being referenced from another location.
 * 
 * \sa sat3_purgeobj sat3_purge
*/
BOOL sat3_purgeindir(SAT_OBJECT dir,char *varname);
/*!
 * \brief Store an object in the given directory.
 * 
 * Store the given object using the given name in the specified directory. If an object with
 * the same name is found, it is replaced without question. 
 * The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the given
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param dir A directory object.
 * \param obj The object to store.
 * \param name A variable name.
 *
 * \return Returns true if successful, false otherwise. 
 * 
 * \sa sat3_sto
*/
BOOL sat3_stoindir(SAT_OBJECT dir,SAT_OBJECT obj,char *name);
/*!
 * \brief Store an object in the current work directory.
 * 
 * Store the given object using the given name in the current directory. If an object with
 * the same name is found, it is replaced without question. 
 * The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the given
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param obj The object to store.
 * \param name A variable name.
 *
 * \return Returns true if successful, false otherwise. 
 * 
 * \sa sat3_stoindir
*/
BOOL sat3_sto(SAT_OBJECT obj,char *varname);

/*!
 * \brief Create a directory in the current work directory.
 * 
 * Create an empty directory in the current work directory. If an object with
 * the same name is found, it is replaced without question. 
 * The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the given
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param name A directory name.
 *
 * \return Returns true if successful, false otherwise. 
 * 
 * \sa sat3_pgdir sat3_chdir
*/
BOOL sat3_crdir(char *name);
/*!
 * \brief Purge an entrire directory tree.
 * 
 * Given a directory name, it deletes the directory, all its contents and subdirectories.
 * This function is an alias for sat3_purge, which causes the same effect when given a directory name.
 * 
 * \param name A directory name.
 *
 * \return Returns true if successful, false otherwise. 
 * 
 * \sa sat3_crdir sat3_chdir
*/
BOOL sat3_pgdir(char *name);
/*!
 * \brief Change the current work directory, given a directory name.
 * 
 * Change the current dir to the given directory name.
 * The name can contain path information
 * in DOS/Unix fashion. Both forward slash / and backslash \\ are acceptable directory
 * separators. A starting slash begins the search at HOME, otherwise the given
 * directory is searched. Double dots '..' represent the parent directory.
 * Examples of valid paths are: "myVAR", "/myVAR" (at HOME), "/mydir/myVAR", "../mydir/myVAR", "../../myVAR".
 * Null-named variables and directories are acceptable, so "//myVAR" will search in the Hidden directory,
 * and "/mydir/" will try to find a null-named variable in /mydir.
 * 
 * \param dir A directory name.
 *
 * \return Returns true if successful, false otherwise. 
 * 
 * \sa sat3_setcwd sat3_getttcwd sat3_getpath
*/
BOOL sat3_chdir(char *dir);



/*!
 * \}
 */


// **************************************************************
//                    STACK MANIPULATION API
// **************************************************************
/*!
 * \ingroup satmodule
 * \defgroup grstack Stack functions
 *
 * This group includes all functions used to access the calculator stack.
 *
 * \{
 */


/*!
 * \brief Get number of elements on the RPL stack (DEPTH)
 * \return Number of elements on stack or zero if empty
 */
 
int sat3_depth();

/*!
 * \brief Pop one element off the stack (DROP)
 */
 
void sat3_drop();

/*!
 * \brief Pop several elements off the stack (DROPN)
 */
 
void sat3_dropn(int n);

/*!
 * \brief Swap elements on level 1 and 2 (SWAP)
 */
 
void sat3_swap();

/*!
 * \brief Push an object to the stack.
 *
 * If the object to push was created in the C-heap, a copy in TEMPOB will
 * be automatically created.
 *
 * \param object Object to push.
 * \return Returns TRUE if push was successful, FALSE on error
 */
 
BOOL sat3_push(SAT_OBJECT object);

/*!
 * \brief Pop an object from the stack. Stack Level 1 is dropped.
 * \return Object in level 1 of the stack.  Returns NULL if stack is empty.
 */
 
SAT_OBJECT sat3_pop();

/*!
 * \brief Get an object at an arbitrary stack level. Stack remains unaltered.
 * \param level Stack level to pick from (1 = first stack level)
 * \return Object in requested level of the stack.  Returns NULL if requested level doesn't contain an object.
 */
SAT_OBJECT sat3_pick(int level);

/*!
 * \brief C version of the sysRPL ERRORSTO. Sets current error number.
 * \param errno Calculator error message number.
 */

void sat3_errorsto(int errno);

/*!
 * \brief Push saturn flag FALSE if arg is FALSE, TRUE otherwise.
 * \param arg Boolean True/False to push on the calculator stack
 * \return Returns TRUE if push was successful, FALSE on error
 */
BOOL sat3_pushtruefalse(BOOL arg);

/*!
 * \}
 */

// **************************************************************
//                    ARGUMENT AND TYPE CHECKING API
// **************************************************************
/*!
 * \ingroup satmodule
 * \defgroup grtype Argument and type checking functions
 *
 * This group includes all functions used to identify objects.
 *
 * \{
 */

/*!
 * \brief Verify object type.
 *
 * Check if the given object is of the specified type.
 * 
 * \param obj    Object to check type.
 * \param prolog Object type, given by one of the \ref grconst "SAT_DOXXX" constants,
 *               optionally adding one of the constants SAT_TAGGED or SAT_OPTIONAL_TAG.
 *               Can also be the constant SAT_ANY, which will return TRUE for all objects, or
 * 				 the constant SAT_NUMERIC, which represents any numeric type, including bints,
 * 				 zints, hex strings, and reals.
 *               Some examples: SAT_TAGGED | SAT_DOREAL will check for a tagged real.
 *                              SAT_OPTIONAL_TAG | SAT_DOREAL will return TRUE for either
 *                              a real or a tagged real (the tag is optional).
 *                              SAT_TAGGED | SAT_ANY accepts any tagged object, and is
 *                              equivalent to SAT_DOTAG.
 * 								SAT_OPTIONAL_TAG | SAT_NUMERIC accepts Zints, reals, bints and hex strings, even if they are tagged.
 * 
 * \return Returns TRUE if type matches, FALSE otherwise.
 * 
 * \sa sat3_checkargs
 */
BOOL sat3_checktype(SAT_OBJECT obj,unsigned int prolog);

/*!
 * \brief Check for multiple stack arguments.
 *
 * Checks if there are at least 'n' arguments on the stack, then verifies the type
 * of each argument using sat3_checktype. This function must be called with exactly
 * 'n' arguments, each indicating the expected type on each level of the stack (level
 * 1 first, then 2 and up).
 * 
 * \param n    Number of arguments to expect on the stack.
 * \param ...  'n' arguments with object type, given by one of the \ref grconst "SAT_DOXXX" constants,
 *               optionally adding one of the constants SAT_TAGGED or SAT_OPTIONAL_TAG.
 *               Can also be the constant SAT_ANY, which represents all objects, or
 * 				 the constant SAT_NUMERIC, which represents any numeric type, including bints,
 * 				 zints, hex strings, and reals.
 *               Some examples: SAT_TAGGED | SAT_DOREAL will check for a tagged real.
 *                              SAT_OPTIONAL_TAG | SAT_DOREAL will accept either
 *                              a real or a tagged real (the tag is optional).
 *                              SAT_TAGGED | SAT_ANY accepts any tagged object, and is
 *                              equivalent to SAT_DOTAG.
 * 
 * \return Returns TRUE if *ALL* stack arguments match the given types, FALSE otherwise.
 * 
 * \sa sat3_ckn sat3_checkargserror
 */

BOOL sat3_checkargs(int n,... );

/*!
 * \brief Check for multiple stack arguments, and exit with an error if types don't match.
 *
 * Checks if there are at least 'n' arguments on the stack, then verifies the type
 * of each argument using sat3_checktype. This function must be called with exactly
 * 'n' arguments, each indicating the expected type on each level of the stack (level
 * 1 first, then 2 and up).
 * 
 * \param n    Number of arguments to expect on the stack.
 * \param ...  'n' arguments with object type, given by one of the \ref grconst "SAT_DOXXX" constants,
 *               optionally adding one of the constants SAT_TAGGED or SAT_OPTIONAL_TAG.
 *               Can also be the constant SAT_ANY, which represents all objects, or
 * 				 the constant SAT_NUMERIC, which represents any numeric type, including bints,
 * 				 zints, hex strings, and reals.
 *               Some examples: SAT_TAGGED | SAT_DOREAL will check for a tagged real.
 *                              SAT_OPTIONAL_TAG | SAT_DOREAL will accept either
 *                              a real or a tagged real (the tag is optional).
 *                              SAT_TAGGED | SAT_ANY accepts any tagged object, and is
 *                              equivalent to SAT_DOTAG.
 * 
 * \return Returns only if *ALL* stack arguments match the given types, otherwise it does not
 *         return, but exits the program generating the proper error message.
 *
 * \sa
 */

void sat3_checkargserror(int n,... );

/*!
 * \brief Check for enough stack arguments, and protect the calculator stack.
 *
 * Checks if there are at least 'n' arguments on the stack, up to a maximum of 5 arguments.
 * This function also sets the protection on the calculator stack, allowing proper stack recovery
 * after an error generated by sat3_exiterror(). It also allows the C program effect to be undone
 * later. This function *MUST* be called if sat3_exiterror() is used, for proper error handling.
 * Even if no arguments are required, it is good practice to call this function (with n=0).
 * 
 * 
 * \param n    Number of arguments to expect on the stack.
 * 
 * \return Returns TRUE if there are at least 'n' objects on the stack, FALSE otherwise.
 *
 * \sa sat3_cknerror sat3_sheckargs sat3_checkargserror sat3_exiterror
 */

BOOL sat3_ckn(int n);

/*!
 * \brief Check for enough stack arguments, protect the calculator stack, and exit with error.
 *
 * Checks if there are at least 'n' arguments on the stack, up to a maximum of 5 arguments.
 * This function also sets the protection on the calculator stack, allowing proper stack recovery
 * after an error generated by sat3_exiterror(). It also allows the C program effect to be undone
 * later in the calculator. This function *MUST* be called if sat3_exiterror() is used, for proper error handling.
 * Even if no arguments are required, it is good practice to call this function (with n=0).
 * 
 * 
 * \param n    Number of arguments to expect on the stack.
 * 
 * \return Returns only if there are at least 'n' objects on the stack, otherwise it does not return, but
 *         it exits the program with the proper error message (uses sat3_exiterror).
 *
 * \sa sat3_ckn sat3_sheckargs sat3_checkargserror sat3_exiterror
 */

void sat3_cknerror(int n);



/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_isreal(obj) sat3_checktype(obj,SAT_DOREAL)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_iscplx(obj) sat3_checktype(obj,SAT_DOCMP)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_isbint(obj) sat3_checktype(obj,SAT_DOBINT)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_iszint(obj) sat3_checktype(obj,SAT_DOINT)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_ischar(obj) sat3_checktype(obj,SAT_DOCHAR)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_iscstr(obj) sat3_checktype(obj,SAT_DOCSTR)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_ishxs(obj) sat3_checktype(obj,SAT_DOHXS)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_islist(obj) sat3_checktype(obj,SAT_DOLIST)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_ismatrix(obj) sat3_checktype(obj,SAT_DOMATRIX)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_isid(obj) sat3_checktype(obj,SAT_DOIDNT)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_istag(obj) sat3_checktype(obj,SAT_DOTAG)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_issymb(obj) sat3_checktype(obj,SAT_DOSYMB)

/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
#define sat3_isdir(obj) sat3_checktype(obj,SAT_DORRP)


/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */
BOOL sat3_israrray(SAT_OBJECT obj);


/*!
 * \brief Check object type on a given object.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if object is of the specified type, FALSE otherwise.
 */

BOOL sat3_iscarray(SAT_OBJECT obj);

/*!
 * \brief Check if SAT_OBJECT is a PTR to an object in ROM.
 * 
 * \param obj Any saturn object.
 * 
 * \return Returns TRUE if it's not an object but a PTR rom address.
 */
BOOL sat3_isptr(SAT_OBJECT obj);

/*!
 * \}
 */

#include <sat3_common_idioms.h>

#endif // __SAT3_H
