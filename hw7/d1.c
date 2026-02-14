#include <limits.h>



typedef struct {
    int key;
    int depth;
    int used;
} Cell;

void dfs(tree *node, int hd, int depth,
         Cell *arr, int *minHD, int *maxHD) {
    if (!node) return;

    int idx = hd + 1000;          

    if (!arr[idx].used || depth < arr[idx].depth) {
        arr[idx].used  = 1;
        arr[idx].key   = node->key;
        arr[idx].depth = depth;
    }

    if (hd < *minHD) *minHD = hd;
    if (hd > *maxHD) *maxHD = hd;

    dfs(node->left,  hd - 1, depth + 1, arr, minHD, maxHD);
    dfs(node->right, hd + 1, depth + 1, arr, minHD, maxHD);
}

void btUpView(tree *root) {
    if (!root) return;

    Cell arr[2001];
    for (int i = 0; i < 2001; ++i)
        arr[i].used = 0;

    int minHD = 0, maxHD = 0;
    dfs(root, 0, 0, arr, &minHD, &maxHD);

    for (int hd = minHD; hd <= maxHD; ++hd) {
        int idx = hd + 1000;
        if (arr[idx].used) {
            printf("%d", arr[idx].key);
            if (hd < maxHD) printf(" ");
        }
    }
}
