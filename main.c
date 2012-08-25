
#include "trie.h"
#include "suffix_compression.h"
#include "indexed_trie.h"

#include <stdio.h>
#include <string.h>

char words[1<<16][1<<5];

int main (int argc, char* argv[])
{
  int i;
  int nwords;
  struct trie* t;
  struct indexed_trie* it;

  t = trie_init(trie_alloc());

  for (nwords = 0; fgets(words[nwords], 1<<5, stdin) != NULL; nwords++) {
    words[nwords][ strlen(words[nwords]) - 1] = 0;

    trie_insert(t, words[nwords]);
  }

  printf("validation: %d\n", trie_validate(t));

  for (i = 1; i  < argc; ++i) {
    printf("Lookup %s: %d\n", argv[i], trie_lookup(t, argv[i]));
  }

  printf("node count: %d\n", t->count);
  trie_suffix_compress(t);
  printf("node count: %d\n", t->count);
  printf("validation: %d\n", trie_validate(t));

  for (i = 1; i  < argc; ++i) {
    printf("Lookup %s: %d\n", argv[i], trie_lookup(t, argv[i]));
  }

  trie_reset_order(t);

  it = indexed_trie_init(indexed_trie_alloc(), t);

  trie_dealloc(t);
  indexed_trie_dealloc(it);

  return 0;
}

