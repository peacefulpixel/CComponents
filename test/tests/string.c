#include <stdlib.h>
#include <string.h>

#include "string.h"
#include "../test.h"
#include "../../src/ccomponents.h"

TestResult *testSysString() {
    TestResult *result = malloc(sizeof(TestResult));
    result->testName = "sys/string";
    result->message = NULL;

    // Testing constructor
    String *string = newString("Hello ");

    if (string->length(string) != 6)
        return setMessage(result, "(0) string->length returns incorrect value");

    String *int_string = newString(41);
    if (int_string->length(int_string) != 2)
        return setMessage(result, "(1) string->length returns incorrect value");

    deleteString(int_string);

    // Testing add() and addInt()
    string->add(string, "world!");

    if (strcmp(string->getValue(string), "Hello world!"))
        return setMessage(result, "(0) string->add was failed");

    string->addInt(string, 93);

    if (strcmp(string->getValue(string), "Hello world!93"))
        return setMessage(result, "(0) string->addInt was failed");
    
    if (string->length(string) != 14)
        return setMessage(result, "(2) string->length returns incorrect value");

    // Testing charAt(), equals() and equalsChr()
    if (string->charAt(string, 4) != 'o')
        return setMessage(result, "(0) string->charAt returns incorrect value");

    if (!string->equalsChr(string, "Hello world!93"))
        return setMessage(result, "(0) string->equalsChr returns incorrect value");

    if (string->equalsChr(string, "Hello world!83"))
        return setMessage(result, "(1) string->equalsChr returns incorrect value");

    String *new_str = newString("Hello world!93");
    if (!string->equals(string, new_str))
        return setMessage(result, "(0) string->equals returns incorrect value");

    new_str->setValue(new_str, "Hello world!83");
    if (string->equals(string, new_str))
        return setMessage(result, "(1) string->equals returns incorrect value");

    deleteString(new_str);

    // Testing match()
    String *regex = newString("o");
    StringMatch *match = string->match(string, regex->getValue(regex), 3);

    if (match[0].begin != 4 || match[0].end != 5 ||
        match[1].begin != 7 || match[1].end != 8)
        return setMessage(result, "(0) string->match returns incorrect value");

    deleteString(regex);
    free(match);

    // Testing replace() and replaceFirst()
    string->replaceFirst(string, "l", "");
    if (!string->equalsChr(string, "Helo world!93"))
        return setMessage(result, "(2) string->equalsChr returns incorrect value");

    if (string->length(string) != 13)
        return setMessage(result, "(3) string->length returns incorrect value");
    
    string->replace(string, "l", "");
    if (!string->equalsChr(string, "Heo word!93"))
        return setMessage(result, "(3) string->equalsChr returns incorrect value");

    if (string->length(string) != 11)
        return setMessage(result, "(4) string->length returns incorrect value");

    // Testing split()
    List *parts = string->split(string, " ");
    String *part_a = (String *) parts->get(parts, 0);
    String *part_b = (String *) parts->get(parts, 1);

    if (!part_a->equalsChr(part_a, "Heo"))
        return setMessage(result, "(4) string->equalsChr returns incorrect value");

    if (!part_b->equalsChr(part_b, "word!93"))
        return setMessage(result, "(5) string->equalsChr returns incorrect value");

    deleteString(part_a);
    deleteString(part_b);
    deleteList(parts);

    // Testing sub()
    new_str = string->sub(string, 9, 11);
    if (!new_str->equalsChr(new_str, "93"))
        return setMessage(result, "(6) string->equalsChr returns incorrect value");
    
    if (new_str->length(new_str) != 2)
        return setMessage(result, "(5) string->length returns incorrect value");

    // Testing toInt()
    if (new_str->toInt(new_str) != 93)
        return setMessage(result, "(0) string->toInt returns incorrect value");

    deleteString(new_str);
    deleteString(string);

    return result;
}