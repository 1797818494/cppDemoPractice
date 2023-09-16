#include <stdio.h>
#include <stdlib.h>
#include "AVL.h"

int main(int argc, char *argv[]) {
  int a[] = {1, 2, 3, 4, 5, 10, 9, 8, 7, 6};
  int i;

  AVLTree root = NULL;

  for (i = 0; i < sizeof(a) / sizeof(int); i++) {
    if (insert_node(&root, a[i]) != 0) {
      printf("insert node failure\n");
      return -1;
    }
  }

  inorder_traverse(root);
  printf("\n");

  remove_node(&root, 3);
  remove_node(&root, 5);
  inorder_traverse(root);
  printf("\n");

  insert_node(&root, 100);
  insert_node(&root, -100);
  insert_node(&root, -1);
  inorder_traverse(root);
  printf("\n");

  destroy(&root);

  inorder_traverse(root);
  printf("\n");
  return 0x0;
}