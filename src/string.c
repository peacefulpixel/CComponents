#include <math.h>

#ifndef _WIN32
#include <regex.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccomponents.h"

#define P_SIZE sizeof(intptr_t)
#define this ((String *) _this)

typedef struct _string_private {
    char *stringValue;
} Private;

static char *__string_get(void *_this) {
    Private *private = (Private *) this->_private;
    return private->stringValue;
}

static void __string_set(void *_this, char *value) {
    int memSize = (int) (sizeof(char) * strlen(value));

    free(((Private *) this->_private)->stringValue);

    Private *stringValue = (Private *) this->_private;
    stringValue->stringValue = (char *) malloc((size_t) memSize + 1);

    memcpy(stringValue->stringValue, value, (size_t) memSize);
    memcpy(stringValue->stringValue + memSize, "\0", 1);
}

static void __string_add(void *_this, char *value) {
    int aSize = (int) (sizeof(char) * strlen(this->class->getValue(this)));
    int bSize = (int) (sizeof(char) * strlen(value));

    char *aDump = (char *) malloc((size_t) aSize);
    memcpy(aDump, this->class->getValue(this), (size_t) aSize);

    Private *private = (Private *) this->_private;
    private->stringValue = (char *) malloc((size_t) (aSize + bSize) + 1);
 
    memcpy(private->stringValue, aDump, (size_t) aSize);
    memcpy(private->stringValue + aSize, value, (size_t) bSize);
    memcpy(private->stringValue + aSize + bSize, "\0", 1);

    free(aDump);
}

static String *__string_sub(void *_this, int begin, int end) {
    int size = (int) sizeof(char) * (end - begin);

    char *newValue = (char *) malloc((size_t) size + 1);
    memcpy(newValue, this->class->getValue(this) + begin, (size_t) size);
    memcpy(newValue + size, "\0", 1);

    String *newString = createStringChar(newValue);
    free(newValue);

    return newString;
}

#ifndef _WIN32

static void __string_replace(void *_this, char *regex, char *value) {
    String *current = createStringChar(this->class->getValue(this));
    StringMatch *match = current->class->match(current, regex, 1);

    String *builder = createStringChar("");

    int finded = 0;
    while (match->begin != -1 && match->begin != -2) {
        String *start = current->class->sub(current, 0, match->begin);
        builder->class->add(builder, start->class->getValue(start));
        builder->class->add(builder, value);

        delete(start);

        String *newCurrent = current->class->sub(current, match->end, current->class->length(current));
        delete(current);
        current = newCurrent;
        finded = 1;
        free(match);
        match = current->class->match(current, regex, 1);
    }

    if (finded)
        builder->class->add(builder, current->class->getValue(current));

    free(match);
    if (!builder->class->equalsChr(builder, ""))
        this->class->setValue(this, builder->class->getValue(builder));

    delete(current);
    delete(builder);
}

static void __string_replaceFirst(void *_this, char *regex, char *value) {
    StringMatch *match = this->class->match(this, regex, 1);

    if (match->begin == -1 || match->begin == -2)
        return;

    char *oldValue = this->class->getValue(this);
    int size = (int) (strlen(oldValue) - strlen(regex) + strlen(value));
    char *newValue = (char *) malloc(sizeof(char) * ((size_t) size + 1));

    memcpy(newValue, oldValue, (size_t) match->begin);
    memcpy(newValue + match->begin, value, strlen(value));
    memcpy(newValue + match->begin + strlen(value), oldValue + match->end, strlen(oldValue + match->end));
    memcpy(newValue + size, "\0", 1);

    this->class->setValue(this, newValue);

    free(match);
    free(newValue);
}

static StringMatch *__string_match(void *_this, char *regex, int maxMatchesCount) {
    regex_t regexObject;

    StringMatch *matchesResult = (StringMatch *) malloc((size_t) maxMatchesCount * sizeof(matchesResult));

    int compileResult = regcomp(&regexObject, regex, REG_EXTENDED);
    if (compileResult != 0) {
        for (int x = 0; x < maxMatchesCount; x++) {
            matchesResult[x].begin = -2;
            matchesResult[x].end   = -2;
        }
    } else  {
        regmatch_t matches[1];
        
        int cuttedLength = 0;
        String *copy = CreateString(this->class->getValue(this));
        for (int x = 0; x < maxMatchesCount; x++) {
            int result = regexec(&regexObject, copy->class->getValue(copy), 1, matches, REG_NOERROR);

            matchesResult[x].begin = result ? -1 : cuttedLength + matches[0].rm_so;
            matchesResult[x].end   = result ? -1 : cuttedLength + matches[0].rm_eo;

            if (!result) {
                String *newCopy = copy->class->sub(copy, matches[0].rm_eo, copy->class->length(copy));

                cuttedLength += matches[0].rm_eo;

                delete(copy);
                copy = newCopy;
            }
        }
    }

    regfree(&regexObject);
    return matchesResult;
}

static ArrayList *__string_split(void *_this, char *regex) {
    StringMatch *match = this->class->match(this, regex, 1);
    ArrayList *result = CreateArrayList((int) sizeof(String *));

    String *copy = this->class->sub(this, 0, this->class->length(this));
    while (match->begin != -1) {
        String *sub = copy->class->sub(copy, 0, match->begin);
        String *newCopy = copy->class->sub(copy, match->end, copy->class->length(copy));
        delete(copy);
        copy = newCopy;

        result->class->_impl_List.push(result, sub);
        match = copy->class->match(copy, regex, 1);
    } result->class->_impl_List.push(result, CreateString(copy->class->getValue(copy)));

    delete(copy);
    free(match);
    return result;
}

#endif

static void __string_addLong(void *_this, long int number) {
    String *numberTemp = createStringLong(number);
    this->class->add(this, numberTemp->class->getValue(numberTemp));

    delete(numberTemp);
}

static void __string_addULong(void *_this, unsigned long int number) {
    String *numberTemp = createStringULong(number);
    this->class->add(this, numberTemp->class->getValue(numberTemp));

    delete(numberTemp);
}

static int __string_toInt(void *_this) {
    int thisLength = this->class->length(this);
    int zeroCount = thisLength - 1;

    int result = 0;
    int isPositive = 1;
    for (int x = 0; x < thisLength; x++) {
        char character = this->class->charAt(this, x);

        if (x == 0 && character == 45) {
            isPositive = 0;
            continue;
        }

        if (((character < 48 || character > 57) && x != 0) || 
            (x == 0 && character != 45 && ((character < 48 || character > 57))))
            return 0;

        int plus = character - 48;
        for (int y = 0; y < zeroCount - x; y++)
            plus *= 10;

        if (isPositive)
            result  += plus;
        else result -= plus;
    }

    return result;
}

static char __string_charAt(void *_this, int index) {
    return this->class->getValue(this)[index];
}

static int __string_stringLength(void *_this) {
    return (int) strlen(this->class->getValue(this));
}

static bool __string_equals(void *_this, String *subject) {
    return (bool) strcmp(this->class->getValue(this), subject->class->getValue(subject)) == 0;
}

static bool __string_equalsChr(void *_this, char *subject) {
    return (bool) strcmp(this->class->getValue(this), subject) == 0;
}

static String *__string_toString(void *_this) {
    return this->class->_impl_CCObject.copy(this);
}

static void *__string_copy(void *_this) {
    String *newString = createStringChar(this->class->getValue(this));

    return newString;
}

static String *createStringNull(void *value) {
    String *newString   = (String *) malloc(sizeof(String));
    newString->_private = NULL;
    newString->class    = &ClassString;
    newString->_class   = &classString;

    return newString;
}

extern String *createStringChar(char *value) {
    int memSize = (int) (sizeof(char) * strlen(value));

    String *newString = createStringNull(NULL);
    newString->_private = malloc(sizeof(Private *));

    Private *private = (Private *) newString->_private;
    private->stringValue = (char *) malloc((size_t) memSize + 1);

    memcpy(private->stringValue, value, (size_t) memSize);
    memcpy(private->stringValue + memSize, "\0", 1);

    return newString;
}

static String *__createStringLong(long int number, bool isUnsigned) {
    String *newString = createStringNull(NULL);
    newString->_private = malloc(sizeof(Private));
    Private *private = (Private *) newString->_private;

    int length = 1;
    int divisior = 10;
    while (floor((double) number / divisior) > 0) {
        length++;
        divisior *= 10;
    }

    private->stringValue = malloc(sizeof(char) * (size_t) length + 1);

    sprintf(private->stringValue, isUnsigned ? "%lu" : "%ld", number);

    return newString;
}

extern String *createStringLong(long int number) {
    return __createStringLong(number, false);
}

extern String *createStringULong(unsigned long int number) {
    return __createStringLong((long int) number, true);
}

static void __string_delete(void *_this) {
    Private *stringValue = (Private *) this->_private;
    
    free(stringValue->stringValue);
    free(stringValue);
    free(this);
}

extern ClassStringType ClassString = {
    &__string_get,
    &__string_set,
    &__string_add,
    &__string_sub,
#ifndef _WIN32
    &__string_replace,
    &__string_replaceFirst,
    &__string_match,
    &__string_split,
#endif
    &__string_addLong,
    &__string_addULong,
    &__string_toInt,
    &__string_charAt,
    &__string_stringLength,
    &__string_equals,
    &__string_equalsChr,
    {
        INTERFACE_CCOBJECT,
        &__string_toString,
        &__string_copy
    }
};

extern Class classString = {
    .classType = CLASS_STRING,
    .delete    = &__string_delete
};
