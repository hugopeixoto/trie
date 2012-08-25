
#ifndef _TRIE_H_
#define _TRIE_H_

struct node;

struct trie {
  struct node* root_;
  unsigned int node_count_;
};

struct trie* trie_alloc ();

void trie_dealloc (struct trie*);

struct trie* trie_init (struct trie*);

void trie_insert (struct trie*, const char*);

int trie_lookup (struct trie*, const char*);

int trie_validate (const struct trie* a_trie);

void trie_reset_order (struct trie* a_trie);

#endif

