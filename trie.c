
#include "trie.h"
#include "node.h"

#include <stdlib.h>
#include <string.h>

static const struct node* trie_node_lookup (const struct trie*, const struct node*, const char*);
static void trie_node_insert (struct trie*, struct node*, const char*);
static struct node* trie_node_compress (struct trie*, struct node*, struct node**, int* a_used);

struct trie* trie_alloc ()
{
  return (struct trie*)malloc(sizeof(struct trie));
}

void trie_dealloc (struct trie* a_trie)
{
  free(a_trie);
}

struct trie* trie_init (struct trie* a_trie)
{
  a_trie->root_ = node_init(node_alloc(), '\0');
  a_trie->node_count_ = 0;

  return a_trie;
}

void trie_insert (struct trie* a_trie, const char* a_text)
{
  trie_node_insert(a_trie, a_trie->root_, a_text);
}

static void trie_node_insert (struct trie* a_trie, struct node* a_node, const char* a_text)
{
  if (a_node->value == *a_text) {
    if (*++a_text) {
      if (a_node->right == NULL) {
        ++(a_trie->node_count_);
        a_node->right = node_init(node_alloc(), *a_text);
      }
      trie_node_insert(a_trie, a_node->right, a_text);
    } else {
      a_node->yes = 1;
    }
  } else {
    if (a_node->left == NULL) {
        ++(a_trie->node_count_);
      a_node->left = node_init(node_alloc(), *a_text);
    }

    trie_node_insert(a_trie, a_node->left, a_text);
  }
}

int trie_lookup (struct trie* a_trie, const char* a_text)
{
  return trie_node_lookup(a_trie, a_trie->root_, a_text) ? 1 : 0;
}

static const struct node* trie_node_lookup (const struct trie* a_trie, const struct node* a_node, const char* a_text)
{
  if (a_node == NULL) {
    return NULL;
  }

  if (a_node->value == *a_text) {
    if (*++a_text) {
      return trie_node_lookup(a_trie, a_node->right, a_text);
    } else {
      if (a_node->yes) {
        return a_node;
      } else {
        return NULL;
      }
    }
  } else {
    return trie_node_lookup(a_trie, a_node->left, a_text);
  }
}

void trie_compress (struct trie* a_trie)
{
  int used = 0;
  struct node** nodes = (struct node**)malloc(sizeof(struct node*) * a_trie->node_count_);

  memset(nodes, 0, sizeof(struct node*) * a_trie->node_count_);

  trie_node_compress(a_trie, a_trie->root_, nodes, &used);

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
    
    cmp = memcmp(a_node, a_nodes[p], sizeof(*a_node));

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

  if (*p && memcmp(*p, a_node, sizeof(*a_node)) == 0) {
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
    --(a_trie->node_count_);
    node_dealloc(a_node);
    return *ptr;
  } else {
    trie_equivalent_insert(a_nodes, a_node, a_used);
    return a_node;
  }
}

