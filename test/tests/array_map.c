#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {

    // Testing constructor
    ArrayMap *map = CreateArrayMap();
    
    assert(ClassArrayMap._impl_Map.length(map) == 0);

    // Testing set() & get()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };
    
    ClassArrayMap._impl_Map.set(map, "0", testData[0]);
    ClassArrayMap._impl_Map.set(map, "1", testData[1]);
    ClassArrayMap._impl_Map.set(map, "2", testData[2]);
    ClassArrayMap._impl_Map.set(map, "3", testData[3]);

    ClassArrayMap._impl_Map.set(map, "2", testData[0]);

    assert(ClassArrayMap._impl_Map.length(map) == 4);
    assert(ClassArrayMap._impl_Map.get(map, "2") == ClassArrayMap._impl_Map.get(map, "0"));

    // Testing remove() & get()
    ClassArrayMap._impl_Map.remove(map, "0");

    assert(ClassArrayMap._impl_Map.length(map) == 3);
    assert(!(strcmp(ClassArrayMap._impl_Map.get(map, "1"), testData[1])) &&
           !(strcmp(ClassArrayMap._impl_Map.get(map, "2"), testData[0])) &&
           !(strcmp(ClassArrayMap._impl_Map.get(map, "3"), testData[3])) );

    // Testing toString()
    String *mapAsString = ClassArrayMap._impl_Map._impl_CCObject.toString(map);
    delete(mapAsString);

    // Testing copy()
    ArrayMap *copy = ClassArrayMap._impl_Map._impl_CCObject.copy(map);
    assert(!(strcmp(ClassArrayMap._impl_Map.get(copy, "1"), testData[1])) &&
           !(strcmp(ClassArrayMap._impl_Map.get(copy, "2"), testData[0])) &&
           !(strcmp(ClassArrayMap._impl_Map.get(copy, "3"), testData[3])) );

    delete(copy);
    delete(map);

    return 0;
}