/*-------------------------------------------------------------------------*/
/**
   @file    iniparser.h
   @author  N. Devillard
   @brief   Parser for ini files.
*/
/*--------------------------------------------------------------------------*/

#ifndef _INIPARSER_H_
#define _INIPARSER_H_

/*---------------------------------------------------------------------------
                                Includes
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * The following #include is necessary on many Unixes but not Linux.
 * It is not needed for Windows platforms.
 * Uncomment it if needed.
 */
/* #include <unistd.h> */

#include "dictionary.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*/
/**
  @brief    Configure a function to receive the error messages.
  @param    errback  Function to call.

  By default, the error will be printed on stderr. If a null pointer is passed
  as errback the error callback will be switched back to default.
 */
/*--------------------------------------------------------------------------*/

void iniparser_set_error_callback(int (*errback)(const char *, ...));



int iniparser_getnsec(const dictionary * d);




const char * iniparser_getsecname(const dictionary * d, int n);




void iniparser_dump_ini(const dictionary * d, FILE * f);


void iniparser_dumpsection_ini(const dictionary * d, const char * s, FILE * f);


void iniparser_dump(const dictionary * d, FILE * f);

int iniparser_getsecnkeys(const dictionary * d, const char * s);


const char ** iniparser_getseckeys(const dictionary * d, const char * s, const char ** keys);


const char * iniparser_getstring(const dictionary * d, const char * key, const char * def);

int iniparser_getint(const dictionary * d, const char * key, int notfound);

long int iniparser_getlongint(const dictionary * d, const char * key, long int notfound);

double iniparser_getdouble(const dictionary * d, const char * key, double notfound);

int iniparser_getboolean(const dictionary * d, const char * key, int notfound);


int iniparser_set(dictionary * ini, const char * entry, const char * val);


void iniparser_unset(dictionary * ini, const char * entry);



int iniparser_find_entry(const dictionary * ini, const char * entry) ;


dictionary * iniparser_load(const char * ininame);

void iniparser_freedict(dictionary * d);

#ifdef __cplusplus
}
#endif

#endif