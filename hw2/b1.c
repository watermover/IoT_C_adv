#include <stddef.h>



size_t totalMemoryUsage(list *head)
{
    size_t total = 0;

    for (list *p = head; p != NULL; p = p->next) {
        total += p->size;
    }

    return total;
}
