#ifndef __FOREACH_H__
#define __FOREACH_H__

#define List_forEach(__LIST__, __ITEM__, __CODE__)                                      \
        for (                                                                            \
            unsigned long int __counter_##__ITEM__ = 0;                                   \
            __counter_##__ITEM__ < __LIST__->class->_impl_List.length(__LIST__);           \
            __counter_##__ITEM__++)                                                         \
        {                                                                                    \
            void *__ITEM__ = __LIST__->class->_impl_List.get(__LIST__, __counter_##__ITEM__); \
            __CODE__                                                                           \
        }

#endif /* __FOREACH_H__ */