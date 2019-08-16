#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {

    // Testing constructor
    Map *map = newMap(sizeof(char **));
    
    assert(map->length(map) == 0);
    assert(map->elementSize == sizeof(char **));

    // Testing set() & get()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };
    
    map->set(map, "0", testData[0]);
    map->set(map, "1", testData[1]);
    map->set(map, "2", testData[2]);
    map->set(map, "3", testData[3]);

    map->set(map, "2", testData[0]);

    assert(map->length(map) == 4);
    assert(map->get(map, "2") == map->get(map, "0"));

    // Testing remove() & get()
    map->remove(map, "0");

    assert(map->length(map) == 3);
    assert(!(strcmp(map->get(map, "1"), testData[1])) &&
           !(strcmp(map->get(map, "2"), testData[0])) &&
           !(strcmp(map->get(map, "3"), testData[3])) );

    // Testing toString()
    String *mapAsString = map->toString(map);
    deleteString(mapAsString);

    // Testing copy()
    Map *copy = map->copy(map);
    assert(!(strcmp(copy->get(copy, "1"), testData[1])) &&
           !(strcmp(copy->get(copy, "2"), testData[0])) &&
           !(strcmp(copy->get(copy, "3"), testData[3])) );

    deleteMap(copy);
    deleteMap(map);

    return 0;
}