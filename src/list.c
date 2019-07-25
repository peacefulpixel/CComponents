#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef struct _list_value_private {
    void **listValue;
} ListValuePrivate;

typedef struct _list_size_private {
    int listSize;
} ListSizePrivate;

void _push(struct _sys_unsafe_list *this, void *value) {
    ListSizePrivate  *listSize  = (ListSizePrivate  *) this->_size;
    ListValuePrivate *thisValue = (ListValuePrivate *) this->_value;

    ListValuePrivate *oldValue  = malloc(sizeof(ListValuePrivate));
    
    int arraySize = this->elementSize * listSize->listSize;
    oldValue->listValue = malloc(arraySize);
    memcpy(oldValue->listValue, thisValue->listValue, arraySize);
    
    thisValue->listValue = malloc(arraySize + this->elementSize);
    memcpy(thisValue->listValue, oldValue->listValue, arraySize);

    memcpy(&thisValue->listValue[listSize->listSize], &value, this->elementSize);
    listSize->listSize++;

    free(oldValue->listValue);
    free(oldValue);
}

void _remove(struct _sys_unsafe_list *this, int index) {
    ListSizePrivate  *listSize  = (ListSizePrivate  *) this->_size;
    ListValuePrivate *thisValue = (ListValuePrivate *) this->_value;

    ListValuePrivate *oldValue  = malloc(sizeof(ListValuePrivate));
    oldValue->listValue = thisValue->listValue;

    int arraySize = this->elementSize * (listSize->listSize - 1);
    thisValue->listValue = malloc(arraySize);
    
    memcpy(thisValue->listValue, oldValue->listValue, index * this->elementSize);
    memcpy(thisValue->listValue + index,
            oldValue->listValue + (index + 1), 
            arraySize + this->elementSize - (index + 1) * this->elementSize);

    listSize->listSize--;

    free(oldValue->listValue);
    free(oldValue);
}

void _set(struct _sys_unsafe_list *this, int index, void *value) {
    ListValuePrivate *thisValue = (ListValuePrivate *) this->_value;

    memcpy(&thisValue->listValue[index], &value, this->elementSize);
}

void *_get(struct _sys_unsafe_list *this, int index) {
    ListValuePrivate *thisValue = (ListValuePrivate *) this->_value;

    return thisValue->listValue[index];
}

int _listLength(struct _sys_unsafe_list *this) {
    return ((ListSizePrivate  *) this->_size)->listSize;
}

List *newList(int elementSize) {
    List *new             = (List *) malloc(sizeof(List));
    ListSizePrivate *listSize = (ListSizePrivate *) malloc(sizeof(ListSizePrivate));
    listSize->listSize   = 0;
    new->_value      = malloc(sizeof(ListValuePrivate));
    new->_size       = (void *) listSize;
    new->elementSize = elementSize;
    new->push        = &_push;
    new->remove      = &_remove;
    new->set         = &_set;
    new->get         = &_get;
    new->length      = &_listLength;

    return new;
}

void deleteList(List *this) {
    ListSizePrivate  *listSize  = (ListSizePrivate  *) this->_size;
    ListValuePrivate *listValue = (ListValuePrivate *) this->_value;

    free(listSize);
    free(listValue->listValue);
    free(listValue);
    free(this);
}