
#include "suffix_compression.h"
#include "trie.h"
#include "node.h"

#include <string.h>
#include <stdlib.h>

static struct node** trie_equivalent_binary_search (struct node**, struct node*, int*);
static struct node** trie_equivalent_lookup (struct node**, struct node*, int*);
static void trie_equivalent_insert (struct node**, struct node*, int*);
static struct node* trie_node_compress (struct trie*, struct node*, struct node**, int*);

void trie_suffix_compress (struct trie* a_trie)
{
  int used = 0;
  struct node** nodes = (struct node**)malloc(sizeof(struct node*) * a_trie->count);

  memset(nodes, 0, sizeof(struct node*) * a_trie->count);

  trie_node_compress(a_trie, a_trie->root, nodes, &used);

  free(nodes);
}

static struct node** trie_equivalent_binary_search (struct node** a_nodes, struct node* a_node, int* a_used)
{
  int begin = 0;
  int end   = *a_used;
  int p     = 0;
  int cmp   = 0;

  while (begin != end) {
    p = begin + (end - begin) / 2;
    
    cmp = node_cmp(a_node, a_nodes[p]);

    if (cmp == 0) {
      return a_nodes + p;
    }

    if (cmp < 0) {
      end = p;
    } else {
      begin = p + 1;
    }
  }

  return a_nodes + begin;
}

static struct node** trie_equivalent_lookup (struct node** a_nodes, struct node* a_node, int* a_used)
{
  struct node** p = trie_equivalent_binary_search(a_nodes, a_node, a_used);

  if (*p && node_cmp(*p, a_node) == 0) {
    return p;
  }

  return NULL;
}

static void trie_equivalent_insert (struct node** a_nodes, struct node* a_node, int* a_used)
{
  struct node** p = trie_equivalent_binary_search(a_nodes, a_node, a_used);

  memmove(p + 1, p, sizeof(struct node*) * (unsigned int)(a_nodes + *a_used - p));
  *p = a_node;
  ++*a_used;
}

static struct node* trie_node_compress (struct trie* a_trie, struct node* a_node, struct node** a_nodes, int* a_used)
{
  struct node** ptr = NULL;

  if (a_node->left) {
    a_node->left  = trie_node_compress(a_trie, a_node->left, a_nodes, a_used);
  }

  if (a_node->right) {
    a_node->right = trie_node_compress(a_trie, a_node->right, a_nodes, a_used);
  }

  ptr = trie_equivalent_lookup(a_nodes, a_node, a_used);
    
  if (ptr) {
    --(a_trie->count);
    node_dealloc(a_node);
    return *ptr;
  } else {
    trie_equivalent_insert(a_nodes, a_node, a_used);
    return a_node;
  }
}

