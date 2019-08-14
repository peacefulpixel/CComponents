#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "../test.h"
#include "../../src/ccomponents.h"

TestResult *testSysList() {
    TestResult *result = malloc(sizeof(TestResult));
    result->testName   = "sys/list";
    result->message    = NULL;

    // Testing constructor
    List *list = newList(sizeof(char **));
    
    if (list->length(list) != 0)
        return setMessage(result, "(0) list->length returns incorrect value");

    if (list->elementSize != sizeof(char **))
        return setMessage(result, "(0) list->elementSize has incorrect value");

    // Testing push()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };

    for (int x = 0; x < 4; x++)
        list->push(list, testData[x]);

    if (list->length(list) != 4)
        return setMessage(result, "(1) list->length returns incorrect value");

    for (int x = 0; x < 4; x++) 
        if (strcmp(list->get(list, x), testData[x]) != 0)
            return setMessage(result, "(0) list->get returns invalid data");

    if (list->elementSize != sizeof(char **))
        return setMessage(result, "(1) list->elementSize has incorrect value");

    // Testing set() & get()
    list->set(list, 2, testData[0]);

    if (list->length(list) != 4)
        return setMessage(result, "(2) list->length returns incorrect value");

    if (list->get(list, 2) != list->get(list, 0))
        return setMessage(result, "(1) list->get returns invalid data");

    // Testing remove() & get()
    list->remove(list, 0);

    if (list->length(list) != 3)
        return setMessage(result, "(3) list->length returns incorrect value");

    if (strcmp(list->get(list, 0), testData[1]) != 0 ||
        strcmp(list->get(list, 1), testData[0]) != 0 ||
        strcmp(list->get(list, 2), testData[3]) != 0 )
        return setMessage(result, "(2) list->get returns invalid data");

    // Testing toString()
    String *listAsString = list->toString(list);
    deleteString(listAsString);

    // Testing include()
    char *testDataFill[] = 
        {
            "A", "B", "C"
        };

    list->include(list, (void **) testDataFill, 3);
    if (strcmp(list->get(list, 3), testDataFill[0]) != 0 ||
        strcmp(list->get(list, 4), testDataFill[1]) != 0 ||
        strcmp(list->get(list, 5), testDataFill[2]) != 0 )
        return setMessage(result, "(0) list->include was failed");

    if (list->length(list) != 6)
        return setMessage(result, "(1) list->include was failed");

    // Testing copy()
    List *copy = list->copy(list);
    if (copy->length(copy) != 6)
        return setMessage(result, "(0) list->copy was failed");

    for (int index = 0; index < 6; index++) {
        if (copy->get(copy, index) != list->get(list, index))
            return setMessage(result, "(0) list->copy was failed");
    }

    deleteList(copy);
    deleteList(list);

    return result;
}