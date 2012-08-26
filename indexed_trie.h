
#ifndef _INDEXED_TRIE_H_
#define _INDEXED_TRIE_H_

struct trie;

struct indexed_trie_node {
  unsigned int pointers[256];
  char yes[256];
};

struct indexed_trie {
  struct indexed_trie_node* nodes;
  unsigned int count;
};

struct indexed_trie* indexed_trie_alloc ();

void indexed_trie_dealloc (struct indexed_trie*);

struct indexed_trie* indexed_trie_init (struct indexed_trie*, const struct trie*);

int indexed_trie_lookup (const struct indexed_trie* a_trie, const char* a_text);

#endif

