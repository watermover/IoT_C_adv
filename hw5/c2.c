#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK 1000

int stack[MAX_STACK];
int top = -1;

void push(int val) {
    stack[++top] = val;
}

int pop() {
    return stack[top--];
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int main() {
    char line[10005];
    if (!fgets(line, sizeof(line), stdin)) {
        return 0;
    }
    
    char *token = strtok(line, " \n");
    
    while (token != NULL) {
        int len = strlen(token);
        if (len > 0 && token[len - 1] == '.') {
            token[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            token = strtok(NULL, " \n");
            continue;
        }
        
        if (is_operator(token[0]) && len == 1) {
            int b = pop();
            int a = pop();
            
            switch(token[0]) {
                case '+':
                    push(a + b);
                    break;
                case '-':
                    push(a - b);
                    break;
                case '*':
                    push(a * b);
                    break;
                case '/':
                    push(a / b);
                    break;
            }
        } else {
            push(atoi(token));
        }
        
        token = strtok(NULL, " \n");
    }
    
    printf("%d\n", pop());
    
    return 0;
}
