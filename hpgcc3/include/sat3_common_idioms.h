//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


#ifndef __SAT3_COMMON_IDIOMS_H
#define __SAT3_COMMON_IDIOMS_H

//  $Id: sat3_common_idioms.h 542 2008-01-28 01:43:40Z ingo $

#include <hpgcc3.h>

int sat3_pop_int(int def);
BOOL sat3_push_int_zint(int n);
BOOL sat3_push_int_real(int n);
BOOL sat3_push_int_bint(int n);
BOOL sat3_push_llong_zint(LONGLONG n);
BOOL sat3_push_decn_zint(decNumber *dcn);
decNumber *sat3_pop_decn(char *def);
double sat3_pop_dbl(double def);
BOOL sat3_push_dbl_real(double x);
char *sat3_pop_string_alloc();
char *sat3_pop_string(char *s, int strl);
BOOL sat3_push_string(const char *s);

#endif // __SAT3_COMMON_IDIOMS_H


