
#include "indexed_trie.h"
#include "trie.h"
#include "node.h"

#include <stdlib.h>
#include <string.h>

struct node_info {
  struct node* node;
  unsigned int index;
};

static void indexed_trie_node_states (struct node*, struct node_info*, unsigned int*, int);
static unsigned int indexed_trie_states (const struct trie*, struct node_info*);

struct indexed_trie* indexed_trie_alloc ()
{
  return (struct indexed_trie*)malloc(sizeof(struct indexed_trie));
}

void indexed_trie_dealloc (struct indexed_trie* a_trie)
{
  free(a_trie->nodes);
  free(a_trie);
}

struct indexed_trie* indexed_trie_init (struct indexed_trie* a_indexed_trie, const struct trie* a_trie)
{
  unsigned int i = 0;
  struct node_info* visited =  NULL;

  a_indexed_trie->count = 0;
  a_indexed_trie->nodes = NULL;
  
  visited = (struct node_info*)calloc(a_trie->count, sizeof(struct node_info));

  /**
   * Indices start at one. this means that by the end, count will be
   * equivalent to the number of nodes. Since we want to leave node 0
   * empty, we'll allocate n+1 nodes. This may be unnecessary. Will check.
   */
  a_indexed_trie->count = 1 + indexed_trie_states(a_trie, visited);

  a_indexed_trie->nodes = (struct indexed_trie_node*)calloc(a_indexed_trie->count, sizeof(struct indexed_trie_node));

  for (i = 0; i < a_trie->count; ++i) {
    unsigned int index = visited[i].index;
    struct node* n = visited[i].node;

    while (n) {
      unsigned char c = (unsigned char) n->value;

      a_indexed_trie->nodes[index].yes[c] = n->yes;
      if (n->right) {
        a_indexed_trie->nodes[index].pointers[c] = visited[n->right->visited].index;
      }

      n = n->left;
    }
  }

  free(visited);
  return a_indexed_trie;
}

static unsigned int indexed_trie_states (const struct trie* a_trie, struct node_info* a_visited)
{
  unsigned int count = 0;

  indexed_trie_node_states(a_trie->root, a_visited, &count, 1);

  return count;
}

static void indexed_trie_node_states (struct node* a_node, struct node_info* a_visited, unsigned int* a_index, int a_new)
{
  if (a_new) {
    a_visited[a_node->visited].node = a_node;
    a_visited[a_node->visited].index = ++(*a_index);
  }

  if (a_node->right && !a_visited[a_node->right->visited].node) {
    indexed_trie_node_states(a_node->right, a_visited, a_index, 1);
  }

  if (a_node->left && !a_visited[a_node->left->visited].node) {
    indexed_trie_node_states(a_node->left, a_visited, a_index, 0);
  }
}

int indexed_trie_lookup (const struct indexed_trie* a_trie, const char* a_text)
{
  unsigned int index = 1;
  int yes = 0;

  do {
    unsigned char c = (unsigned char)*a_text++;

    yes = a_trie->nodes[index].yes[c];
    index = a_trie->nodes[index].pointers[c];
  } while (index && *a_text);

  return yes;
}
