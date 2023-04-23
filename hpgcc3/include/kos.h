//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

// $Id: kos.h 517 2007-11-19 04:09:04Z claudio $

#ifndef _KOS_H
#define _KOS_H



typedef unsigned int U32;
typedef int S32;
typedef unsigned short U16;
typedef short S16;
typedef unsigned char U8,*P_U8;
typedef char S8;
typedef char BYTE;
typedef char *P_BYTE;
typedef int BOOL;
typedef void *P_VOID;

#ifndef size_t
#define size_t unsigned int
#endif /* size_t */

#ifndef SIZE_T
#define SIZE_T size_t
#endif


//Al 19th Nov '04
#ifndef NULL
#define NULL 0
#endif

// < ibl ; 2004-10-28 >
#define TIME_T unsigned int 

typedef U32 (*FUNC_PTR)();
typedef U32 (*FUNC0PTR)();
typedef U32 (*FUNC1PTR)(U32);
typedef U32 (*FUNC2PTR)(U32,U32);
typedef U32 (*FUNC3PTR)(U32,U32,U32);



#ifndef FALSE
#define FALSE 0
#define TRUE (! FALSE)
#endif




//! The KOS Date structure
typedef struct
{
    U16 year;    //!< Year
    U8 month;    //!< Month
    U8 day;        //!< Day
}
KDATE;

//! The KOS Time structure
typedef struct
{
    U8 hours;        //!< Hours
    U8 minutes;        //!< Minutes
    U8 seconds;        //!< Seconds
    U8 unknown1;    //!< FIXME
    U32 unknown2;    //!< FIXME
} KTIME;

// The KOS Date&Time Structure (nesting)
typedef struct
{
    KDATE date;        //!< Date in YMD
    KTIME time;        //!< Time in HMS##
}
KDATETIME, *P_KDATETIME;




// memory alloc type define
#define KOS_MEM_FREE                        0x0000
// memory not allocated
#define KOS_MEM_GLOBAL                      0x1000
// global memory type
#define KOS_MEM_LOCAL                       0x2000
// local (task) memory type
#define KOS_MEM_HEAP0                       0x0100
// heap number

#define KOS_MODULE_FILESYSTEM 1

#define KOS_FUNC_FOPEN    0
#define KOS_FUNC_FREAD    1
#define KOS_FUNC_FWRITE    2
#define KOS_FUNC_FCLOSE    3
#define KOS_FUNC_FSEEK    4
#define KOS_FUNC_FDELETE 5
// < Benjamin ; 2005-06-12 >
#define KOS_FUNC_FRENAME        6
#define KOS_FUNC_CHDIR            7
#define KOS_FUNC_GETFILEINFO    8
#define KOS_FUNC_GETFIRSTDIR    9
#define KOS_FUNC_GETNEXTDIR        10
#define KOS_FUNC_GETPREVDIR        11
#define KOS_FUNC_GETFIRSTFILE    12
#define KOS_FUNC_GETNEXTFILE    13
#define KOS_FUNC_GETPREVFILE    14
#define KOS_FUNC_FATTR            15
#define KOS_FUNC_GETSIZE        16
#define KOS_FUNC_CHECKINSERT    17
#define KOS_FUNC_CHECKPROTECT    18



#define KOS_READMODE 1        
#define KOS_WRITEMODE 2

// Filesystem (fs.h)

// we don't have that...
// #include "kgeneraldef.h"

#define FAT32_SUPPORT  1


#define NEW_FILE_NAME

// < Benjamin ; 2005-06-12 >
//! The KOS File info structure
typedef struct 
//__file_dir_info
{
       KDATETIME       last_modifier;
       KDATETIME       create_date;
       KDATE           last_access_date;
#ifdef  NEW_FILE_NAME
       BYTE            file_name[13];    //!< The full filename, including the "." extension
#else
       BYTE            extension[4];
       BYTE            name[9];
#endif
        /** from: http://alumnus.caltech.edu/~pje/dosfiles.html\n
        The bits in the attribute byte determine the type of entry as follows (bit 7 is the most significant bit):
\n
     bit  meaning if bit = 1\n
     ---  ---------------------------------------\n
      7   unused\n
      6   unused\n
      5   file has been changed since last backup (=archive file)\n
      4   entry represents a subdirectory\n
      3   entry represents a volume label\n
      2   system file\n
      1   hidden file\n
      0   read-only\n
\n
    If the entry represents a subdirectory, bit 4 is set and all other bits are unused. If the entry represents a volume label, bit 3 is set and other bits are unused.

    Since DOS file names are case-insensitive, the file name and extension contain no lower-case (small) letters. They are always converted to the corresponding capital letters before writing them to the disk. What DOS will do if it finds small letters in a file name or extension in a directory entry is undefined.
        */
       U8               attribute;    //!< Attributes
       U16             ext_attr;    //!< Extended attribute
       U32             size;        //!< Exact size in bytes
}
FILE_INFO, DIR_INFO, *P_FILE_INFO, *P_DIR_INFO;



typedef S32     H_FILE;

// < Benjamin ; 2005-06-12 >
// Attributes for FILE_INFO/DIR_INFO
//! Read-only bit for read-only attribute field
#define KOS_READONLY_ATTRIBUTE (1)
//! Read-only bit for hidden file attribute field
#define KOS_HIDDEN_ATTRIBUTE (1<<1)
//! Read-only bit for system file attribute field
#define KOS_SYSTEM_ATTRIBUTE (1<<2)
//! Read-only bit for archive attribute field
#define KOS_ARCHIVE_ATTRIBUTE (1<<5)

#define FILE_READONLY   0x00000001      //mode parameter in kos_fopen
#define FILE_READWRITE  0x00000002
#define FILE_APPEND     0x00000003

#define FS_SEEK_SET             0
#define FS_SEEK_CUR             1
#define FS_SEEK_END             2

#define ERR_FS_INVALID_NAME                     -1
#define ERR_FS_FILE_OPENED                      -2
#define ERR_FS_INVALID_HANDLE                   -3
#define ERR_FS_INVALID_INDEX                    -4
#define ERR_FS_INVALID_MODE                     -5
#define ERR_FS_DISK_FULL                        -6
#define ERR_FS_DISK_ERROR                       -7
#define ERR_FS_DISK_CHANGE                      -8
#define ERR_FS_NO_SDCARD                        -9
#define ERR_FS_NO_MEMORY                       -10
#define ERR_FS_NOT_SUPPORT                     -11
#define ERR_FS_PROTECT                         -13
#define ERR_FS_NOTFIND                         -14
#define ERR_FS_DISK_REMOVE                     -15
#define ERR_FS_FILE_NOTEXIST                   -16
#define ERR_FS_UNKNOWN                        -100



// Errors

#define SUCCESS 0
#define EOF -1
#define KOS_ERR_INVALIDPTR    -200 // FIXME: random guess

// Kernel

FUNC_PTR kos_GetFuncEntry(U32 module, U32 index);

// Filesystem

H_FILE kos_fopen(P_BYTE fname, U8 mode);
SIZE_T kos_fread(H_FILE fd, P_BYTE buf, U32 length);
SIZE_T kos_fwrite(H_FILE fd, P_BYTE buf, U32 length);
S32 kos_fseek(H_FILE fd, S32 offset, U32 mode);
S32 kos_fclose(H_FILE fd);
S32 kos_fdelete(P_BYTE fname);


// CHANGED TO A MACRO
#define fdelete(a) kos_fdelete(a);







// < Benjamin ; 2005-06-12 >
//! FIXME: this function does not seem to work as it should (return -200 err every time)
/**
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_frename(P_BYTE oldname, P_BYTE newname);

//! Change system current directory
/**
 * The format of the path is unix-like. For example: / is the root, /ARCHIVES/TEMP is the sub-sub-directory TEMP in ARCHIVE, in /
 * Example:
 \code
 if (kos_chdir("/TEMP")==SUCCESS)
 {
 }
 \endcode
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_chdir(P_BYTE pDir);

//! FIXME: Still confused by this
S32 kos_GetSpecialFileInfo(P_BYTE fname, P_FILE_INFO pInfo);

//! Fill a FILE_INFO structure of the first sub-directory of the current directory
/**
 * Example: see kos_getFirstFileInfo example (change File by Dir).\n
 * \see kos_getFirstFileInfo kos_getNextDirInfo kos_getPrevDirInfo
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_GetFirstDirInfo(P_DIR_INFO pInfo);

//! Fill a DIR_INFO structure of the next 'indexed' sub-directory of the current directory
/**
 * You *must* call kos_getFirstDirInfo(P_DIR_INFO pInfo) first to ensure
 * that the system current file ID is pointing to the first file of the current
 * directory\n
 * \see kos_getFirstDirInfo kos_getPrevDirInfo
 * \param pInfo [out] pointer to structure
 * \param index [in]  index of current sub-directory
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_GetNextDirInfo(P_DIR_INFO pInfo, U32 index);

//! Fill a DIR_INFO structure of the previous 'indexed' sub-directory of the current directory
/**
 * You *must* call kos_getFirstDirInfo(P_DIR_INFO pInfo) first to ensure
 * that the system current file ID is pointing to the first file of the current
 * directory\n
 * \see kos_getFirstDirInfo kos_getNextDirInfo 
 * \param pInfo [out] pointer to structure
 * \param index [in]  index of current sub-directory
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_GetPrevDirInfo(P_DIR_INFO pInfo, U32 index);

//! Fill a FILE_INFO structure of the first file of the current directory
/**
 * \see kos_getNextFileInfo kos_getPrevFileInfo
 * Example:
 \code
void print_file_info(P_FILE_INFO pInfo)
{
    printf("%s : %d bytes\n", pInfo->file_name, pInfo->size);
    printf("Attr: %b %b\n", pInfo->attribute, pInfo->ext_attr);
    printf("Las.Acc: %d %d %d\n", pInfo->last_access_date.day,
           pInfo->last_access_date.month, pInfo->last_access_date.year);
    printf("T: %d %d %d\n", pInfo->create_date.date.day,
           pInfo->create_date.date.month, pInfo->create_date.date.year);
    printf("T: %d %d %d\n", pInfo->create_date.time.hours,
           pInfo->create_date.time.minutes,
           pInfo->create_date.time.seconds);
    printf("T: %d %d %d\n", pInfo->last_modifier.time.hours,
           pInfo->last_modifier.time.minutes,
           pInfo->last_modifier.time.seconds);
}

int main()
{
    FILE_INFO Info;
    if (kos_GetFirstFileInfo(&Info) == SUCCESS)
    {
        print_file_info(&Info);
        keyb_waitKeyPressed();
        unsigned int j;
        for (j = 0; kos_GetNextFileInfo(&Info, j++) == SUCCESS; )
        {
            print_file_info(&Info);
            keyb_waitKeyPressed();
        }
    }
    else
    {
        printf("No files\n");
        keyb_waitKeyPressed();
    }
}
    \endcode
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_GetFirstFileInfo(P_FILE_INFO pInfo);

//! Fill a FILE_INFO structure of the next 'indexed' file of the current directory
/**
 * You *must* call kos_getFirstFileInfo(P_FILE_INFO pInfo) first to ensure
 * that the system current file ID is pointing to the first file of the current
 * directory\n
 * \see kos_getFirstFileInfo kos_getPrevFileInfo
 * \param pInfo [out] pointer to structure
 * \param index [in]  index of current file in the directory
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_GetNextFileInfo(P_FILE_INFO pInfo, U32 index);

//! Fill a FILE_INFO structure of the previous 'indexed' file of the current directory
/**
 * You *must* call kos_getFirstFileInfo(P_FILE_INFO pInfo) first to ensure
 * that the system current file ID is pointing to the first file of the current
 * directory\n
 * \see kos_getFirstFileInfo kos_getNextFileInfo
 * \param pInfo [out] pointer to structure
 * \param index [in]  index of current file in the directory
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_GetPrevFileInfo(P_FILE_INFO pInfo, U32 index);

//! Set file attributes
/**
 * \return SUCCESS (0) if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_fattr(P_BYTE fname, U8 attr);


//! Get disk free size in bytes
/**
 * \see kos_checkDiskProtect kos_checkDiskInsert
 * \return free bytes if ok, KOS_ERR_INVALIDPTR    -200  else.
 */
S32 kos_GetDiskFreeSize(void);

//! Check if a valid SD is inserted
/**
 * \see kos_checkDiskProtect kos_getDiskFreeSize
 * \return 1 if a disk is inserted, 0  else.
 */
S32 kos_CheckDiskInsert(void);

//! Check if a the SD is write-protected
/**
 * \see kos_checkDiskInsert kos_getDiskFreeSize
 * \return 1 if protected, 0 else.
 */
S32 kos_CheckDiskProtect(void);

//! Reset calculator OS file system
/**
 * Call to reinitiate SD card system after using the FSystem library.
 * \see kos_checkDiskInsert kos_getDiskFreeSize
 * \return 1 if protected, 0 else.
 */

void kos_ResetSD(void);






//Edit by Al 19th October
//Include more of HP's KOS functions

//TODO: Add the rest (except maybe the dangerous ones)

//memory management

void *kos_malloc(SIZE_T size); //malloc. ~100kb free on 49g+, <30k free on 39g+
S32 kos_free(void *pMemory); //free
 

//display routines

void kos_ClearLcd(void); //clears the LCD
U32 kos_PutString(U32 dx, U32 dy, P_VOID ptr); //writes a string, in large 5 x 8 font
void kos_PutHexNumber(U32 dx, U32 dy, U32 value); //write a number in hex
void kos_PutDecNumber(U32 dx, U32 dy, U32 value, U32 bit); //writes a number in base-10

U32 kos_LCDGetContrast(void); //gets and sets LCD contrast
void kos_LCDSetContrast(U32 contrast);

//misc routines

P_BYTE kos_getSerialNumber(void); //seems to return garbage, and a Kinpo copyright. 
void kos_Reset(void); // reset OS + emulator?
S32 kos_beep(U32 freq, TIME_T time, U32 override);

// Time

U32 kos_GetSysTickTime();



#endif
