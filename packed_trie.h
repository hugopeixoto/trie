
#ifndef _PACKED_TRIE_H_
#define _PACKED_TRIE_H_

struct indexed_trie;

struct packed_trie_node {
  unsigned int transition;
  char yes;
  unsigned char value;
  char used;
};

struct packed_trie {
  unsigned int count;
  struct packed_trie_node* nodes;
};

struct packed_trie* packed_trie_alloc ();

void packed_trie_dealloc (struct packed_trie*);

struct packed_trie* packed_trie_init (struct packed_trie*, const struct indexed_trie*);

int packed_trie_lookup (const struct packed_trie*, const char*);

#endif

