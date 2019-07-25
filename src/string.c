#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

typedef struct _string_value_private {
    char *stringValue;
} StringValuePrivate;

char *_get_value(String *this) {
    StringValuePrivate *stringValue = (StringValuePrivate *) this->_value;
    return stringValue->stringValue;
}

void _set_value(String *this, char *value) {
    int memSize = sizeof(char) * strlen(value);

    free(((StringValuePrivate *) this->_value)->stringValue);

    StringValuePrivate *stringValue = (StringValuePrivate *) this->_value;
    stringValue->stringValue = malloc(memSize + 1);

    memcpy(stringValue->stringValue, value, memSize);
    memcpy(stringValue->stringValue + memSize, "\0", 1);
}

void _add(String *this, char *value) {
    int aSize = sizeof(char) * strlen(this->getValue(this));
    int bSize = sizeof(char) * strlen(value);

    char *aDump = malloc(aSize);
    memcpy(aDump, this->getValue(this), aSize);

    StringValuePrivate *stringValue = (StringValuePrivate *) this->_value;
    stringValue->stringValue = malloc(aSize + bSize + 1);
 
    memcpy(stringValue->stringValue, aDump, aSize);
    memcpy(stringValue->stringValue + aSize, value, bSize);
    memcpy(stringValue->stringValue + aSize + bSize, "\0", 1);

    free(aDump);
}

String *_sub(struct _sys_string *this, int begin, int end) {
    int size = sizeof(char) * (end - begin);

    char *newValue = malloc(size + 1);
    memcpy(newValue, this->getValue(this) + begin, size);
    memcpy(newValue + size, "\0", 1);

    String *new = newStringChar(newValue);
    free(newValue);

    return new;
}

void _replace(struct _sys_string *this, char *regex, char *value) {
    String *current = newStringChar(this->getValue(this));
    StringMatch *match = current->match(current, regex, 1);

    String *builder = newStringChar("");

    int finded = 0;
    while (match->begin != -1 && match->begin != -2) {
        String *start = current->sub(current, 0, match->begin);
        builder->add(builder, start->getValue(start));
        builder->add(builder, value);

        deleteString(start);

        String *newCurrent = current->sub(current, match->end, current->length(current));
        deleteString(current);
        current = newCurrent;
        finded = 1;
        free(match);
        match = current->match(current, regex, 1);
    }

    if (finded)
        builder->add(builder, current->getValue(current));

    free(match);
    if (!builder->equalsChr(builder, ""))
        this->setValue(this, builder->getValue(builder));

    deleteString(current);
    deleteString(builder);
}

void _replaceFirst(struct _sys_string *this, char *regex, char *value) {
    StringMatch *match = this->match(this, regex, 1);

    if (match->begin == -1 || match->begin == -2)
        return;

    char *oldValue = this->getValue(this);
    int size = strlen(oldValue) - strlen(regex) + strlen(value);
    char *newValue = malloc(sizeof(char) * (size + 1));

    memcpy(newValue, oldValue, match->begin);
    memcpy(newValue + match->begin, value, strlen(value));
    memcpy(newValue + match->begin + strlen(value), oldValue + match->end, strlen(oldValue + match->end));
    memcpy(newValue + size, "\0", 1);

    this->setValue(this, newValue);

    free(match);
    free(newValue);
}

StringMatch *_match(String *this, char *regex, int maxMatchesCount) {
    regex_t regexObject;

    StringMatch *matchesResult = malloc(maxMatchesCount * sizeof(matchesResult));

    int compileResult = regcomp(&regexObject, regex, REG_EXTENDED);
    if (compileResult != 0) {
        for (int x = 0; x < maxMatchesCount; x++) {
            matchesResult[x].begin = -2;
            matchesResult[x].end   = -2;
        }
    } else  {
        regmatch_t matches[1];
        
        int cuttedLength = 0;
        String *copy = newString(this->getValue(this));
        for (int x = 0; x < maxMatchesCount; x++) {
            int result = regexec(&regexObject, copy->getValue(copy), 1, matches, REG_NOERROR);

            matchesResult[x].begin = result ? -1 : cuttedLength + matches[0].rm_so;
            matchesResult[x].end   = result ? -1 : cuttedLength + matches[0].rm_eo;

            if (!result) {
                String *newCopy = copy->sub(copy, matches[0].rm_eo, copy->length(copy));

                cuttedLength += matches[0].rm_eo;

                deleteString(copy);
                copy = newCopy;
            }
        }
    }

    regfree(&regexObject);
    return matchesResult;
}

List *_split(String *this, char *regex) {
    StringMatch *match = this->match(this, regex, 1);
    List *result = newList(sizeof(String *));

    String *copy = this->sub(this, 0, this->length(this));
    while (match->begin != -1) {
        String *sub = copy->sub(copy, 0, match->begin);
        String *newCopy = copy->sub(copy, match->end, copy->length(copy));
        deleteString(copy);
        copy = newCopy;

        result->push(result, sub);
        match = copy->match(copy, regex, 1);
    } result->push(result, newString(copy->getValue(copy)));

    deleteString(copy);
    free(match);
    return result;
}

void _addInt(struct _sys_string *this, int number) {
    String *numberTemp = newStringInt(number);
    this->add(this, numberTemp->getValue(numberTemp));

    deleteString(numberTemp);
}

int _toInt(struct _sys_string *this) {
    int thisLength = this->length(this);
    int zeroCount = thisLength - 1;

    int result = 0;
    int isPositive = 1;
    for (int x = 0; x < thisLength; x++) {
        char character = this->charAt(this, x);

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

char _charAt(struct _sys_string *this, int index) {
    return this->getValue(this)[index];
}

int _stringLength(struct _sys_string *this) {
    return strlen(this->getValue(this));
}

int _equals(struct _sys_string *this, struct _sys_string *subject) {
    return strcmp(this->getValue(this), subject->getValue(subject)) == 0;
}

int _equalsChr(struct _sys_string *this, char *subject) {
    return strcmp(this->getValue(this), subject) == 0;
}

String *newStringNull(void *value) {
    String *new       = (String *) malloc(sizeof(String));
    new->_value       = NULL;
    new->getValue     = &_get_value;
    new->setValue     = &_set_value;
    new->add          = &_add;
    new->sub          = &_sub;
    new->replace      = &_replace;
    new->replaceFirst = &_replaceFirst;
    new->match        = &_match;
    new->split        = &_split;
    new->addInt       = &_addInt;
    new->toInt        = &_toInt;
    new->charAt       = &_charAt;
    new->length       = &_stringLength;
    new->equals       = &_equals;
    new->equalsChr    = &_equalsChr;

    return new;
}

String *newStringChar(char *value) {
    int memSize = sizeof(char) * strlen(value);

    String *new = newStringNull(NULL);
    new->_value = (void *) malloc(sizeof(StringValuePrivate));

    StringValuePrivate *stringValue = (StringValuePrivate *) new->_value;
    stringValue->stringValue = malloc(memSize + 1);

    memcpy(stringValue->stringValue, value, memSize);
    memcpy(stringValue->stringValue + memSize, "\0", 1);

    return new;
}

String *newStringInt(int number) {
    String *new = newStringNull(NULL);
    new->_value = malloc(sizeof(StringValuePrivate));
    StringValuePrivate *stringValue = (StringValuePrivate *) new->_value;

    int length = 1;
    int divisior = 10;
    while (floor(number / divisior) > 0) {
        length++;
        divisior *= 10;
    }

    stringValue->stringValue = malloc(sizeof(char) * length + 1);

    sprintf(stringValue->stringValue, "%d", number);

    return new;
}

void deleteString(String *this) {
    StringValuePrivate *stringValue = (StringValuePrivate *) this->_value;
    
    free(stringValue->stringValue);
    free(stringValue);
    free(this);
}