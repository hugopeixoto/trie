
#include "trie.h"
#include "suffix_compression.h"

#include <stdio.h>
#include <string.h>


int main (int argc, char* argv[])
{
  int i;
  int nwords;
  char words[1<<16][1<<5];
  struct trie t;

  trie_init(&t);

  for (nwords = 0; fgets(words[nwords], 1<<5, stdin) != NULL; nwords++) {
    words[nwords][ strlen(words[nwords]) - 1] = 0;

    trie_insert(&t, words[nwords]);
  }

  for (i = 1; i  < argc; ++i) {
    printf("Lookup %s: %d\n", argv[i], trie_lookup(&t, argv[i]));
  }

  printf("node count: %d\n", t.node_count_);
  trie_suffix_compress(&t);
  printf("node count: %d\n", t.node_count_);

  for (i = 1; i  < argc; ++i) {
    printf("Lookup %s: %d\n", argv[i], trie_lookup(&t, argv[i]));
  }

  return 0;
}

