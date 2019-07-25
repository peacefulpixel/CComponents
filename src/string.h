#ifndef __STRING_H__
#define __STRING_H__

#define newString(X)      \
    _Generic((X),         \
    char *: newStringChar,\
    int   : newStringInt  \
    ) (X)

#include "list.h"

/*
 * The structure that stores the match (for the match method of String).
 * Released in all cases through free();
 */
typedef struct _sys_string_match {
    int begin;
    int end;
} StringMatch;

typedef struct _sys_string {
    /* 
     * Private value, do not even try!)
     */
    void *_value;
    /*
     * Returns the String value.
     */
    char *(*getValue)(struct _sys_string *this);
    /*
     * Sets the value to String
     */
    void (*setValue)(struct _sys_string *this, char *);
    /*
     * Concatenates the value of the current String and the second argument of the method.
     * The result is written to the current String
     *   Example:
     *     String *s = newString("Hello ");
     *     s->add(s, "world!");
     *     printf("%s\n", s->getValue(s)); // output: "Hello worlrd!"
     */
    void (*add)(struct _sys_string *this, char *);
    /*
     * Returns a substring of the current String between two indices, inclusive
     *   Example:
     *     String *s = newString("Hello world!");
     *     String *left  = s->sub(s, 3, 4);
     *     String *right = s->sub(s, 3, 3);
     *     printf("%s%s\n", left->getValue(left), right->getValue(right)); // output: "lol"
     */
    struct _sys_string *(*sub)(struct _sys_string *this, int, int);
    /* 
     * Replaces in the value of the current String all matches on the regex-pattern with the value
     * passed by the last argument
     *   Example:
     *     String *s = newString("Hello world!");
     *     s->replace(s, "l", "he");
     *     printf("%s\n", s->getValue(s)); // output: "Heheheo worhed!" 
     */
    void (*replace)(struct _sys_string *this, char *, char *);
    /* 
     * Replaces the value of the current String with the first match of the regex pattern with the value
     * passed by the last argument
     *   Example:
     *     String *s = newString("Hello world!");
     *     s->replaceFirst(s, "l", "he");
     *     printf("%s\n", s->getValue(s)); // output: "Hehelo world!" 
     */
    void (*replaceFirst)(struct _sys_string *this, char *, char *);
    /*
     * Returns an array of n StringMatch elements.
     * The second argument is the char * that contains the regular expression.
     *   Where:
     *     n - last argument of method, maximum number of matches
     *   StringMatch:
     *     The begin property is the start of a match.
     *     The end property is the end of a match.
     *   Example:
     *     String *s = newString("Hello world!");
     *     String *regex = newString("l");
     *     StringMatch *matches = s->match(s, regex->getValue(regex), 3);
     *     printf("%d:%d, %d:%d\n", matches[0].begin, matches[0].end, 
     *                              matches[1].begin, matches[1].end); // output: "2:3, 3:4" 
     *     deleteString(regex);
     *     deleteString(s);
     */
    StringMatch *(*match)(struct _sys_string *this, char *, int);
    /*
     * Returns an sys/list of String *
     */
    List *(*split)(struct _sys_string *this, char *);
    /*
     * Adds a integer to a string.
     */
    void (*addInt)(struct _sys_string *this, int);
    /*
     * Attempts to convert a string to a number.
     * On unsuccessful attempt returns 0
     */
    int (*toInt)(struct _sys_string *this);
    /*
     * Returns a character from a string by index
     */
    char (*charAt)(struct _sys_string *this, int);
    /*
     * Returns the length of a string.
     */
    int (*length)(struct _sys_string *this);
    /*
     * Returns the result of comparing two Strings.
     */
    int (*equals)(struct _sys_string *this, struct _sys_string *);
    /*
     * Returns the result of comparing a String and a pointer to a character.
     */
    int (*equalsChr)(struct _sys_string *this, char *);
} String;

String *newStringChar(char *);
String *newStringInt (int   );
void    deleteString(String *);

#endif /* __STRING_H__ */