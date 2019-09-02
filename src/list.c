#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ccomponents.h"

#define P_SIZE sizeof(intptr_t)

typedef struct _list_private {
    void **listValue;
    unsigned long int listSize;
} Private;

static void __list_push(ArrayList *this, void *value) {
    Private *private = (Private *) this->_private;
    Private *oldPrivate  = (Private *) malloc(sizeof(Private));
    
    unsigned long int arraySize = P_SIZE * private->listSize;
    oldPrivate->listValue = (void **) malloc((size_t) arraySize);
    memcpy(oldPrivate->listValue, private->listValue, (size_t) arraySize);
    
    private->listValue = (void **) malloc((size_t) (arraySize + P_SIZE));
    memcpy(private->listValue, oldPrivate->listValue, (size_t) arraySize);

    memcpy(&private->listValue[private->listSize], &value, (size_t) P_SIZE);
    private->listSize++;

    free(oldPrivate->listValue);
    free(oldPrivate);
}

static void __list_remove(ArrayList *this, unsigned long int index) {
    Private *private = (Private *) this->_private;

    Private *oldPrivate  = (Private *) malloc(sizeof(Private));
    oldPrivate->listValue = private->listValue;

    unsigned long int arraySize = P_SIZE * (private->listSize - 1);
    private->listValue = (void **) malloc((size_t) arraySize);
    
    memcpy(private->listValue, oldPrivate->listValue, (size_t) (index * P_SIZE));
    memcpy(private->listValue + index,
            oldPrivate->listValue + (index + 1), 
            (size_t) (arraySize + P_SIZE - (index + 1) * P_SIZE));

    private->listSize--;

    free(oldPrivate->listValue);
    free(oldPrivate);
}

static void __list_set(ArrayList *this, unsigned long int index, void *value) {
    Private *private = (Private *) this->_private;

    memcpy(&private->listValue[index], &value, (size_t) P_SIZE);
}

static void *__list_get(ArrayList *this, unsigned long int index) {
    Private *private = (Private *) this->_private;

    return private->listValue[index];
}

static unsigned long int __list_length(ArrayList *this) {
    return ((Private *) this->_private)->listSize;
}

static String *__list_toString(ArrayList *this) {
    unsigned long int listLength = __list_length(this);
    String *result = CreateString("ArrayList: [ ");

    for (unsigned long int index = 0; index < listLength; index++) {
        result->class->addULong(result, (unsigned long int) this->class->get(this, index));
        if (index != (listLength - 1)) {
            result->class->add(result, ", ");
        }
    }

    result->class->add(result, " ] (");
    result->class->addULong(result, (unsigned long int) this->class->length(this));
    result->class->add(result, ");");
    return result;
}

static void __list_include(ArrayList *this, void **array, unsigned long int count) {
    for (int index = 0; index < count; index++) {
        this->class->push(this, *(array + index));
    }
}

static ArrayList *__list_copy(ArrayList *this) {
    Private *private = (Private *) this->_private;

    ArrayList *newArrayList = CreateArrayList();
    newArrayList->class->include(newArrayList, private->listValue, private->listSize);

    return newArrayList;
}

extern ArrayList *createArrayList() {
    ArrayList *newArrayList = (ArrayList *) malloc(sizeof(ArrayList));
    Private *private = (Private *) malloc(sizeof(Private));
    private->listSize = 0;
    newArrayList->_private = private;
    newArrayList->class = &ClassArrayList;
    newArrayList->_class = &classArrayList;

    return newArrayList;
}

static void __list_delete(void *this) {
    Private *private = (Private *) ((ArrayList *) this)->_private;

    free(private->listValue);
    free(private);
    free(this);
}

extern ClassArrayListType ClassArrayList = {
    .push     = &__list_push,
    .remove   = &__list_remove,
    .set      = &__list_set,
    .get      = &__list_get,
    .length   = &__list_length,
    .toString = &__list_toString,
    .include  = &__list_include,
    .copy     = &__list_copy
};

extern Class classArrayList = {
    .classType = CLASS_ARRAY_LIST,
    .delete    = &__list_delete
};