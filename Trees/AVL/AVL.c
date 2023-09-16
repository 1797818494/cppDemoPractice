#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX(x1, x2) ((x1) > (x2)) ? (x1) : (x2)

static int getheight(AVLTree node);

static int findmin(AVLTree node);

static void left_rotate(AVLTree *node);
static void right_rotate(AVLTree *node);

static int getheight(AVLNode *node) {
  if (node == NULL) {
    return 0;
  }
  return node->height;
}

static void left_rotate(AVLTree *node) {
  AVLNode *tmp_right = (*node)->right;
  (*node)->right = tmp_right->left;
  tmp_right->left = *node;
  *node = tmp_right;
}
static void right_rotate(AVLTree *node) {
  AVLNode *tmp_left = (*node)->left;
  (*node)->left = tmp_left->right;
  tmp_left->right = *node;
  *node = tmp_left;
}

static int findmin(AVLTree node) {
  if (node == NULL) {
    return -1;
  }
  while (node->left) {
    node = node->left;
  }
  return node->data;
}
int insert_node(AVLTree *root, int data) {
  if (!*root) {
    *root = (AVLNode *)malloc(sizeof(AVLNode));
    if (!*root) {
      return -1;
    }
    (*root)->data = data;
    (*root)->height = 1;
    (*root)->left = (*root)->right = NULL;
    return 0;
  }
  if ((*root)->data == data) {
    return -1;
  } else if ((*root)->data < data) {
    if (insert_node(&(*root)->right, data) < 0) {
      return -1;
    }
    int lheight = getheight((*root)->left);
    int rheight = getheight((*root)->right);
    if (rheight - lheight == 2) {
      if ((*root)->right->data < data) {
        left_rotate(root);
      } else {
        right_rotate(&(*root)->right);
        left_rotate(root);
      }
    }
  } else {
    if (insert_node(&(*root)->left, data) < 0) return -1;
    int lheight = getheight((*root)->left);
    int rhegiht = getheight((*root)->right);

    if (lheight - rhegiht == 2) {
      if ((*root)->left->data > data) {
        right_rotate(root);
      } else {
        left_rotate(&(*root)->right);
        right_rotate(root);
      }
    }
  }
  (*root)->height = MAX(getheight((*root)->left), getheight((*root)->right));
  return 0;
}

int remove_node(AVLTree *root, int data) {
  if (*root == NULL) {
    return -1;
  }
  if ((*root)->data == data) {
    if ((*root)->left != NULL && (*root)->right != NULL) {
      int key = findmin((*root)->right);
      (*root)->data = key;
      remove_node(&(*root)->right, key);
      int lheight = getheight((*root)->left);
      int rheight = getheight((*root)->right);
      if (lheight - rheight == 2) {
        if (getheight((*root)->left->left) >= getheight((*root)->left->right)) {
          right_rotate(root);
        } else {
          left_rotate(&(*root)->left);
          right_rotate(root);
        }
      }
    } else if ((*root)->left == NULL) {
      AVLNode *node = *root;
      *root = (*root)->right;
      free(node);
      node = NULL;
      return 0;
    } else {
      AVLNode *node = *root;
      *root = (*root)->left;
      free(node);
      node = NULL;
      return 0;
    }
  } else if ((*root)->data < data) {
    if (remove_node(&(*root)->right, data) < 0) {
      return -1;
    }
    int lheight = getheight((*root)->left);
    int rheight = getheight((*root)->right);
    if (lheight - rheight == 2) {
      if (getheight((*root)->left->left) >= getheight((*root)->left->right)) {
        right_rotate(root);
      } else {
        left_rotate(&(*root)->left);
        right_rotate(root);
      }
    }
  } else {
    if (remove_node(&(*root)->left, data) < 0) {
      return -1;
    }
    int lheight = getheight((*root)->left);
    int rheight = getheight((*root)->right);
    if (rheight - lheight == 2) {
      if (getheight((*root)->right->right) >= getheight((*root)->right->left)) {
        left_rotate(root);
      } else {
        right_rotate(&(*root)->right);
        left_rotate(root);
      }
    }
  }
  (*root)->height = MAX(getheight((*root)->left), getheight((*root)->right)) + 1;
  return 0;
}
static inline void destroy_postorder(AVLTree root) {
  if (!root) {
    return;
  }
  destroy_postorder(root->left);
  destroy_postorder(root->right);
  free(root);
}
int destroy(AVLTree *root) {
  if (!*root) return 0;

  destroy_postorder(*root);
  *root = NULL;
  return 0x0;
}

int find(AVLTree root, int data) {
  if (!root) {
    return -1;
  } else if (root->data == data) {
    return 1;
  } else if (root->data < data) {
    return find(root->right, data);
  } else {
    return find(root->left, data);
  }
}

void inorder_traverse(AVLTree root) {
  if (!root) return;
  inorder_traverse(root->left);
  printf("%d ", root->data);
  inorder_traverse(root->right);
}

void preorder_traverse(AVLTree root) {
  if (!root) return;

  printf("%d ", root->data);
  preorder_traverse(root->left);
  preorder_traverse(root->right);
}

void postorder_traverse(AVLTree root) {
  if (!root) return;

  postorder_traverse(root->left);
  postorder_traverse(root->right);
  printf("%d ", root->data);
}
