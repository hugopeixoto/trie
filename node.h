
#ifndef _NODE_H_
#define _NODE_H_

struct node {
  struct node* left;
  struct node* right;
  char value;
  char yes;
};

struct node* node_alloc ();
void node_dealloc (struct node*);
struct node* node_init (struct node*, char a_value);
int node_is_leaf (const struct node*);

#endif

