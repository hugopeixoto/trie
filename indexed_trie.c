
#include "indexed_trie.h"
#include "trie.h"
#include "node.h"

#include <stdlib.h>
#include <string.h>

struct node_info {
  struct node* node;
  unsigned int index;
};

static void indexed_trie_count (struct node*, struct node_info*, unsigned int*);

struct indexed_trie* indexed_trie_alloc ()
{
  return (struct indexed_trie*)malloc(sizeof(struct indexed_trie));
}

void indexed_trie_dealloc (struct indexed_trie* a_trie)
{
  free(a_trie->nodes_);
  free(a_trie);
}

struct indexed_trie* indexed_trie_init (struct indexed_trie* a_indexed_trie, const struct trie* a_trie)
{
  unsigned int i = 0;
  unsigned int nnodes = 1;
  struct node_info* visited =  NULL;
  
  visited = (struct node_info*)malloc((a_trie->node_count_ + 1) * sizeof(struct node_info));
  memset(visited, 0, sizeof(struct node_info) * (a_trie->node_count_ + 1));

  indexed_trie_count(a_trie->root_, visited, &nnodes);

  a_indexed_trie->nodes_ = (struct indexed_trie_node*)malloc(sizeof(struct indexed_trie_node) * nnodes);

  for (i = 0; i <= a_trie->node_count_; ++i) {
    unsigned int j = visited[i].index;
    struct node* n = visited[i].node;
    unsigned char c = (unsigned char) n->value;

    a_indexed_trie->nodes_[j].yes[c] = n->yes;
    if (n->right) {
      a_indexed_trie->nodes_[j].pointers[c] = visited[n->right->visited].index;
    }
  }
  
  return a_indexed_trie;
}

static void indexed_trie_count (struct node* a_node, struct node_info* a_visited, unsigned int* a_index)
{
  if (a_visited[a_node->visited].node) {
    return;
  }

  a_visited[a_node->visited].node = a_node;
  a_visited[a_node->visited].index = *a_index;

  if (a_node->left) {
    indexed_trie_count(a_node->left, a_visited, a_index);
  }

  if (a_node->right) {
    ++*a_index;
    indexed_trie_count(a_node->right, a_visited, a_index);
  }
}

int indexed_trie_lookup (const struct indexed_trie* a_trie, const char* a_text)
{
  unsigned int index = 1;
  int yes = 0;

  do {
    unsigned char c = (unsigned char)*a_text++;

    yes = a_trie->nodes_[index].yes[c];
    index = a_trie->nodes_[index].pointers[c];
  } while (index && *a_text);

  return yes;
}
