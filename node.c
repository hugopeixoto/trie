
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

int node_cmp (const struct node* a_a, const struct node* a_b)
{
  if (a_a->left != a_b->left) { return a_a->left - a_b->left; }
  if (a_a->right != a_b->right) { return a_a->right - a_b->right; }
  if (a_a->visited != a_b->visited) { return a_a->visited - a_b->visited; }
  if (a_a->value != a_b->value) { return a_a->value - a_b->value; }
  return a_a->yes - a_b->yes;
}

