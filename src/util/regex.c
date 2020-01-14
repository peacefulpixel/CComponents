#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "regex.h"

#ifdef ENDLN
#error ENDLN already defined
#endif

#define ENDLN (char) 0

#ifdef STR_LENGTH
#error STR_LENGTH already defined
#endif

#define STR_LENGTH(__VAR_NAME__, __STR__) \
        unsigned int __VAR_NAME__ = 0; \
        for (; \
            *(__STR__ + __VAR_NAME__); \
            __VAR_NAME__++ \
        ) {}

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
    if (!error) return;

    free(error);
}

void _regex_free_match(_RegMatch *match) {
    if (!match) return;

    free(match);
}

static inline _RegError *regErr(unsigned int index, char *message) {

    _RegError *re = malloc(sizeof(_RegError));
    re->message = message;
    re->index = index;
    return re;

}

/**
 * Concatenates two strings.
 * Parameter "count" is a char count to copy from b
 **/
static inline char *strAppendSome(char *a, char *b, unsigned int count) {
    STR_LENGTH(aLen, a);

    char *newStr = malloc(CH_SIZE * (aLen + count + 1));
    memcpy(newStr, a, aLen);
    memcpy(newStr + aLen, b, count);
    newStr[count + aLen] = ENDLN;

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
 * Extracts positive integer from a string.
 * If an empty string was passed it will return 0
 **/
static inline unsigned int strToInt(char *string) {

    unsigned int result = 0;
    for (unsigned int c = 0; *(string + c); c++) {

        result *= 10;
        result += (unsigned int) *(string + c) - 48;

    }

    return result;

}

/**
 * Checks the availability to extract a positive integer from a string.
 * If an empty string was passed it will return TRUE
 **/
static inline bool isInt(char *string) {

    for (unsigned int c = 0; *(string + c); c++) {

        if (*(string + c) < 48 || *(string + c) > 57)
            return false;

    }

    return true;

}

/**
 * Returns true if string begins with a pattern.
 * If pattern longer than string it will not to produce a segmentation fault
 **/
static inline bool isStartsWith(const char *pattern, char *string) {

    for (unsigned int c = 0; *(pattern + c); c++) {

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

    for (char *c = sequence; *c; c++) {

        if (*c == symbol) 
            return true;

    }

    return false;
}

static inline char *charToString(char character) {

    char *result = malloc(CH_SIZE * 2);
    *(result) = character;
    *(result + 1) = ENDLN;

    return result;

}

enum {
    PF_ANY_VALUE = 1,
    PF_RX_NOT    = 2,
    PF_RX_BEGIN  = 4,
    PF_RX_EMPTY  = 8,
    PF_RX_ENDLN  = 16,
};

typedef struct _pattern {
    char *value;
    byte flags;
    unsigned int from;
    unsigned int to;
    struct _pattern *next;
} Pattern;

static inline Pattern *createPattern() {

    Pattern *pattern = malloc(sizeof(Pattern));
    pattern->value = NULL; // Required only by isMatchPattern
    pattern->next = NULL;
    pattern->flags = 0;

    return pattern;
}

static inline void freePattern(Pattern *pattern) {

    Pattern *cursor = pattern;
    while (cursor) {
        Pattern *next = cursor->next;

        if (cursor->value)
            free(cursor->value);

        free(cursor);

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
    const bool inSeq = isAny || isInSequence(pattern->value, symbol);

    /* This variable is necessary because an isAny will be true even if the
       symbol are '\0'. This is incorrect and will to produce segmentation
       faults by parsing a memory that is beyond of allocated space for string
       what we parse.

       If just to add code to isAny expression like:
           && symbol != '\0'
       Then isInSequence might be invoked with a sequence that NULL, what
       will produce segfault
       */

    const bool isInv = !pattern->value && isAny && !symbol;

    return  (isNot ?
                (!inSeq ||  isBeg):
                ( inSeq && !isBeg))
            && !isInv;

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
        newMatch = malloc(sizeof(_RegMatch));
        newMatch->to = 0;

        if (processPattern(pattern->next, newMatch, input + availableRange)) {

            match->to += (unsigned int) availableRange + newMatch->to;
            free(newMatch);
            return true;

        } else free(newMatch);

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

    while (cursor && !(cursor->flags & PF_RX_EMPTY)) {

        const char current = *(input + inputIndex);

        if (cursor->flags & PF_RX_BEGIN) {

            if (!inputIndex && !isBeginProcessed) {

                cursor = cursor->next;
                isBeginProcessed = true;
                continue;

            } else return false;

        }

        if (cursor->flags & PF_RX_ENDLN) {

            if (current)
                return false;

            cursor = cursor->next;
            continue;

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

    char *allocated = malloc(CH_SIZE * (len + 1));
    memcpy(allocated, string, len);
    allocated[len] = ENDLN;

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

    for (unsigned int c = 0; *(sequence + c); c++) {
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
            if (!(to = *(sequence + c + 2)))
                return regErr(c, "Invalid symbol range");

            char diff = (char) (to - current + 1);
            if (diff < 1)
                return regErr(c, "Invalid symbol range");

            char *seq = malloc(CH_SIZE * (unsigned int) diff + 1);
            for (char c1 = 0; c1 < diff; c1++) {
                *(seq + c1) = (char) (current + c1);
            }

            *(seq + diff) = ENDLN;

            char *appended = strAppend(*buffer, seq);
            free(*buffer);
            free(seq);
            *buffer = appended;

            c += 2;
            
        } else if (current == '\\') {
            char *seq;
            if (!processSpecial(&seq, *(sequence + c + 1)))
                return regErr(c, "Invalid special character");

            char *appended = strAppend(*buffer, seq);
            free(*buffer);
            free(seq);
            *buffer = appended;

            c++;

        } else {
            char *appended = strAppendSome(*buffer, &current, 1);
            free(*buffer);
            *buffer = appended;
        }
    }

    return NULL;
}

static bool processCustomCount(char *pattern, unsigned int *countFrom,
                               unsigned int *countTo, unsigned int *rpLength) {

    /*  If _countTo are been less than _countFrom execution result will be
        FALSE. It means the regex engine will recognize it as just a string.
        */

    char *_countFrom = pattern + 1,
         *_countTo = NULL;

    size_t bracketIndex;

    for (char *p = pattern;; p++) {

        if (!*p) return false;
        if (*p == ',') {
            *p = ENDLN;
            _countTo = p + 1;
        } else if (*p == '}') {
            if (!_countTo)
                return false;

            *p = ENDLN;
            bracketIndex = (size_t) (p - pattern);
            break;
        }

    }

    if (!isInt(_countFrom) || !*_countFrom || !isInt(_countTo))
        return false;

    *countFrom = strToInt(_countFrom);
    *countTo   = !*_countTo ? MAX_AMOUNT : strToInt(_countTo);

    /*  It's bad to write to result buffers and return false but faster
        than create private buffers.
        */

    if (*countTo < *countFrom)
        return false;

    *rpLength  = bracketIndex + 1;

    *(_countTo - 1) = ',';
    *(pattern + bracketIndex) = '}';

    return true;
}

/**
 * Processes a regex count modificators like +, *, etc.
 * Returns true if pattern begin like one of modificators
 **/
static bool processCount(char *pattern, unsigned int *countFrom,
                         unsigned int *countTo, unsigned int *rpLength) {

    if (*pattern == '{')
        return processCustomCount(pattern, countFrom, countTo, rpLength);

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
            *rpLength  = 1;

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

    unsigned int nextIndex = 0;
    if (isStartsWith("[", pattern)) {

        if (*(pattern + 1) == '^') {
            dest->flags |= PF_RX_NOT;
            pattern++;
        }

        unsigned int seqLen = 0;
        while (*(pattern + seqLen + 1) != ']' || *(pattern + seqLen) == '\\') {

            if (!*(pattern + seqLen + 1))
                return regErr(seqLen + 1, "Expected ']' but founded end of line");

            seqLen++;
        }

        /*
            It faster than copy part of string, because expression
            *(pattern + seqLen + 1) always means ']' in this case
        */
        *(pattern + seqLen + 1) = ENDLN;

        char *buffer = malloc(CH_SIZE);
        *buffer = ENDLN;
        _RegError *err = processSequence(&buffer, pattern + 1);
        if (err) {
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

        if (!processSpecial(&seq, *(pattern + 1)))
            return regErr(1, "Unknown special character");

        dest->value = seq;
        nextIndex = 2;

    } else if (isStartsWith("^", pattern)) {

        dest->flags |= PF_RX_BEGIN;
        nextIndex = 1;

    } else if (isStartsWith("$", pattern)) {

        dest->flags |= PF_RX_ENDLN;
        nextIndex = 1;

    } else if (!*pattern) {

        /* This case might be reached only at the first invoking of
           buildPattern if an expression is empty line here. In other calls
           into recursion this case will be never reached because before
           buildPattern are invoking themselves, it will check the
           *(pattern + nextIndex) != ENDLN what makes this code unreachable

           So this code was written only because, in most popular regex
           implementations, expression match("", "") will return one match
           that begins at 0 and ends at 0
           */

        dest->flags |= PF_RX_EMPTY;
        return NULL;

    } else {

        unsigned int count, len;
        if (processCount(pattern, &count, &count, &len))
            return regErr(0, "Declaration of count is unexpected here");

        dest->value = malloc(CH_SIZE * 2);
        *(dest->value) = *pattern;
        *(dest->value + 1) = ENDLN;

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
    if (*(pattern + nextIndex)) {
        dest->next = createPattern();
        
        return buildPattern(pattern + nextIndex, dest->next);
    }

    return NULL;
}

bool _regex_match(_RegError **error, _RegMatch **match, char *pattern, char *str) {

    Pattern *_pattern = createPattern();

    char *patternCopy = toHeap(pattern);
    *error = buildPattern(patternCopy, _pattern);
    free(patternCopy);

    if (*error) {
        return false;
    }

    *match = malloc(sizeof(_RegMatch));
    INIT_REG_MATCH((*match));

    char *strCopy = toHeap(str);
    bool result = false;
    _RegMatch *cursor = *match;

    isBeginProcessed = false;

    for (unsigned int c = 0; c < 1 || *(strCopy + c - 1); c++) {
        if (processPattern(_pattern, cursor, strCopy + c)) {

            result = true;

            unsigned int newC = c + cursor->to - !(cursor->to == cursor->from);
            cursor->next = malloc(sizeof(_RegMatch));
            cursor->from += c;
            cursor->to += c;

            c = newC;

            cursor = cursor->next;
            INIT_REG_MATCH(cursor);

        }
    }

    free(cursor->next);
    free(strCopy);

    if (!result) {
        free(*match);
        *match = NULL;
    }

    freePattern(_pattern);

    return result;
}
