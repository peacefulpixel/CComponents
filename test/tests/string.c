#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {

    // Testing constructor
    String *string = CreateString("Hello ");

    assert(ClassString.length(string) == 6);

    String *int_string = CreateString(41);
    assert(ClassString.length(int_string) == 2);

    delete(int_string);

    String *long_string = CreateString(-1744674407709551614);
    assert(!(strcmp(ClassString.getValue(long_string), "-1744674407709551614")));

    delete(long_string);

    String *ulong_string = CreateString((unsigned long int) -1744674407709551614);
    assert(!(strcmp(ClassString.getValue(ulong_string), "16702069666000000002")));

    // Testing addULong()
    ClassString.addULong(ulong_string, -1);
    assert(!(strcmp(ClassString.getValue(ulong_string), "1670206966600000000218446744073709551615")));

    delete(ulong_string);

    // Testing add() and addLong()
    ClassString.add(string, "world!");
    assert(!(strcmp(ClassString.getValue(string), "Hello world!")));

    ClassString.addLong(string, 93);
    assert(!(strcmp(ClassString.getValue(string), "Hello world!93")));
    assert(ClassString.length(string) == 14);

    // Testing charAt(), equals() and equalsChr()
    assert(ClassString.charAt(string, 4) == 'o');
    assert(ClassString.equalsChr(string, "Hello world!93"));
    assert(!ClassString.equalsChr(string, "Hello world!83"));

    String *new_str = CreateString("Hello world!93");
    assert(ClassString.equals(string, new_str));

    ClassString.setValue(new_str, "Hello world!83");
    assert(!ClassString.equals(string, new_str));

    delete(new_str);

    // Testing match()
    String *regex = CreateString("o");
    StringMatch *match = ClassString.match(string, ClassString.getValue(regex), 3);

    assert(match[0].begin == 4 && match[0].end == 5 &&
           match[1].begin == 7 && match[1].end == 8 );

    delete(regex);
    free(match);

    // Testing replace() and replaceFirst()
    ClassString.replaceFirst(string, "l", "");
    assert(ClassString.equalsChr(string, "Helo world!93"));
    assert(ClassString.length(string) == 13);
    
    ClassString.replace(string, "l", "");
    assert(ClassString.equalsChr(string, "Heo word!93"));
    assert(ClassString.length(string) == 11);

    // Testing split()
    ArrayList *parts = ClassString.split(string, " ");
    String *part_a = (String *) ClassArrayList.get(parts, 0);
    String *part_b = (String *) ClassArrayList.get(parts, 1);

    assert(ClassString.equalsChr(part_a, "Heo"));
    assert(ClassString.equalsChr(part_b, "word!93"));

    delete(part_a);
    delete(part_b);
    delete(parts);

    // Testing sub()
    new_str = ClassString.sub(string, 9, 11);
    assert(ClassString.equalsChr(new_str, "93"));
    assert(ClassString.length(new_str) == 2);

    // Testing toInt()
    assert(ClassString.toInt(new_str) == 93);

    delete(new_str);

    // Testing toString()
    String *toStringTest = ClassString.toString(string);
    assert(ClassString.equals(toStringTest, string));

    delete(toStringTest);

    // Testing copy()
    String *copy = ClassString.copy(string);
    assert(ClassString.equals(copy, string));

    delete(copy);
    delete(string);

    return 0;
}