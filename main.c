
#include "trie.h"
#include "suffix_compression.h"
#include "indexed_trie.h"
#include "packed_trie.h"

#include <stdio.h>
#include <string.h>

char words[1<<16][1<<6];

int main (int argc, char* argv[])
{
  int i;
  int nwords;
  struct trie* t;
  struct indexed_trie* it;
  struct packed_trie* pt;

  t = trie_init(trie_alloc());

  for (nwords = 0; fgets(words[nwords], 1<<6, stdin) != NULL; nwords++) {
    words[nwords][ strlen(words[nwords]) - 1] = 0;

    trie_insert(t, words[nwords]);
  }

  puts("------------------ linked");
  printf("node count: %d\n", t->count);
  for (i = 1; i  < argc; ++i) {
    printf("Lookup %s: %d\n", argv[i], trie_lookup(t, argv[i]));
  }

  trie_suffix_compress(t);
  puts("------------------ compressed");
  printf("node count: %d\n", t->count);
  for (i = 1; i  < argc; ++i) {
    printf("Lookup %s: %d\n", argv[i], trie_lookup(t, argv[i]));
  }

  /* TODO(hpeixoto): this should be implicit somehow. */
  trie_reset_order(t);

  it = indexed_trie_init(indexed_trie_alloc(), t);
  puts("------------------ indexed");
  printf("node count: %d\n", it->count);
  for (i = 1; i  < argc; ++i) {
    printf("Lookup %s: %d\n", argv[i], indexed_trie_lookup(it, argv[i]));
  }

  pt = packed_trie_init(packed_trie_alloc(), it);
  puts("------------------ packed");
  printf("node count: %d\n", pt->count);
  for (i = 1; i  < argc; ++i) {
    printf("Lookup %s: %d\n", argv[i], packed_trie_lookup(pt, argv[i]));
  }

  packed_trie_dealloc(pt);
  indexed_trie_dealloc(it);
  trie_dealloc(t);

  return 0;
}

