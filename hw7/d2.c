

tree *findBrother(tree *root, int key) {
    if (root == NULL) return NULL;

    tree *cur = root;
    while (cur != NULL && cur->key != key) {
        if (key < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }

    if (cur == NULL)         
        return NULL;

    if (cur->parent == NULL) 
        return NULL;

    if (cur->parent->left == cur)
        return cur->parent->right;
    else
        return cur->parent->left;
}
