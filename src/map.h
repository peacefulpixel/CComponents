#ifndef __MAP_H__
#define __MAP_H__

typedef struct _sys_unsafe_map {
    /* 
     * Private value, do not even try!)
     */
    void *_value;
    /* 
     * Private size, do not even try too!)
     */
    int  *_size;
    /*
     * The size of the map item. Required for memory allocation (preferably not to change)
     */
    int  elementSize;
    /*
     * Removes an item from the map
     */
    void (*remove)(struct _sys_unsafe_map *this, char *key);
    /*
     * Replaces map item
     */
    void (*set)(struct _sys_unsafe_map *this, char *key, void *value);
    /*
     * Returns a map item
     */
    void *(*get)(struct _sys_unsafe_map *this, char *_value);
    /*
     * Returns the length of the map
     */
    int  (*length)(struct _sys_unsafe_map *this);
} Map;

Map *newMap(int elementSize);
void deleteMap(Map *this);

#endif /* __MAP_H__ */