#include <stdlib.h>
#include <stdbool.h>

#include "ccomponents.h"

#define this ((LinkedList *) _this)

typedef struct _list_private {
    struct entry {
        void *value;
        struct entry *previous;
        struct entry *next;
    } *firstEntry, *lastEntry;
    unsigned long int listSize;
} Private;

static inline struct entry *getEntryForwards
        (struct entry *first, unsigned long count) {

    struct entry *result = first;
    for (; count; count--)
        result = result->next;

    return result;

}

static inline struct entry *getEntryBackwards
        (struct entry *last, unsigned long count) {

    struct entry *result = last;
    for (; count; count--)
        result = result->previous;

    return result;

}

static void removeEntry(Private *private, struct entry *entry) {

    if (!entry->previous) {
        private->firstEntry = entry->next;
        if (private->firstEntry)
            private->firstEntry->previous = NULL;
    } else
        entry->previous->next = entry->next;

    if (!entry->next) {
        private->lastEntry = entry->previous;
        if (private->lastEntry)
            private->lastEntry->next = NULL;
    } else
        entry->next->previous = entry->previous;

    free(entry);
    private->listSize--;

}

/** May return NULL */
static struct entry *getEntry(Private *private, unsigned long index) {

    unsigned long maxIndex = private->listSize - 1;
    bool forwards = maxIndex - index > maxIndex >> 1;

    return forwards ?
        getEntryForwards(private->firstEntry, index) :
        getEntryBackwards(private->lastEntry, maxIndex - index);

}

extern void __CComp_LinkedList_implList_add(void *_this, void *value) {

    Private *private = (Private *) this->_private;
    struct entry *newLastEntry = malloc(sizeof(struct entry));

    if (!private->listSize) {
        private->firstEntry = private->lastEntry = newLastEntry;
    } else
        private->lastEntry->next = newLastEntry;

    newLastEntry->next = NULL;
    newLastEntry->previous = private->listSize ? private->lastEntry : NULL;
    newLastEntry->value = value;

    private->lastEntry = newLastEntry;
    private->listSize++;

}

extern void __CComp_LinkedList_implList_remove
            (void *_this, unsigned long index) {

    Private *private = (Private *) this->_private;
    struct entry *entry = getEntry(private, index);

    removeEntry(private, entry);

}

extern void __CComp_LinkedList_implList_set
            (void *_this, unsigned long index, void *value) {

    getEntry((Private *) this->_private, index)->value = value;
}

extern void *__CComp_LinkedList_implList_get
            (void *_this, unsigned long index) {

    return getEntry((Private *) this->_private, index)->value;
}

extern unsigned long __CComp_LinkedList_implList_length(void *_this) {

    return ((Private *) this->_private)->listSize;
}

String * __CComp_LinkedList_implObject_toString(void *_this) {

    Private *private = (Private *) this->_private;

    String *result = CreateString("LinkedList: [ ");
    struct entry *cursor = private->firstEntry;
    while (cursor) {
        
        result->class->addULong(result, (unsigned long) cursor->value);
        if (cursor != private->lastEntry)
            result->class->add(result, ", ");

        cursor = cursor->next;
        
    }

    result->class->add(result, " ] (");
    result->class->addULong(result, private->listSize);
    result->class->add(result, ");");

    return result;

}

extern void *__CComp_LinkedList_implObject_copy(void *_this) {

    LinkedList *result = createLinkedList();

    Private *privateSrc = (Private *) this->_private;
    Private *privateDst = (Private *) result->_private;

    *privateDst = *privateSrc;
    struct entry *entry = malloc(sizeof(struct entry));
    *entry = *privateSrc->firstEntry;

    for (; entry->next; entry = entry->next) {
        entry->next->previous = entry;
    }

    return result;

}

extern void __CComp_LinkedList_addFirst(void *_this, void *value) {

    Private *private = (Private *) this->_private;

    struct entry *entry = malloc(sizeof(struct entry));
    entry->previous = NULL;
    entry->next = private->firstEntry;
    entry->value = value;

    if (private->firstEntry)
        private->firstEntry->previous = entry;
    
    private->firstEntry = entry;
    private->listSize++;

}

extern void *__CComp_LinkedList_getFirst(void *_this) {

    struct entry *entry = ((Private *) this->_private)->firstEntry;
    return entry ? entry->value : NULL;
}

extern void *__CComp_LinkedList_getLast(void *_this) {

    struct entry *entry = ((Private *) this->_private)->lastEntry;
    return entry ? entry->value : NULL;
}

extern void __CComp_LinkedList_removeFirst(void *_this) {

    struct entry *entry = ((Private *) this->_private)->firstEntry;
    if (entry)
        removeEntry((Private *) this->_private, entry);
}

extern void __CComp_LinkedList_removeLast(void *_this) {

    struct entry *entry = ((Private *) this->_private)->lastEntry;
    if (entry)
        removeEntry((Private *) this->_private, entry);
}

extern LinkedList *createLinkedList() {

    LinkedList *newLinkedList = (LinkedList *) malloc(sizeof(LinkedList));
    Private *private = (Private *) malloc(sizeof(Private));
    private->listSize = 0;
    private->firstEntry = private->lastEntry = NULL;
    newLinkedList->_private = private;
    newLinkedList->class = &ClassLinkedList;
    newLinkedList->_class = &classLinkedList;

    return newLinkedList;

}

extern void __CComp_Cls_LinkedList_delete(void *_this) {

    Private *private = (Private *) this->_private;

    struct entry *cursor = private->firstEntry;
    while (cursor) {
        struct entry *next = cursor->next;
        free(cursor);
        cursor = next;
    }

    free(private);
    free(this);

}

ClassLinkedListType ClassLinkedList = {
    &__CComp_LinkedList_addFirst,
    &__CComp_LinkedList_getFirst,
    &__CComp_LinkedList_getLast,
    &__CComp_LinkedList_removeFirst,
    &__CComp_LinkedList_removeLast,
    {
        INTERFACE_LIST,
        &__CComp_LinkedList_implList_add,
        &__CComp_LinkedList_implList_remove,
        &__CComp_LinkedList_implList_set,
        &__CComp_LinkedList_implList_get,
        &__CComp_LinkedList_implList_length,
        {
            INTERFACE_CCOBJECT,
            &__CComp_LinkedList_implObject_toString,
            &__CComp_LinkedList_implObject_copy
        }
    }
};

Class classLinkedList = {
    .classType = CLASS_LINKED_LIST,
    .delete    = &__CComp_Cls_LinkedList_delete
};