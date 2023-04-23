//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

// $Id: stdint.h 541 2008-01-27 23:17:51Z ingo $

#ifndef __STDINT_H
#define __STDINT_H

/*
 *  Definitions of standard integer types.
 */

typedef unsigned char        uint8_t;
typedef char            int8_t;
typedef unsigned short        uint16_t;
typedef short             int16_t;
typedef unsigned int        uint32_t;
typedef int            int32_t;
#ifdef __GNUC__
typedef unsigned long long     uint64_t;
typedef long long        int64_t;
#else
typedef unsigned __int64     uint64_t;
typedef __int64       int64_t;
#endif

#endif // __STDINT_H


