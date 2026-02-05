#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    char n[105];  
    if (scanf("%s", n) != 1) {
        printf("0\n");
        return 0;
    }
    
    int len = strlen(n);
    
    bool *found = (bool*)calloc(1000, sizeof(bool));
    if (!found) {
        printf("0\n");
        return 0;
    }
    
    int count = 0;
    
    for (int i = 0; i < len - 2; i++) {
        if (n[i] == '0') continue; 
        
        for (int j = i + 1; j < len - 1; j++) {
            for (int k = j + 1; k < len; k++) {
                int num = (n[i] - '0') * 100 + (n[j] - '0') * 10 + (n[k] - '0');
                
                if (!found[num]) {
                    found[num] = true;
                    count++;
                }
            }
        }
    }
    
    free(found);
    printf("%d\n", count);
    return 0;
}
