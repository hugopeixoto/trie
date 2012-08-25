
#include "node.h"

#include <stdlib.h>

struct node* node_alloc ()
{
  return (struct node*)malloc(sizeof(struct node));
}

void node_dealloc (struct node* a_node)
{
  free(a_node);
}

struct node* node_init (struct node* a_node, unsigned char a_char)
{
  a_node->left = NULL;
  a_node->right = NULL;
  a_node->visited = 0;
  a_node->value = a_char;
  a_node->yes = 0;

  return a_node;
}

int node_is_leaf (const struct node* a_node)
{
  return a_node->left == NULL && a_node->right == NULL;
}

