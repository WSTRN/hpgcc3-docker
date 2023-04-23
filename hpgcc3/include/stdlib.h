//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


/*!    \file    stdlib.h
    \brief    Standard library
*/

#ifndef _HPSTDLIB_H
#define _HPSTDLIB_H


// LONG TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05
#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif



#include <sys.h>

// TODO: Doxygen (ibl)
// abstract PRNG descriptor
typedef unsigned (*RNDFUNC)(int);
typedef void (*SRNDFUNC)(unsigned);

// dispatch pointer
extern RNDFUNC rand_hook;
extern SRNDFUNC srand_hook;

/*! \brief    Constant to use with getheapblockinfo()
 * \sa getheapblockinfo
*/
#define INVALID_PTR 0
/*! \brief    Constant to use with getheapblockinfo()
 * \sa getheapblockinfo
*/
#define BLOCK_USED 1
/*! \brief    Constant to use with getheapblockinfo()
 * \sa getheapblockinfo
*/
#define BLOCK_FREE -1



/*!    \fn    void *malloc(size_t)
    \brief    Standard C library function
*/
void     *malloc(size_t);


/*!    \fn    void free(void *)
    \brief    Standard C library function
*/
void     free(void *);



/*!   \brief    Return available free memory in bytes
*/
int freemem();

/*!    \fn    void *calloc(size_t nelem, size_t elsize)
    \brief     Standard C library function
*/
void     *calloc(size_t, size_t);

/*!    \fn    void *realloc(void *oldp, size_t size)
    \brief     Standard C library function
*/

void * realloc(void *oldp, size_t size);

/*!    \fn    int getheapblockinfo(void *ptr, void **blkstart,size_t *blksize)
 * \brief Get information about a heap pointer.
 *
 * Obtain heap block information, given an arbitrary pointer.
 * 
 * \param ptr        Any pointer to get information, does not necessarily need to be
 *                   pointing to the start of a heap block, pointers within a block
 *                   are acceptable.
 * \param blkstart   Address of a pointer to fill the block start information. If ptr
 *                   falls within a block, its start address will be stored in *blkstart
 * \param blksize   Address of a pointer to fill the block size information. If ptr
 *                   falls within a block, its size will be stored in *blksize
 *
 * \return Status information about the block. Can be one of the following:
 *         INVALID_PTR  = ptr does not fall within a block in the C-heap
 *         BLOCK_USED   = ptr falls within a used block in the C heap, *blkstart and *blksize
 *                        contain a valid address and size respectively.
 *         BLOCK_FREE   = ptr falls within a previously free'd block,  *blkstart and *blksize
 *                        contain a valid address and size respectively.
 *
 * \sa malloc realloc free 
*/	
	
int getheapblockinfo(void *ptr, void **blkstart,size_t *blksize);

// runtime

/*!    \fn    void    exit(int)
    \brief    Standard C library function
*/
void    exit(int);


/*!    \fn    int    atexit(void(*__func)(void))
    \brief    Standard C library function
*/
int    atexit(void(*__func)(void));



// misc

/*!    \fn    unsigned mwc(int bits)    
    \brief    32 bit multiply with carry pseudo RNG
    \retval "bits" bit pseudo random number.
    
    Very fast PRNG with extreme cycle length.
    \warning Don't use for cryptography.
*/

unsigned    mwc(int bits);    // multiply with carry 32 bit pseudo RNG


// misc

/*!    \fn    int rand()    
    \brief    wrapper for concrete PRNG implementation
    If srand() has not been called before the first invocation, srand(1) is
    being called implicitly.
    \retval 31 bit pseudo random number.
    
    \warning Don't use for cryptography. Use srand() first!
*/

#define RAND_MAX 0x7FFFFFFF

int rand(void);
unsigned     int rand_bits(int bits);


// misc

/*!    \fn    void srand(unsigned int seed)    
    \brief    seeds the random sequence with \a seed. 
    If seed == 0, use entropic seeding based on current time.
    In this case, the sequence should be nearly unreproduceable, given a sufficient long
    cycle length of the RNG, which is the case for mwc().
    \param seed The seed value or 0 (zero) for a system time based value.
    \warning Don't use for cryptography.
*/

void    srand(unsigned int seed);    // multiply with carry 32 bit pseudo RNG


int memcmp(const void *s1, const void *s2, size_t n); //Memory Compare
//Al Nov '04 - TODO: Documentation?

/*!
    \fn     void qsort(void *base, size_t n_elements, size_t width,int (*compar)(const void *, const void *))
    \brief  Standard C library function
*/

void
qsort(void *base, size_t n_elements, size_t width,int (*compar)(const void *, const void *));

/*!
    \fn     void * bsearch(register const void *key, register const void *base,
                       register size_t n_elements, register size_t size,
                       int (*compar)(const void *, const void *))
               
    \brief  Standard C library function
*/

void *
bsearch(register const void *key, register const void *base,
    register size_t n_elements, register size_t size,
    int (*compar)(const void *, const void *));
    

#endif 
