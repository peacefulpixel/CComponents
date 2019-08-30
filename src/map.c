#include <stdint.h>
#include <stdlib.h>

#include "ccomponents.h"

#define P_SIZE sizeof(intptr_t)

typedef struct _map_private {
    List *values;
    List *keys;
    unsigned long int mapSize;
} Private;

static long int __map_indexOfKey(List *keys, char *key) {
    for (unsigned long int index = 0; index < keys->class->length(keys); index++) {
        String *currentKey = (String *) keys->class->get(keys, index);
        if (currentKey->class->equalsChr(currentKey, key))
            return index;
    } return -1;
}

static void __map_remove(Map *this, char *key) {
    Private *private = (Private *) this->_private;

    unsigned long int index = __map_indexOfKey(private->keys, key);

    delete(((String *) private->keys->class->get(private->keys, index)));
    private->keys->class->remove(private->keys, index);
    private->values->class->remove(private->values, index);

    private->mapSize--;
}

static void __map_set(Map *this, char *key, void *value) {
    Private *private = (Private *) this->_private;
    unsigned long int index = __map_indexOfKey(private->keys, key);

    if (index == -1) {
        String *newKey = CreateString(key);
        private->keys->class->push(private->keys, newKey);

        void *randomMemory = malloc((size_t) P_SIZE);
        private->values->class->push(private->values, randomMemory);
        free(randomMemory);

        private->mapSize++;

        // More fast then call __map_indexOfKey() again
        index = private->keys->class->length(private->keys) - 1;
    }

    private->values->class->set(private->values, index, value);
}

static void *__map_get(Map *this, char *key) {
    Private *private = (Private *) this->_private;
    long int index = __map_indexOfKey(private->keys, key);

    if (index == -1) //TODO: Catch the element existence by another way. __map_indexOfKey must return unsigned long int
        return NULL;

    return private->values->class->get(private->values, (unsigned long int) index);
}

static unsigned long int __map_length(Map *this) {
    Private *private = (Private *) this->_private;

    return private->mapSize;
}

static String *__map_toString(Map *this) {
    unsigned long int mapLength = this->class->length(this);
    Private *private = (Private *) this->_private;
    List *keys = private->keys;

    String *result = CreateString("Map: [ ");
    for (unsigned long int index = 0; index < mapLength; index++) {
        String *k = (String *) keys->class->get(keys, index);
        uintptr_t v = (uintptr_t) this->class->get(this, k->class->getValue(k));

        result->class->add(result, k->class->getValue(k));
        result->class->add(result, ":");
        result->class->addULong(result, (unsigned long int) v);
        if (index != mapLength - 1) {
            result->class->add(result, ", ");
        }
    }

    result->class->add(result, " ] (");
    result->class->addULong(result, (unsigned long int) mapLength);
    result->class->add(result, ");");

    return result;
}

static Map *__map_copy(Map *this) {
    Map *newMap = createMap();
    Private *nmPrivate = (Private *) newMap->_private;
    List *nKeys = nmPrivate->keys;
    List *nVals = nmPrivate->values;

    Private *private = (Private *) this->_private;
    List *keys = private->keys;
    List *vals = private->values;

    for (unsigned long int index = 0; index < this->class->length(this); index++) {
        nKeys->class->push(nKeys, keys->class->get(keys, index));
        nVals->class->push(nVals, vals->class->get(vals, index));
    }

    return newMap;
}

extern Map *createMap() {
    Map *newMap = (Map *) malloc(sizeof(Map));

    Private *private = (Private *) malloc(sizeof(Private));
    private->keys    = CreateList();
    private->values  = CreateList();
    private->mapSize = 0;

    newMap->_private = private;
    newMap->class    = &ClassMap;
    newMap->_class   = &classMap;

    return newMap;
}

static void __map_delete(void *this) {
    Private *private = (Private *) ((Map *) this)->_private;

    delete(private->keys);
    delete(private->values);
    free(private);
    free(this);
}

extern ClassMapType ClassMap = {
    .remove      = &__map_remove,
    .set         = &__map_set,
    .get         = &__map_get,
    .length      = &__map_length,
    .toString    = &__map_toString,
    .copy        = &__map_copy
};

extern Class classMap = {
    .classType = CLASS_MAP,
    .delete    = &__map_delete
};