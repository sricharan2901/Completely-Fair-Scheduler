#include <stdio.h>
#include <stdlib.h>

enum Color
{
    RED,
    BLACK
};

typedef struct RBTreeNode
{
    float key;
    int process_id;
    struct RBTreeNode *left, *right, *parent;
    enum Color color;
} RBTreeNode;

typedef struct RBTree
{
    RBTreeNode *root;
    RBTreeNode *nil;
} RBTree;

RBTreeNode *newRBTreeNode(float key, int pid)
{
    RBTreeNode *node = (RBTreeNode *)malloc(sizeof(RBTreeNode));
    if (node != NULL)
    {
        node->key = key;
        node->process_id = pid;
        node->left = node->right = node->parent = NULL;
        node->color = RED; // New node is initially red
    }
    return node;
}

RBTree *newRBTree()
{
    RBTree *tree = (RBTree *)malloc(sizeof(RBTree));
    if (tree != NULL)
    {
        tree->nil = newRBTreeNode(0, 0); // Create a sentinel node
        tree->nil->color = BLACK;        // Sentinel node is always black
        tree->root = tree->nil;
    }
    return tree;
}

void leftRotate(RBTree *T, RBTreeNode *x)
{
    RBTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != T->nil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == T->nil)
        T->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rightRotate(RBTree *T, RBTreeNode *y)
{
    RBTreeNode *x = y->left;
    y->left = x->right;
    if (x->right != T->nil)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == T->nil)
        T->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    x->right = y;
    y->parent = x;
}

void RBInsertFixup(RBTree *T, RBTreeNode *z)
{
    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            RBTreeNode *y = z->parent->parent->right;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    leftRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(T, z->parent->parent);
            }
        }
        else
        {
            RBTreeNode *y = z->parent->parent->left;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rightRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

void RBInsert(RBTree *T, float key, int pid)
{
    RBTreeNode *z = newRBTreeNode(key, pid);
    RBTreeNode *y = T->nil;
    RBTreeNode *x = T->root;
    while (x != T->nil)
    {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == T->nil)
        T->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
    z->left = z->right = T->nil;
    z->color = RED;
    RBInsertFixup(T, z);
}

void transplant(RBTree *T, RBTreeNode *u, RBTreeNode *v)
{
    if (u->parent == T->nil)
        T->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

RBTreeNode *treeMinimum(RBTreeNode *node, RBTreeNode *nil)
{
    while (node->left != nil)
        node = node->left;
    return node;
}

void RBDeleteFixup(RBTree *T, RBTreeNode *x)
{
    while (x != T->root && x->color == BLACK)
    {
        if (x == x->parent->left)
        {
            RBTreeNode *w = x->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(T, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(T, x->parent);
                x = T->root;
            }
        }
        else
        {
            RBTreeNode *w = x->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

void RBDelete(RBTree *T, RBTreeNode *z)
{
    RBTreeNode *y = z, *x;
    enum Color yOriginalColor = y->color;
    if (z->left == T->nil)
    {
        x = z->right;
        transplant(T, z, z->right);
    }
    else if (z->right == T->nil)
    {
        x = z->left;
        transplant(T, z, z->left);
    }
    else
    {
        y = treeMinimum(z->right, T->nil);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else
        {
            transplant(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (yOriginalColor == BLACK)
        RBDeleteFixup(T, x);
    free(z);
}

RBTreeNode *popSmallest(RBTree *T)
{
    if (T->root == T->nil)
    {
        printf("Tree is empty\n");
        return NULL;
    }
    RBTreeNode *temp;
    RBTreeNode *smallest = treeMinimum(T->root, T->nil);
    temp->key = smallest->key;
    temp->process_id = smallest->process_id;
    RBDelete(T, smallest);
    return temp;
}

void inorderTraversal(RBTreeNode *node, RBTreeNode *nil)
{
    if (node != nil)
    {
        inorderTraversal(node->left, nil);
        printf("process %d :%f ", node->process_id, node->key);
        inorderTraversal(node->right, nil);
    }
}
