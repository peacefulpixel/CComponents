#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {

    // Testing constructor
    ArrayMap *map = CreateArrayMap();
    
    assert(ClassArrayMap.length(map) == 0);

    // Testing set() & get()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };
    
    ClassArrayMap.set(map, "0", testData[0]);
    ClassArrayMap.set(map, "1", testData[1]);
    ClassArrayMap.set(map, "2", testData[2]);
    ClassArrayMap.set(map, "3", testData[3]);

    ClassArrayMap.set(map, "2", testData[0]);

    assert(ClassArrayMap.length(map) == 4);
    assert(ClassArrayMap.get(map, "2") == ClassArrayMap.get(map, "0"));

    // Testing remove() & get()
    ClassArrayMap.remove(map, "0");

    assert(ClassArrayMap.length(map) == 3);
    assert(!(strcmp(ClassArrayMap.get(map, "1"), testData[1])) &&
           !(strcmp(ClassArrayMap.get(map, "2"), testData[0])) &&
           !(strcmp(ClassArrayMap.get(map, "3"), testData[3])) );

    // Testing toString()
    String *mapAsString = ClassArrayMap.toString(map);
    delete(mapAsString);

    // Testing copy()
    ArrayMap *copy = ClassArrayMap.copy(map);
    assert(!(strcmp(ClassArrayMap.get(copy, "1"), testData[1])) &&
           !(strcmp(ClassArrayMap.get(copy, "2"), testData[0])) &&
           !(strcmp(ClassArrayMap.get(copy, "3"), testData[3])) );

    delete(copy);
    delete(map);

    return 0;
}