#ifndef AVL_H
#define AVL_H

typedef struct node {
  int height;
  int data;
  struct node *left;
  struct node *right;
} AVLNode, *AVLTree;

int insert_node(AVLTree *root, int data);
int remove_node(AVLTree *root, int data);
int find(AVLTree root, int data);
int destroy(AVLTree *root);
int destroy(AVLTree *root);

// just for test

void inorder_traverse(AVLTree root);

void preorder_traverse(AVLTree root);

void postorder_traverse(AVLTree root);

#endif