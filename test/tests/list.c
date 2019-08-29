#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {
    
    // Testing constructor
    List *list = CreateList();
    assert(ClassList.length(list) == 0);

    // Testing push()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };

    for (int x = 0; x < 4; x++)
        ClassList.push(list, testData[x]);

    assert(ClassList.length(list) == 4);

    for (int x = 0; x < 4; x++) 
        assert(!(strcmp(ClassList.get(list, x), testData[x])));

    // Testing set() & get()
    ClassList.set(list, 2, testData[0]);

    assert(ClassList.length(list) == 4);
    assert(ClassList.get(list, 2) == ClassList.get(list, 0));

    // Testing remove() & get()
    ClassList.remove(list, 0);

    assert(ClassList.length(list) == 3);
    assert(!(strcmp(ClassList.get(list, 0), testData[1])) &&
           !(strcmp(ClassList.get(list, 1), testData[0])) &&
           !(strcmp(ClassList.get(list, 2), testData[3])) );

    // Testing toString()
    String *listAsString = ClassList.toString(list);
    delete(listAsString);

    // Testing include()
    char *testDataFill[] = 
        {
            "A", "B", "C"
        };

    ClassList.include(list, (void **) testDataFill, 3);
    assert(!(strcmp(ClassList.get(list, 3), testDataFill[0])) &&
           !(strcmp(ClassList.get(list, 4), testDataFill[1])) &&
           !(strcmp(ClassList.get(list, 5), testDataFill[2])) );

    assert(ClassList.length(list) == 6);

    // Testing copy()
    List *copy = ClassList.copy(list);
    assert(ClassList.length(copy) == 6);

    for (int index = 0; index < 6; index++)
        assert(ClassList.get(copy, index) == ClassList.get(list, index));

    delete(copy);
    delete(list);

    return 0;
}