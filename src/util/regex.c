#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "regex.h"

/* Debug area */

// #define __REGEX_DEBUG__

#ifdef ALOC
#error ALOC already defined
#endif

#ifdef FREE
#error FREE already defined
#endif

#ifdef PRINT_ALLOCATED_COUNT
#error PRINT_ALLOCATED_COUNT already defined
#endif

#ifdef __REGEX_DEBUG__
#include <stdio.h>
static long __regex_allocated_count = 0;
#define ALOC(__SIZE__) malloc(__SIZE__); __regex_allocated_count++;
#define FREE(__POINTER__) free(__POINTER__); __regex_allocated_count--;
#define PRINT_ALLOCATED_COUNT \
printf("[RX_DEBUG] __regex_allocated_count=%d\n", __regex_allocated_count); \
__regex_allocated_count = 0;
#else
#define ALOC(__SIZE__) malloc(__SIZE__);
#define FREE(__POINTER__) free(__POINTER__);
#define PRINT_ALLOCATED_COUNT
#endif

/* End of debug area */

#ifdef STR_LENGTH
#error STR_LENGTH already defined
#endif

#define STR_LENGTH(__VAR_NAME__, __STR__) \
        unsigned int __VAR_NAME__ = 0; \
        for (; \
            *(__STR__ + __VAR_NAME__) != '\0'; \
            __VAR_NAME__++ \
        ) {}

#ifdef THROW_REG_ERR
#error THROW_REG_ERR already defined
#endif

#define THROW_REG_ERR(__INDEX__, __MSG__) \
    _RegError *__RegError = ALOC(sizeof(_RegError)); \
    __RegError->message = __MSG__; \
    __RegError->index = __INDEX__; \
    return __RegError;

#ifdef CH_SIZE
#error CH_SIZE already defined
#endif

#define CH_SIZE sizeof(char)

#ifdef INIT_REG_MATCH
#error INIT_REG_MATCH already defined
#endif

#define INIT_REG_MATCH(__REG_MATCH__) \
    __REG_MATCH__->from = 0; \
    __REG_MATCH__->to   = 0; \
    __REG_MATCH__->next = NULL;

static const unsigned int MAX_AMOUNT = 65535;

typedef char byte;

void _regex_free_error(_RegError *error) {
    if (error == NULL) return;

    FREE(error);
}

void _regex_free_match(_RegMatch *match) {
    if (match == NULL) return;

    FREE(match);
}

/**
 * Concatenates two strings.
 * Parameter "count" is a char count to copy from b
 **/
static inline char *strAppendSome(char *a, char *b, unsigned int count) {
    STR_LENGTH(aLen, a);

    char *newStr = ALOC(CH_SIZE * (aLen + count + 1));
    memcpy(newStr, a, aLen);
    memcpy(newStr + aLen, b, count);
    newStr[count + aLen] = '\0';

    return newStr;
}

/**
 * Concatenates two strings
 **/
static inline char *strAppend(char *a, char *b) {
    STR_LENGTH(bLen, b);

    return(strAppendSome(a, b, bLen));
}

/**
 * Returns true if string begins with a pattern.
 * If pattern longer than string it will not to produce a segmentation fault
 **/
static inline bool isStartsWith(const char *pattern, char *string) {

    for (unsigned int c = 0; *(pattern + c) != '\0'; c++) {

        if (*(string + c) != *(pattern + c))
            return false;

    }

    return true;
}

/**
 * Returns true if sequence contains a symbol.
 * The sequence must be a C-string what means the last char of 
 * sequence will be '\0'
 **/
static inline bool isInSequence(char *sequence, char symbol) {

    for (char *c = sequence; *c != '\0'; c++) {

        if (*c == symbol) 
            return true;

    }

    return false;
}

static inline char *charToString(char character) {

    char *result = ALOC(CH_SIZE * 2);
    *(result) = character;
    *(result + 1) = '\0';

    return result;

}

enum {
    PF_ANY_VALUE = 1,
    PF_RX_NOT    = 2,
    PF_RX_BEGIN  = 4,
};

typedef struct _pattern {
    char *value;
    byte flags;
    unsigned int from;
    unsigned int to;
    struct _pattern *next;
} Pattern;

static inline Pattern *createPattern() {

    Pattern *pattern = ALOC(sizeof(Pattern));
    pattern->next = NULL;
    pattern->flags = 0;

    return pattern;
}

static inline void freePattern(Pattern *pattern) {

    Pattern *cursor = pattern;
    while (cursor != NULL) {
        Pattern *next = cursor->next;

        if (cursor->value != NULL)
            FREE(cursor->value);

        FREE(cursor);

        cursor = next;
    }

}

/**
 * Indicates the first character of input was processed or not
 **/
static bool isBeginProcessed;

/**
 * Checks a symbol matches the value of pattern
 **/
static inline bool isMatchPattern(Pattern *pattern, char symbol) {

    const bool isAny = pattern->flags & PF_ANY_VALUE;
    const bool isNot = pattern->flags & PF_RX_NOT;
    const bool isBeg = pattern->flags & PF_RX_BEGIN;
    const bool inSeq = isInSequence(pattern->value, symbol);

    return isNot ?
        ((!isAny && !inSeq) ||  isBeg):
        (( isAny ||  inSeq) && !isBeg);

}

/**
 * Processes a part of the pattern that contains a flexible amount of symbols.
 * This function invokes processPattern() while it processes an amount and
 * returns a final result of iteration
 **/
static bool processFlexibleCount(   Pattern *pattern,
                                    _RegMatch *match,
                                    char *srcInput);
/**
 * Processes the pattern and store matches.
 * Returns false if the input does not match a pattern
 **/
static bool processPattern(  Pattern *pattern,
                            _RegMatch *match,
                            char *srcInput);

static bool processFlexibleCount(   Pattern *pattern,
                                    _RegMatch *match,
                                    char *input) {

    const unsigned int range = pattern->to - pattern->from;

    int availableRange = 0;
    for (; availableRange < range; availableRange++) {

        if (!isMatchPattern(pattern, *(input + availableRange)))
            break;

    }

    do {

        _RegMatch *newMatch;

    process:
        newMatch = ALOC(sizeof(_RegMatch));
        newMatch->to = 0;

        if (processPattern(pattern->next, newMatch, input + availableRange)) {

            match->to += (unsigned int) availableRange + newMatch->to;
            FREE(newMatch);
            return true;

        } else FREE(newMatch);

        availableRange--;

    } while (availableRange > 0);

    if (availableRange == 0) goto process;

    return false;
}

static bool processPattern( Pattern *pattern,
                            _RegMatch *match,
                            char *input) {
    
    Pattern *cursor = pattern;
    unsigned int inputIndex = 0;
    unsigned int patternCountPassed = 0;

    while (cursor != NULL) {

        const char current = *(input + inputIndex);

        if (cursor->flags & PF_RX_BEGIN) {

            if (!inputIndex && !isBeginProcessed) {

                cursor = cursor->next;
                isBeginProcessed = true;
                continue;

            } else return false;

        }

        bool isMatch = isMatchPattern(cursor, current);

        isBeginProcessed = true;

        if (cursor->from > patternCountPassed) {

            if (!isMatch) {
                return false;
            }

            patternCountPassed++;
            inputIndex++;

        } else if (cursor->from < cursor->to) {

            if (processFlexibleCount(cursor, match, input + inputIndex))
                break;

            return false;

        } else {

            cursor = cursor->next;
            patternCountPassed = 0;

        }
    }

    match->to  += inputIndex;
    match->next = NULL;

    return true;
}

/**
 * Copies bytes from string to allocated space.
 * Returns pointer to allocated space
 **/
static inline char *toHeap(const char *string) {
    STR_LENGTH(len, string);

    char *allocated = ALOC(CH_SIZE * (len + 1));
    memcpy(allocated, string, len);
    allocated[len] = '\0';

    return allocated;
}

/**
 * Processes a regex special rules what starts with backslash like \s, \d, etc.
 * Puts to a buffer a char sequence that contains what special rule means
 **/
static bool processSpecial(char **buffer, char character) {

    const char *s = "\r\n\t\f\v ";
    const char *w = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    const char *d = "0123456789";

    switch (character) {
        
        // TODO: Catch more cases
        case 's': *buffer = toHeap(s); return true;
        case 'w': *buffer = toHeap(w); return true;
        case 'd': *buffer = toHeap(d); return true;

        case '\\': *buffer = toHeap("\\"); return true; // TODO: Replace with cycle

    }

    char *specials = ".\\[]-()+*$^{}?";

    if (isInSequence(specials, character)) {
        *buffer = charToString(character);
        return true;
    }

    return false;
}

/**
 * Processes a regex sequence what places between quad brackets
 * Puts to a buffer a char sequence that contains all available symbols 
 * from passed regex pattern
 **/
static _RegError *processSequence(char **buffer, char *sequence) {

    for (unsigned int c = 0; *(sequence + c) != '\0'; c++) {
        char current = *(sequence + c);
        
        /*
            Expression *(pattern + c + 1) will not to produce segmentation falut
            because if (pattern + c) points to last char of string, next char
            will be '\0'. If string does not ends with '\0' means the string is invalid.
        */
        if (*(sequence + c + 1) == '-') {
            char to;

            /*
                Safe too, because *(pattern + c + 1) is not '\0' here what means
                the length of string equals c + 3 as minimum
            */
            if ((to = *(sequence + c + 2)) == '\0') {
                THROW_REG_ERR(c, "Invalid symbol range");
            }

            char diff = (char) (to - current + 1);
            if (diff < 1) {
                THROW_REG_ERR(c, "Invalid symbol range");
            }

            char *seq = ALOC(CH_SIZE * (unsigned int) diff + 1);
            for (char c1 = 0; c1 < diff; c1++) {
                *(seq + c1) = (char) (current + c1);
            }

            *(seq + diff) = '\0';

            char *appended = strAppend(*buffer, seq);
            FREE(*buffer);
            FREE(seq);
            *buffer = appended;

            c += 2;
            
        } else if (current == '\\') {
            char *seq;
            if (!processSpecial(&seq, *(sequence + c + 1))) {
                THROW_REG_ERR(c, "Invalid special character");
            }

            char *appended = strAppend(*buffer, seq);
            FREE(*buffer);
            FREE(seq);
            *buffer = appended;

            c++;

        } else {
            char *appended = strAppendSome(*buffer, &current, 1);
            FREE(*buffer);
            *buffer = appended;
        }
    }

    return NULL;
}

/**
 * Processes a regex count modificators like +, *, etc.
 * Returns true if pattern begin like one of modificators
 **/
static bool processCount(char *pattern, unsigned int *countFrom,
                         unsigned int *countTo, unsigned int *rpLength) {

    const unsigned int supportedCount = 3; /* Amount of supported modificators */

    const struct { 
        const char *ptrn; unsigned int from; unsigned int to;
    } countPatterns[] = {
        { "*", 0, MAX_AMOUNT },
        { "+", 1, MAX_AMOUNT },
        { "?", 0, 1 }
    };
    
    for (unsigned int c = 0; c < supportedCount; c++) {

        if (isStartsWith(countPatterns[c].ptrn, pattern)) {
            *countFrom = countPatterns[c].from;
            *countTo   = countPatterns[c].to;
            *rpLength  = 1; // it will be required by the {0, ...} feature

            return true;
        }
    }

    return false;
}

/**
 * Compiles a Pattern from regular expression string
 * If compiling was successfully passed it returns NULL
 **/
static _RegError *buildPattern(char *pattern, Pattern *dest) {

    /* Parsing expression */
    unsigned int nextIndex = 0;
    if (isStartsWith("[", pattern)) {

        if (*(pattern + 1) == '^') {
            dest->flags |= PF_RX_NOT;
            pattern++;
        }

        unsigned int seqLen = 0;
        while (*(pattern + seqLen + 1) != ']' || *(pattern + seqLen) == '\\') {

            if (*(pattern + seqLen + 1) == '\0') { 
                THROW_REG_ERR(seqLen + 1, "Expected ']' but founded end of line");
            }

            seqLen++;
        }

        /*
            It faster than copy part of string, because expression
            *(pattern + seqLen + 1) always means ']' in this case
        */
        *(pattern + seqLen + 1) = '\0';

        char *buffer = ALOC(CH_SIZE);
        *buffer = '\0';
        _RegError *err = processSequence(&buffer, pattern + 1);
        if (err != NULL) {
            err->index++;
            return err;
        }

        *(pattern + seqLen + 1) = ']';

        dest->value = buffer;
        nextIndex = seqLen + 2; /* +2 for brackets */

    } else if (isStartsWith(".", pattern)) {

        dest->flags |= PF_ANY_VALUE;
        nextIndex = 1;

    } else if (isStartsWith("\\", pattern)) {
        char *seq;

        if (!processSpecial(&seq, *(pattern + 1))) {
            THROW_REG_ERR(1, "Unknown special character");
        }

        dest->value = seq;
        nextIndex = 2;

    } else if (isStartsWith("^", pattern)) {

        dest->flags |= PF_RX_BEGIN;
        nextIndex = 1;

    } else {

        unsigned int count, len;
        if (processCount(pattern, &count, &count, &len)) {
            THROW_REG_ERR(0, "Declaration of count is unexpected here")
        }

        dest->value = ALOC(CH_SIZE * 2);
        *(dest->value) = *pattern;
        *(dest->value + 1) = '\0';

        nextIndex = 1;
    }

    /* Parsing expression matching count */
    unsigned int countFrom, countTo, len = 0;
    if (!processCount(pattern + nextIndex, &countFrom, &countTo, &len)) {
        dest->from = dest->to = 1;
    } else {
        dest->from = countFrom;
        dest->to = countTo;
        nextIndex += len;
    }

    /* Parsing next expression if needed */
    if (*(pattern + nextIndex) != '\0') {
        dest->next = createPattern();
        
        return buildPattern(pattern + nextIndex, dest->next);
    }

    return NULL;
}

bool _regex_match(_RegError **error, _RegMatch **match, char *pattern, char *str) {

    Pattern *_pattern = createPattern();

    char *patternCopy = toHeap(pattern);
    *error = buildPattern(patternCopy, _pattern);
    FREE(patternCopy);

    if (*error != NULL) {
        return false;
    }

    *match = ALOC(sizeof(_RegMatch));
    INIT_REG_MATCH((*match));

    char *strCopy = toHeap(str);
    bool result = false;
    _RegMatch *cursor = *match;

    isBeginProcessed = false;

    for (unsigned int c = 0; c < 1 || (*(strCopy + c - 1) != '\0'); c++) {
        if (processPattern(_pattern, cursor, strCopy + c)) {

            result = true;

            unsigned int newC = c + cursor->to - 1;
            cursor->next = ALOC(sizeof(_RegMatch));
            cursor->from += c;
            cursor->to += c;

            c = newC;

            cursor = cursor->next;
            INIT_REG_MATCH(cursor);

        }
    }

    FREE(cursor->next);
    FREE(strCopy);

    if (!result) {
        FREE(*match);
        *match = NULL;
    }

    freePattern(_pattern);

    PRINT_ALLOCATED_COUNT

    return result;
}
