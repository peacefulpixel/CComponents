#ifndef __LIST_H__
#define __LIST_H__

typedef struct _sys_unsafe_list {
    /* 
     * Private value, do not even try!)
     */
    void *_value;
    /* 
     * Private size, do not even try too!)
     */
    int  *_size;
    /*
     * The size of the list item. Required for memory allocation (preferably not to change)
     */
    int  elementSize;
    /*
     * Adds an item to the list.
     */
    void (*push)(struct _sys_unsafe_list *this, void *value);
    /*
     * Removes an item from the list with offset
     */
    void (*remove)(struct _sys_unsafe_list *this, int index);
    /*
     * Replaces list item by index
     */
    void (*set)(struct _sys_unsafe_list *this, int index, void *value);
    /*
     * Returns a list item by index
     */
    void *(*get)(struct _sys_unsafe_list *this, int index);
    /*
     * Returns the length of the array
     */
    int  (*length)(struct _sys_unsafe_list *this);
} List;

List *newList(int elementSize);
void deleteList(List *this);

#endif /* __LIST_H__ */