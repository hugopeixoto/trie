
#include "trie.h"
#include "node.h"

#include <stdlib.h>
#include <string.h>

static const struct node* trie_node_lookup (const struct trie*, const struct node*, const char*);
static void trie_node_insert (struct trie*, struct node*, const char*);
static int trie_node_validate (const struct node* a_node);
static void trie_node_reset_order (struct node* a_node, int* a_current);

struct trie* trie_alloc ()
{
  return (struct trie*)malloc(sizeof(struct trie));
}

void trie_dealloc (struct trie* a_trie)
{
  if (a_trie->root_ != NULL) {
    /* TODO(hpeixoto): dealloc all the nodes */
    node_dealloc(a_trie->root_);
  }

  free(a_trie);
}

struct trie* trie_init (struct trie* a_trie)
{
  a_trie->root_ = node_init(node_alloc(), '\0');
  a_trie->node_count_ = 0;

  return a_trie;
}

#include <stdio.h>
void trie_insert (struct trie* a_trie, const char* a_text)
{
  trie_node_insert(a_trie, a_trie->root_, a_text);
}

static void trie_node_insert (struct trie* a_trie, struct node* a_node, const char* a_text)
{
  /* The left nodes should be lexically ordered,
   * so that it yields the same trie regardlessly
   * of the word insertion order.
   */
  if ((unsigned char)*a_text == a_node->value) {
    if (*++a_text) {
      if (a_node->right == NULL) {
        ++(a_trie->node_count_);
        a_node->right = node_init(node_alloc(), (unsigned char)*a_text);
      }
      trie_node_insert(a_trie, a_node->right, a_text);
    } else {
      a_node->yes = 1;
    }
  } else if ((unsigned char)*a_text < a_node->value) {
    struct node* n = NULL;

    ++(a_trie->node_count_);
    n = node_init(node_alloc(), a_node->value);
    n->right = a_node->right;
    n->left = a_node->left;
    n->visited = a_node->visited;
    n->yes = a_node->yes;

    a_node->right = NULL;
    a_node->left  = n;
    a_node->value = (unsigned char)*a_text;
    a_node->yes   = 0;

    trie_node_insert(a_trie, a_node, a_text);

  } else {
    if (a_node->left == NULL) {
      ++(a_trie->node_count_);
      a_node->left = node_init(node_alloc(), (unsigned char)*a_text);
    }

    trie_node_insert(a_trie, a_node->left, a_text);
  }
}

int trie_lookup (struct trie* a_trie, const char* a_text)
{
  return !!trie_node_lookup(a_trie, a_trie->root_, a_text);
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

int trie_validate (const struct trie* a_trie)
{
  return trie_node_validate(a_trie->root_);
}

static int trie_node_validate (const struct node* a_node)
{
  if (a_node->left) {
    if (a_node->left->value < a_node->value || trie_node_validate(a_node->left)) {
      return 1;
    }
  }

  if (a_node->right) {
    return trie_node_validate(a_node->right);
  }

  return 0;
}

void trie_reset_order (struct trie* a_trie)
{
  int current = 0;
  trie_node_reset_order(a_trie->root_, &current);
}

static void trie_node_reset_order (struct node* a_node, int* a_current)
{
  a_node->visited = (*a_current)++;

  if (a_node->left && !a_node->left->visited) {
    trie_node_reset_order(a_node->left, a_current);
  }

  if (a_node->right && !a_node->right->visited) {
    trie_node_reset_order(a_node->right, a_current);
  }
}
