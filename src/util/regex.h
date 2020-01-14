#ifndef __REGEX_H__
#define __REGEX_H__

#include <stdbool.h>

typedef struct __reg_error {
    char *message;
    unsigned int index;
} _RegError;

void _regex_free_error(_RegError *error);

typedef struct __reg_match {
    unsigned int from;
    unsigned int to;
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

#endif /* __REGEX_H__ */
