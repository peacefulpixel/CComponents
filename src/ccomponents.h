#ifndef __C_COMPONENTS_H__
#define __C_COMPONENTS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>

#ifdef delete
#error Macro delete already defined
#endif /* delete */
#define delete(object) (object->_class->delete(object))

typedef enum _ccomp_classtype {
    INTERFACE_CCOBJECT,
    INTERFACE_LIST,
    INTERFACE_MAP,
    CLASS_ARRAY_LIST,
    CLASS_ARRAY_MAP,
    CLASS_STRING
} ClassType;

typedef struct _ccomp_class {
    ClassType classType;
    void (*delete)(void *instance);
} Class;

typedef void * v_private;

/**
 * Interfaces pre-declaration
 */

typedef struct _ccomp_object CCObject; // For case library user will reserves name Object
typedef struct _ccomp_list List;
typedef struct _ccomp_map Map;

/**
 * Classes pre-declaration
 */

typedef struct _ccomp_array_list_class ClassArrayListType;
typedef struct _ccomp_array_list ArrayList;
typedef struct _ccomp_array_map_class ClassArrayMapType;
typedef struct _ccomp_array_map ArrayMap;
typedef struct _ccomp_string_class ClassStringType;
typedef struct _ccomp_string String;

/**
 * CCObject
 * 
 * 
 */

struct _ccomp_object {
    /**
     * Type of interface
     */
    ClassType interfaceType;
    /**
     * Will return serialized Object as String
     */
    String *(*toString)(void *this);
    /*
     * Returns a copy of current Object
     */
    void *(*copy)(void *this);
};

/**
 * List
 * 
 * 
 */

struct _ccomp_list {
    ClassType interfaceType;
    /*
     * Adds an item to the list.
     */
    void (*push)(void *this, void *value);
    /*
     * Removes an item from the list with offset
     */
    void (*remove)(void *this, unsigned long int index);
    /*
     * Replaces list item by index
     */
    void (*set)(void *this, unsigned long int index, void *value);
    /*
     * Returns a list item by index
     */
    void *(*get)(void *this, unsigned long int index);
    /*
     * Returns the length of the array
     */
    unsigned long int (*length)(void *this);

    CCObject _impl_CCObject;
};

/**
 * Map
 * 
 * 
 */

struct _ccomp_map {
    ClassType interfaceType;
    /*
     * Removes an item from the map
     */
    void (*remove)(void *this, char *key);
    /*
     * Replaces map item
     */
    void (*set)(void *this, char *key, void *value);
    /*
     * Returns a map item
     */
    void *(*get)(void *this, char *_value);
    /*
     * Returns the length of the map
     */
    unsigned long int (*length)(void *this);

    CCObject _impl_CCObject;
};

/*
 *  ArrayList
 * 
 *  
 */ 

extern Class classArrayList;
extern ClassArrayListType ClassArrayList;

struct _ccomp_array_list_class {
    /*
     * Adds elements of array to list
     */
    void (*include)(void *this, void **, unsigned long int);

    List _impl_List;
};

struct _ccomp_array_list {
    Class *_class;
    ClassArrayListType *class;
    v_private _private;
};

extern ArrayList *createArrayList();

#ifdef CreateArrayList
#error Macro CreateArrayList already defined
#endif /* CreateArrayList */
#define CreateArrayList createArrayList

/*
 *  ArrayMap
 * 
 *  
 */ 

extern Class classArrayMap;
extern ClassArrayMapType ClassArrayMap;

struct _ccomp_array_map_class {
    Map _impl_Map;
};

struct _ccomp_array_map {
    Class *_class;
    ClassArrayMapType *class;
    v_private _private;
};

extern ArrayMap *createArrayMap();

#ifdef CreateArrayMap
#error Macro CreateArrayMap already defined
#endif /* CreateArrayMap */
#define CreateArrayMap createArrayMap

/*
 *  String
 * 
 *  
 */ 

/*
 * The structure that stores the match (for the match method of String).
 * Released in all cases through free();
 */
typedef struct _sys_string_match {
    int begin;
    int end;
} StringMatch;

extern Class classString;
extern ClassStringType ClassString;

struct _ccomp_string_class {
    /*
     * Returns the String value.
     */
    char *(*getValue)(void *this);
    /*
     * Sets the value to String
     */
    void (*setValue)(void *this, char *);
    /*
     * Concatenates the value of the current String and the second argument of the method.
     * The result is written to the current String
     *   Example:
     *     String *s = newString("Hello ");
     *     s->add(s, "world!");
     *     printf("%s\n", s->getValue(s)); // output: "Hello worlrd!"
     */
    void (*add)(void *this, char *);
    /*
     * Returns a substring of the current String between two indices, inclusive
     *   Example:
     *     String *s = newString("Hello world!");
     *     String *left  = s->sub(s, 3, 4);
     *     String *right = s->sub(s, 3, 3);
     *     printf("%s%s\n", left->getValue(left), right->getValue(right)); // output: "lol"
     */
    String *(*sub)(void *this, int, int);
    /* 
     * Replaces in the value of the current String all matches on the regex-pattern with the value
     * passed by the last argument
     *   Example:
     *     my beautiful girlfriendString *s = newString("Hello world!");
     *     s->replace(s, "l", "he");
     *     printf("%s\n", s->getValue(s)); // output: "Heheheo worhed!" 
     */
    void (*replace)(void *this, char *, char *);
    /* 
     * Replaces the value of the current String with the first match of the regex pattern with the value
     * passed by the last argument
     *   Example:
     *     String *s = newString("Hello world!");
     *     s->replaceFirst(s, "l", "he");
     *     printf("%s\n", s->getValue(s)); // output: "Hehelo world!" 
     */
    void (*replaceFirst)(void *this, char *, char *);
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
    StringMatch *(*match)(void *this, char *, int);
    /*
     * Returns an sys/list of String *
     */
    ArrayList *(*split)(void *this, char *);
    /*
     * Adds a long integer to a string.
     */
    void (*addLong)(void *this, long int);
    /*
     * Adds a unsigned long integer to a string.
     */
    void (*addULong)(void *this, unsigned long int);
    /*
     * Attempts to convert a string to a number.
     * On unsuccessful attempt returns 0
     */
    int (*toInt)(void *this);
    /*
     * Returns a character from a string by index
     */
    char (*charAt)(void *this, int);
    /*
     * Returns the length of a string.
     */
    int (*length)(void *this);
    /*
     * Returns the result of comparing two Strings.
     */
    bool (*equals)(void *this, String *);
    /*
     * Returns the result of comparing a String and a pointer to a character.
     */
    bool (*equalsChr)(void *this, char *);
    
    CCObject _impl_CCObject;
};

struct _ccomp_string {
    Class *_class;
    ClassStringType *class;
    v_private _private;
};

extern String *createStringChar(char *);
extern String *createStringLong(long int);
extern String *createStringULong(unsigned long int);

#ifdef CreateString
#error Macro CreateString already defined
#endif /* CreateString */
#define CreateString(X)      \
    _Generic((X),         \
    char *   : createStringChar,\
    int      : createStringLong,\
    long int : createStringLong,\
    unsigned long int : createStringULong,\
    unsigned int      : createStringULong\
    ) (X)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __C_COMPONENTS_H__ */