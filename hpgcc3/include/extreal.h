//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


// TYPE DEFINITIONS

// EXTENDED REAL FORMAT:
// BASE-2:
//         * LEFT-JUSTIFIED MANTISSA, BASE-2 EXPONENT
// BASE-10:
//         * INTEGER BASE-10 MANTISSA, BASE-10 EXPONENT

#ifndef _XREAL_H

#define _XREAL_H
/*!
 * \brief Extended real type that can contain a calculator real.
 *
 * This type has a 64-bit mantissa, a 31-bit exponent and 1 bit sign.
 * The total size is 12 bytes (96 bits).
 * Numbers can be stored in two formats:
 *
 * 1) Left justified mantissa with base-2 exponent (number = mantissa * 2^(exponent-63)
 * 2) Right justified mantissa with base-10 exponent (number = mantissa * 10^exponent)
 *
 * The first format is used by all math operations, while the second format is only
 * useful when printing in base 10, since the mantissa is an integer number and the
 * exponent indicates the number of places for the decimal point.
 */

typedef struct {
	/*!
	 * \brief 64-bit Mantissa
	 */
unsigned long long Mant;  // 64-BIT MANTISSA, LEFT OR RIGHT JUSTIFIED
/*!
 * \brief 31-bit exponent (signed)
 */
signed Exp:31; 			// 31-BIT SIGNED EXPONENT
/*!
 * \brief Sign bit
 */
unsigned Sign:1;			    // 1-BIT SIGN
} XREAL;

// MACRO REPLACEMENT FOR TRIVIAL FUNCTIONS

#define xr_xr2uint(num) ((unsigned int)xr_xr2ullong(num))

// GENERAL STRING CONVERSION ROUTINES REPLACED BY MACROS

/*!
 * \brief Data structure used to convert an XREAL number to string.
 *
 * This structure contains all information needed for string conversion,
 * including all format modifiers supported by printf
 */

struct conversion_data {
	/*!
	 * \brief Flags to control string output:
	 *
	 * bit 0 (1) = Left-align when set, right align when clear.
	 * bit 1 (2) = Force sign when set, even for positive numbers
	 * bit 2 (4) = 0 padding when set, blank padding when clear
	 * bit 3 (8) = Add blank space for sign of positive numbers when set,
	 *             otherwise sign of positive numbers does not take space.
	 * bit 4 (16)= When set, adds prefix '0' or '0x' for octal and hex numbers respectively.
	 * bit 5 (32)= Force the display of decimal point when set, even for integer numbers.
	 * bit 6 (64)= Display capital letters in hex numbers when set.
	 */

unsigned int flags;
/*!
 * \brief Total field width, used for right-justified numbers.
*/
int width;
/*!
 * \brief Number of decimal digits to display.
*/
int precision;
/*!
* \brief Base to display the number. Supports any base, but tested only with 2, 8, 10, 16
*/
int base;
/*!
 * \brief Type of number to be displayed.
 *
 * 1 = signed integer
 * 2 = unsigned integer
 * 4 = floating point in 'e' format (see printf formats)
 * 8 = floating point in 'f' format (see printf formats)
 * 12= floating point in 'g' format (see printf formats)
*/
int type;
};

/*!
 * \brief Power of 10 as an XREAL number.
 *
 * This function returns 10 to the indicated power.
 *
 * \param exp10  Desired exponent
 * \param value  Pointer to an XREAL to be filled with the result.
 * \return The XREAL number (10^exp10) written to the given pointer.
 * \sa XREAL
 */
void xr_10pow(int exp10,XREAL *value);
/*!
 * \brief Convert a string to an XREAL number
 *
 * This function converts a number stored in text format to the extended real format.
 *
 * \param str  String with a valid number
 * \param res  Pointer to an XREAL to be filled with the result.
 * \return A pointer into the given string, pointing to the character after the last valid character
 *         considered part of the number. The converted number is written at the given structure.
 * \sa xr_xrtoa
 */
char *xr_atoxr(const char *str,XREAL *res);

/*!
 * \brief Convert an XREAL number to string format.
 *
 * This function converts a number in XREAL format into a string.
 *
 * \param number  The number to be converted
 * \param data  Pointer to a structure with all formatting options needed for the conversion.
 * \return A pointer to a newly allocated string (allocated with malloc, the user is responsible to free
 *         the memory block after use). The converted number follows all the formatting specifications
 *         given in the 'data' argument.
 * \sa xr_atoxr conversion_data
 */

char *xr_xrtoa(XREAL *number, struct conversion_data *data);



/*!
 * \brief Change internal representation to a base-2 system.
 *
 * This function converts a number stored in base-10 format to the base-2 format.
 *
 * \param base10  Pointer to an XREAL number stored in base-10 format
 * \param res  Pointer to an XREAL to be filled with the result.
 * \return Number with a left justified mantissa with base-2 exponent (number = mantissa * 2^(exponent-63)
 *         The converted number is written at the given structure.
 * \sa XREAL xr_base10
 */
void xr_base2(XREAL *base10,XREAL *res);
/*!
 * \brief Change internal representation to a base-10 system.
 *
 * This function converts a number stored in base-2 format to the base-10 format.
 *
 * \param base2  Pointer to an XREAL number stored in base-2 format
 * \param res  Pointer to an XREAL to be filled with the result.
 * \return Number with a right justified mantissa with base-10 exponent (number = mantissa * 10^exponent)
 *         The converted number is written at the given structure.
 * \sa XREAL xr_base2
 */

void xr_base10(XREAL *base2,XREAL *res);
/*!
 * \brief Basic operator: division
 *
 * This function calculates numerator / denominator and stores the result.
 *
 * \param num  Pointer to an XREAL number (numerator)
 * \param div  Pointer to an XREAL number (denominator)
 * \param result  Pointer to an XREAL to be filled with the result. Can be the same as one of the operands.
 * \return The result of the operation is written at the given result structure.
 * \sa XREAL xr_add xr_sub xr_mul
 */
void xr_div(XREAL *num,XREAL *div,XREAL *result);
/*!
 * \brief Basic operator: multiplication
 *
 * This function calculates num1 * num2 and stores the result.
 *
 * \param num1  Pointer to an XREAL number
 * \param num2  Pointer to an XREAL number
 * \param result  Pointer to an XREAL to be filled with the result. Can be the same as one of the operands.
 * \return The result of the operation is written at the given result structure.
 * \sa XREAL xr_add xr_sub xr_div
 */

void xr_mul(XREAL *num1,XREAL *num2,XREAL *result);
/*!
 * \brief Basic operator: addition
 *
 * This function calculates num1 + num2 and stores the result.
 *
 * \param num1  Pointer to an XREAL number
 * \param num2  Pointer to an XREAL number
 * \param result  Pointer to an XREAL to be filled with the result. Can be the same as one of the operands.
 * \return The result of the operation is written at the given result structure.
 * \sa XREAL xr_mul xr_sub xr_div
 */

void xr_add(XREAL *num1,XREAL *num2,XREAL *result);
/*!
 * \brief Basic operator: Subtraction
 *
 * This function calculates num1 - num2 and stores the result.
 *
 * \param num1  Pointer to an XREAL number
 * \param num2  Pointer to an XREAL number
 * \param result  Pointer to an XREAL to be filled with the result. Can be the same as one of the operands.
 * \return The result of the operation is written at the given result structure.
 * \sa XREAL xr_add xr_mul xr_div
 */

void xr_sub(XREAL *num1,XREAL *num2,XREAL *result);
/*!
 * \brief Convert a double to an XREAL number
 *
 * This function converts a number stored in 'double' format to the extended real format.
 *
 * \param val  Number to convert
 * \param num  Pointer to an XREAL to be filled with the result.
 * \return The converted number is written at the given structure.
 * \sa xr_int2xr xr_llong2xr xr_uint2xr xr_ullong2xr
 */
void xr_double2xr(double val,XREAL *num);
/*!
 * \brief Convert a 32-bit integer to an XREAL number
 *
 * This function converts a number stored in 'int' format to the extended real format.
 *
 * \param val  Number to convert
 * \param num  Pointer to an XREAL to be filled with the result.
 * \return The converted number is written at the given structure.
 * \sa xr_double2xr xr_llong2xr xr_uint2xr xr_ullong2xr
 */

void xr_int2xr(int val,XREAL *num);
/*!
 * \brief Convert a 64-bit integer to an XREAL number
 *
 * This function converts a number stored in 'int' format to the extended real format.
 *
 * \param val  Number to convert
 * \param num  Pointer to an XREAL to be filled with the result.
 * \return The converted number is written at the given structure.
 * \sa xr_double2xr xr_int2xr xr_uint2xr xr_ullong2xr
 */

void xr_llong2xr(long long val,XREAL *num);
/*!
 * \brief Convert a 32-bit unsigned integer to an XREAL number
 *
 * This function converts a number stored in 'unsigned int' format to the extended real format.
 *
 * \param val  Number to convert
 * \param num  Pointer to an XREAL to be filled with the result.
 * \return The converted number is written at the given structure.
 * \sa xr_double2xr xr_llong2xr xr_int2xr xr_ullong2xr
 */

void xr_uint2xr(unsigned int val,XREAL *num);
/*!
 * \brief Convert a 64-bit integer to an XREAL number
 *
 * This function converts a number stored in 'long long' format to the extended real format.
 *
 * \param val  Number to convert
 * \param num  Pointer to an XREAL to be filled with the result.
 * \return The converted number is written at the given structure.
 * \sa xr_double2xr xr_llong2xr xr_int2xr xr_uint2xr
 */

void xr_ullong2xr(unsigned long long val,XREAL *num);
/*!
 * \brief Convert an XREAL number to 'double' format
 *
 * This function converts an XREAL number to the standard C 'double' format.
 *
 * \param num  Number to convert
 * \return The converted number.
 * \sa xr_xr2llong xr_xr2int xr_xr2uint xr_xr2ullong
 */

double xr_xr2double(XREAL *num);
/*!
 * \brief Convert an XREAL number to 64-bit integer in 'long long' format
 *
 * This function converts an XREAL number to the standard C 'long long' format.
 *
 * \param num  Number to convert
 * \return The converted number.
 * \sa xr_xr2double xr_xr2int xr_xr2uint xr_xr2ullong
 */

long long xr_xr2llong(XREAL *num);
/*!
 * \brief Convert an XREAL number to 32-bit integer
 *
 * This function converts an XREAL number to the standard C 'int' format.
 *
 * \param num  Number to convert
 * \return The converted number.
 * \sa xr_xr2double xr_xr2llong xr_xr2uint xr_xr2ullong
 */

int xr_xr2int(XREAL *num);
/*!
 * \brief Convert an XREAL number to unsigned 64-bit integer in 'unsigned long long' format
 *
 * This function converts an XREAL number to the standard C 'unsigned long long' format.
 *
 * \param num  Number to convert
 * \return The converted number.
 * \sa xr_xr2double xr_xr2int xr_xr2uint xr_xr2llong
 */
unsigned long long xr_xr2ullong(XREAL *num);

/*!
 * \brief Internal use
 */

int xr_getbasedigit(char num,int base);
/*!
 * \brief Internal use
 */

unsigned long long xr_getbaselimit(int base);
/*!
 * \brief Internal use
 */

int xr_getdigits10(XREAL *xr,char *buffer);
/*!
 * \brief Internal use
 */
int xr_getdigits(XREAL *num,char *buffer,int base);
/*!
 * \brief Internal use
 */
int xr_getnumbits(int number);

/*!
 * \brief Constant LN(2)/LN(10) expressed as a 64-bit integer in fixed point, for internal use.
 */
#define LN2_LN10  1292913987LL

#endif



