//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

#ifndef __FS_PRIV_H
#define __FS_PRIV_H

// PRIVATE HEADER FILE FOR LIBRARY BUILD

typedef int FS_VOLUME;

typedef struct {
FS_VOLUME *Volumes[4];		// ALL MOUNTED VOLUMES
unsigned CurrentVolume:16,CaseMode:8,Init:8;			// CURRENTLY SELECTED VOLUME, Init=1 if FS was initialized
} FS_PUBLIC;


#endif

