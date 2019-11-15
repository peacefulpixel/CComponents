/**
 * The regular expressions are required by CComponents library for some
 * components methods implementation (String.match as an example). The reason
 * for creating custom regex realization is library independence and
 * cross-platform building and usage.
 * 
 * Native regex implementation for C supports only in UNIX systems
 * like OS X or Linux. The header <regex.h> does not exist on the Windows
 * platform and MinGW doesn't provide it.
 * 
 * PCRE2 usage will cause an additional project dependency that must be
 * installed by library users for build or usage.
 **/

#ifndef __REGEX_H__
#define __REGEX_H__

#include <stdbool.h>

typedef struct __reg_error {
    char *message;
    int index;
} _RegError;

void _regex_free_error(_RegError *error);

typedef struct __reg_match {
    int from;
    int to;
    struct __reg_match *next;
} _RegMatch;

void _regex_free_match(_RegMatch *match);

/** 
 * Returns 0 if patern is invalid or *error and/or *match is not null
 */
bool _regex_match(  _RegError **error,
                    _RegMatch **match,
                    char *pattern,
                    char *str);

#endif
