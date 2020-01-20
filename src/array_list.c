#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ccomponents.h"

#define P_SIZE sizeof(intptr_t)
#define this ((ArrayList *) _this)

typedef struct _list_private {
    void **listValue;
    unsigned long int listSize;
} Private;

extern void __CComp_ArrayList_implList_push(void *_this, void *value) {
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

extern void __CComp_ArrayList_implList_remove(void *_this, unsigned long int index) {
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

extern void __CComp_ArrayList_implList_set(void *_this, unsigned long int index, void *value) {
    Private *private = (Private *) this->_private;

    memcpy(&private->listValue[index], &value, (size_t) P_SIZE);
}

extern void *__CComp_ArrayList_implList_get(void *_this, unsigned long int index) {
    Private *private = (Private *) this->_private;

    return private->listValue[index];
}

extern unsigned long int __CComp_ArrayList_implList_length(void *_this) {
    return ((Private *) this->_private)->listSize;
}

extern String *__CComp_ArrayList_implObject_toString(void *_this) {
    unsigned long int listLength = __CComp_ArrayList_implList_length(this);
    String *result = CreateString("ArrayList: [ ");

    for (unsigned long int index = 0; index < listLength; index++) {
        result->class->addULong(result, (unsigned long int) this->class->_impl_List.get(this, index));
        if (index != (listLength - 1)) {
            result->class->add(result, ", ");
        }
    }

    result->class->add(result, " ] (");
    result->class->addULong(result, (unsigned long int) this->class->_impl_List.length(this));
    result->class->add(result, ");");
    return result;
}

extern void *__CComp_ArrayList_implObject_copy(void *_this) {
    Private *private = (Private *) this->_private;

    ArrayList *newArrayList = CreateArrayList();
    newArrayList->class->include(newArrayList, private->listValue, private->listSize);

    return newArrayList;
}

extern void __CComp_ArrayList_include(void *_this, void **array, unsigned long int count) {
    for (int index = 0; index < count; index++) {
        this->class->_impl_List.push(this, *(array + index));
    }
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

extern void __CComp_Cls_ArrayList_delete(void *_this) {
    Private *private = (Private *) this->_private;

    free(private->listValue);
    free(private);
    free(this);
}

ClassArrayListType ClassArrayList = {
    &__CComp_ArrayList_include,
    {
        INTERFACE_LIST,
        &__CComp_ArrayList_implList_push,
        &__CComp_ArrayList_implList_remove,
        &__CComp_ArrayList_implList_set,
        &__CComp_ArrayList_implList_get,
        &__CComp_ArrayList_implList_length,
        {
            INTERFACE_CCOBJECT,
            &__CComp_ArrayList_implObject_toString,
            &__CComp_ArrayList_implObject_copy
        }
    }
};

Class classArrayList = {
    .classType = CLASS_ARRAY_LIST,
    .delete    = &__CComp_Cls_ArrayList_delete
};
