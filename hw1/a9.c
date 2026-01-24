#include <stdio.h>
#include <stdint.h>

struct pack_array {
    uint32_t array;   
    uint32_t count0;  
    uint32_t count1;  
};

void array2struct(int a[], struct pack_array *p) {
    uint32_t value = 0;
    uint32_t c0 = 0, c1 = 0;

    for (int i = 0; i < 32; ++i) {
        value <<= 1;         

        if (a[i] == 1) {
            value |= 1u;      
            ++c1;
        } else {             
            ++c0;
        }
    }

    p->array  = value;
    p->count0 = c0;
    p->count1 = c1;
}

int main(void) {
    int a[32];
    for (int i = 0; i < 32; ++i) {
        if (scanf("%d", &a[i]) != 1)
            return 0;         
    }

    struct pack_array p;
    array2struct(a, &p);

    printf("%u %u %u\n", p.array, p.count0, p.count1);
    return 0;
}
