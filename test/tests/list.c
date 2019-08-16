#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {
    
    // Testing constructor
    List *list = newList(sizeof(char **));
    assert(list->length(list) == 0);
    assert(list->elementSize == sizeof(char **));

    // Testing push()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };

    for (int x = 0; x < 4; x++)
        list->push(list, testData[x]);

    assert(list->length(list) == 4);

    for (int x = 0; x < 4; x++) 
        assert(!(strcmp(list->get(list, x), testData[x])));

    assert(list->elementSize == sizeof(char **));

    // Testing set() & get()
    list->set(list, 2, testData[0]);

    assert(list->length(list) == 4);
    assert(list->get(list, 2) == list->get(list, 0));

    // Testing remove() & get()
    list->remove(list, 0);

    assert(list->length(list) == 3);
    assert(!(strcmp(list->get(list, 0), testData[1])) &&
           !(strcmp(list->get(list, 1), testData[0])) &&
           !(strcmp(list->get(list, 2), testData[3])) );

    // Testing toString()
    String *listAsString = list->toString(list);
    deleteString(listAsString);

    // Testing include()
    char *testDataFill[] = 
        {
            "A", "B", "C"
        };

    list->include(list, (void **) testDataFill, 3);
    assert(!(strcmp(list->get(list, 3), testDataFill[0])) &&
           !(strcmp(list->get(list, 4), testDataFill[1])) &&
           !(strcmp(list->get(list, 5), testDataFill[2])) );

    assert(list->length(list) == 6);

    // Testing copy()
    List *copy = list->copy(list);
    assert(copy->length(copy) == 6);

    for (int index = 0; index < 6; index++)
        assert(copy->get(copy, index) == list->get(list, index));

    deleteList(copy);
    deleteList(list);

    return 0;
}