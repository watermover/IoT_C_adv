#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list {
    char word[20];
    struct list *next;
};

void add_to_list(struct list **head, const char *w) {
    struct list *node = (struct list *)malloc(sizeof(struct list));
    strcpy(node->word, w);
    node->next = NULL;

    if (*head == NULL) {
        *head = node;
    } else {
        struct list *p = *head;
        while (p->next != NULL)
            p = p->next;
        p->next = node;
    }
}

void swap_elements(struct list *a, struct list *b) {
    char tmp[20];
    strcpy(tmp, a->word);
    strcpy(a->word, b->word);
    strcpy(b->word, tmp);
}

void sort_list(struct list *head) {
    if (!head) return;
    int swapped;
    struct list *p;
    struct list *last = NULL;

    do {
        swapped = 0;
        p = head;
        while (p->next != last) {
            if (strcmp(p->word, p->next->word) > 0) {
                swap_elements(p, p->next);
                swapped = 1;
            }
            p = p->next;
        }
        last = p;
    } while (swapped);
}

void print_list(struct list *head) {
    struct list *p = head;
    while (p) {
        printf("%s", p->word);
        if (p->next)
            printf(" ");
        p = p->next;
    }
    printf("\n");
}

void delete_list(struct list *head) {
    struct list *p;
    while (head) {
        p = head->next;
        free(head);
        head = p;
    }
}

int main(void) {
    char ch;
    char buf[21];
    int i = 0;
    struct list *head = NULL;

    while ((ch = getchar()) != EOF) {
        if (ch == ' ' || ch == '.') {
            if (i > 0) {
                buf[i] = '\0';
                add_to_list(&head, buf);
                i = 0;
            }
            if (ch == '.')
                break;
        } else {
            if (i < 20)   
                buf[i++] = ch;
        }
    }

    sort_list(head);
    print_list(head);
    delete_list(head);

    return 0;
}
