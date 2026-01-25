#include <stdint.h>
#include <stddef.h>



uint64_t findMaxBlock(list *head)
{
    if (head == NULL) {
        return 0;              
    }

    size_t   max_size = 0;
    uint64_t max_addr = 0;

    for (list *p = head; p != NULL; p = p->next) {
        if (p->size > max_size) {
            max_size = p->size;
            max_addr = p->address;
        }
    }

    return max_addr;
}
