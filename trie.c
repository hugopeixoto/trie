
#include "trie.h"
#include "node.h"

#include <stdlib.h>
#include <string.h>

static const struct node* trie_node_lookup (const struct trie*, const struct node*, const char*);
static void trie_node_insert (struct trie*, struct node*, const char*);
static struct node* trie_node_compress (struct trie*, struct node*, struct node**);

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
  struct node** nodes = (struct node**)malloc(sizeof(struct node*) * a_trie->node_count_);

  memset(nodes, 0, sizeof(struct node*) * a_trie->node_count_);

  trie_node_compress(a_trie, a_trie->root_, nodes);

  free(nodes);
}

static int trie_equivalent (const struct node* a_a, const struct node* a_b)
{
  return memcmp(a_a, a_b, sizeof(*a_a)) == 0;
}

static struct node** trie_equivalent_lookup (struct node** a_nodes, struct node* a_node)
{
  while (*a_nodes && !trie_equivalent(a_node, *a_nodes)) ++a_nodes;

  return a_nodes;
}

static struct node* trie_node_compress (struct trie* a_trie, struct node* a_node, struct node** a_nodes)
{
  struct node** ptr = NULL;

  if (a_node->left) {
    a_node->left  = trie_node_compress(a_trie, a_node->left, a_nodes);
  }

  if (a_node->right) {
    a_node->right = trie_node_compress(a_trie, a_node->right, a_nodes);
  }

  ptr = trie_equivalent_lookup(a_nodes, a_node);
    
  if (*ptr) {
    --(a_trie->node_count_);
    node_dealloc(a_node);
    return *ptr;
  } else {
    return *ptr = a_node;
  }
}

