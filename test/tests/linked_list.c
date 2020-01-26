#include <assert.h>

#include "../../src/ccomponents.h"

int main(int argc, char **argv) {
    
    // Testing constructor
    LinkedList *list = CreateLinkedList();
    assert(!ClassLinkedList._impl_List.length(list));

    // Testing push()
    char *testData[4] =
        {
            "Hel", "lo ", "wor", "ld!"
        };

    for (int x = 0; x < 4; x++)
        ClassLinkedList._impl_List.add(list, testData[x]);

    assert(ClassLinkedList._impl_List.length(list) == 4);

    for (int x = 0; x < 4; x++) 
        assert(ClassLinkedList._impl_List.get(list, x) == testData[x]);

    // Testing set() & get()
    ClassLinkedList._impl_List.set(list, 2, testData[0]);

    assert(ClassLinkedList._impl_List.length(list) == 4);
    assert(ClassLinkedList._impl_List.get(list, 2) == ClassLinkedList
        ._impl_List.get(list, 0));

    // Testing remove() & get()
    ClassLinkedList._impl_List.remove(list, 0);

    assert(ClassLinkedList._impl_List.length(list) == 3);
    assert(ClassLinkedList._impl_List.get(list, 0) == testData[1] &&
           ClassLinkedList._impl_List.get(list, 1) == testData[0] &&
           ClassLinkedList._impl_List.get(list, 2) == testData[3] );

    // Testing toString()
    String *listAsString = ClassLinkedList._impl_List
        ._impl_CCObject.toString(list);
    delete(listAsString);

    // Testing copy()
    LinkedList *copy = ClassLinkedList._impl_List._impl_CCObject.copy(list);
    assert(ClassLinkedList._impl_List.length(copy) == 3);

    for (int index = 0; index < 3; index++)
        assert(ClassLinkedList._impl_List.get(copy, index) == 
               ClassLinkedList._impl_List.get(list, index));

    // Testing addFirst
    ClassLinkedList.addFirst(list, testData[2]);
    assert(ClassLinkedList._impl_List.length(list) == 4);
    assert(ClassLinkedList._impl_List.get(list, 0) == testData[2] &&
           ClassLinkedList._impl_List.get(list, 1) == testData[1] &&
           ClassLinkedList._impl_List.get(list, 2) == testData[0] &&
           ClassLinkedList._impl_List.get(list, 3) == testData[3] );

    // Testing getFirst & getLast
    assert(ClassLinkedList.getFirst(list) == testData[2]);
    assert(ClassLinkedList.getLast(list) == testData[3]);

    // Testing removeFirst & removeLast
    ClassLinkedList.removeFirst(list);
    assert(ClassLinkedList.getFirst(list) == testData[1]);
    assert(ClassLinkedList._impl_List.length(list) == 3);

    ClassLinkedList.removeLast(list);
    assert(ClassLinkedList.getLast(list) == testData[0]);
    assert(ClassLinkedList._impl_List.length(list) == 2);

    List_forEach(copy, item, {
        assert(item);
    });

    delete(copy);
    delete(list);

    return 0;
}