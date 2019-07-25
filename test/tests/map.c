#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "../test.h"
#include "../../src/map.h"

TestResult *testSysMap() {
    TestResult *result = malloc(sizeof(TestResult));
    result->testName = "sys/map";
    result->message = NULL;

    // Testing constructor
    Map *map = newMap(sizeof(char **));
    
    if (map->length(map) != 0)
        return setMessage(result, "(0) map->length returns incorrect value");

    if (map->elementSize != sizeof(char **))
        return setMessage(result, "(0) map->elementSize has incorrect value");

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

    if (map->length(map) != 4)
        return setMessage(result, "(1) map->length returns incorrect value");

    if (map->get(map, "2") != map->get(map, "0"))
        return setMessage(result, "(0) map->get returns invalid data");

    // Testing remove() & get()
    map->remove(map, "0");

    if (map->length(map) != 3)
        return setMessage(result, "(2) map->length returns incorrect value");

    if (strcmp(map->get(map, "1"), testData[1]) != 0 ||
        strcmp(map->get(map, "2"), testData[0]) != 0 ||
        strcmp(map->get(map, "3"), testData[3]) != 0 )
        return setMessage(result, "(1) map->get returns invalid data");

    deleteMap(map);

    return result;
}