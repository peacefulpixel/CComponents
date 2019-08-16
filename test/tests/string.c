#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {

    // Testing constructor
    String *string = newString("Hello ");

    assert(string->length(string) == 6);

    String *int_string = newString(41);
    assert(int_string->length(int_string) == 2);

    deleteString(int_string);

    String *long_string = newString(-1744674407709551614);
    assert(!(strcmp(long_string->getValue(long_string), "-1744674407709551614")));

    deleteString(long_string);

    String *ulong_string = newString((unsigned long int) -1744674407709551614);
    assert(!(strcmp(ulong_string->getValue(ulong_string), "16702069666000000002")));

    // Testing addULong()
    ulong_string->addULong(ulong_string, -1);
    assert(!(strcmp(ulong_string->getValue(ulong_string), "1670206966600000000218446744073709551615")));

    deleteString(ulong_string);

    // Testing add() and addLong()
    string->add(string, "world!");
    assert(!(strcmp(string->getValue(string), "Hello world!")));

    string->addLong(string, 93);
    assert(!(strcmp(string->getValue(string), "Hello world!93")));
    assert(string->length(string) == 14);

    // Testing charAt(), equals() and equalsChr()
    assert(string->charAt(string, 4) == 'o');
    assert(string->equalsChr(string, "Hello world!93"));
    assert(!string->equalsChr(string, "Hello world!83"));

    String *new_str = newString("Hello world!93");
    assert(string->equals(string, new_str));

    new_str->setValue(new_str, "Hello world!83");
    assert(!string->equals(string, new_str));

    deleteString(new_str);

    // Testing match()
    String *regex = newString("o");
    StringMatch *match = string->match(string, regex->getValue(regex), 3);

    assert(match[0].begin == 4 && match[0].end == 5 &&
           match[1].begin == 7 && match[1].end == 8 );

    deleteString(regex);
    free(match);

    // Testing replace() and replaceFirst()
    string->replaceFirst(string, "l", "");
    assert(string->equalsChr(string, "Helo world!93"));
    assert(string->length(string) == 13);
    
    string->replace(string, "l", "");
    assert(string->equalsChr(string, "Heo word!93"));
    assert(string->length(string) == 11);

    // Testing split()
    List *parts = string->split(string, " ");
    String *part_a = (String *) parts->get(parts, 0);
    String *part_b = (String *) parts->get(parts, 1);

    assert(part_a->equalsChr(part_a, "Heo"));
    assert(part_b->equalsChr(part_b, "word!93"));

    deleteString(part_a);
    deleteString(part_b);
    deleteList(parts);

    // Testing sub()
    new_str = string->sub(string, 9, 11);
    assert(new_str->equalsChr(new_str, "93"));
    assert(new_str->length(new_str) == 2);

    // Testing toInt()
    assert(new_str->toInt(new_str) == 93);

    deleteString(new_str);

    // Testing toString()
    String *toStringTest = string->toString(string);
    assert(toStringTest->equals(toStringTest, string));

    deleteString(toStringTest);

    // Testing copy()
    String *copy = string->copy(string);
    assert(copy->equals(copy, string));

    deleteString(copy);
    deleteString(string);

    return 0;
}