#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {

    // Testing constructor
    Map *map = CreateMap();
    
    assert(ClassMap.length(map) == 0);

    // Testing set() & get()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };
    
    ClassMap.set(map, "0", testData[0]);
    ClassMap.set(map, "1", testData[1]);
    ClassMap.set(map, "2", testData[2]);
    ClassMap.set(map, "3", testData[3]);

    ClassMap.set(map, "2", testData[0]);

    assert(ClassMap.length(map) == 4);
    assert(ClassMap.get(map, "2") == ClassMap.get(map, "0"));

    // Testing remove() & get()
    ClassMap.remove(map, "0");

    assert(ClassMap.length(map) == 3);
    assert(!(strcmp(ClassMap.get(map, "1"), testData[1])) &&
           !(strcmp(ClassMap.get(map, "2"), testData[0])) &&
           !(strcmp(ClassMap.get(map, "3"), testData[3])) );

    // Testing toString()
    String *mapAsString = ClassMap.toString(map);
    delete(mapAsString);

    // Testing copy()
    Map *copy = ClassMap.copy(map);
    assert(!(strcmp(ClassMap.get(copy, "1"), testData[1])) &&
           !(strcmp(ClassMap.get(copy, "2"), testData[0])) &&
           !(strcmp(ClassMap.get(copy, "3"), testData[3])) );

    delete(copy);
    delete(map);

    return 0;
}