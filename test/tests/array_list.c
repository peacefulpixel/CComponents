#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {
    
    // Testing constructor
    ArrayList *list = CreateArrayList();
    assert(ClassArrayList._impl_List.length(list) == 0);

    // Testing push()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };

    for (int x = 0; x < 4; x++)
        ClassArrayList._impl_List.push(list, testData[x]);

    assert(ClassArrayList._impl_List.length(list) == 4);

    for (int x = 0; x < 4; x++) 
        assert(!(strcmp(ClassArrayList._impl_List.get(list, x), testData[x])));

    // Testing set() & get()
    ClassArrayList._impl_List.set(list, 2, testData[0]);

    assert(ClassArrayList._impl_List.length(list) == 4);
    assert(ClassArrayList._impl_List.get(list, 2) == ClassArrayList._impl_List.get(list, 0));

    // Testing remove() & get()
    ClassArrayList._impl_List.remove(list, 0);

    assert(ClassArrayList._impl_List.length(list) == 3);
    assert(!(strcmp(ClassArrayList._impl_List.get(list, 0), testData[1])) &&
           !(strcmp(ClassArrayList._impl_List.get(list, 1), testData[0])) &&
           !(strcmp(ClassArrayList._impl_List.get(list, 2), testData[3])) );

    // Testing toString()
    String *listAsString = ClassArrayList._impl_List._impl_CCObject.toString(list);
    delete(listAsString);

    // Testing include()
    char *testDataFill[] = 
        {
            "A", "B", "C"
        };

    ClassArrayList.include(list, (void **) testDataFill, 3);
    assert(!(strcmp(ClassArrayList._impl_List.get(list, 3), testDataFill[0])) &&
           !(strcmp(ClassArrayList._impl_List.get(list, 4), testDataFill[1])) &&
           !(strcmp(ClassArrayList._impl_List.get(list, 5), testDataFill[2])) );

    assert(ClassArrayList._impl_List.length(list) == 6);

    // Testing copy()
    ArrayList *copy = ClassArrayList._impl_List._impl_CCObject.copy(list);
    assert(ClassArrayList._impl_List.length(copy) == 6);

    for (int index = 0; index < 6; index++)
        assert(ClassArrayList._impl_List.get(copy, index) == ClassArrayList._impl_List.get(list, index));

    delete(copy);
    delete(list);

    return 0;
}