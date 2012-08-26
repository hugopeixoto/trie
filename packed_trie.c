
#include "packed_trie.h"
#include "indexed_trie.h"

#include <stdlib.h>
#include <string.h>

struct packed_trie_data {
  unsigned int* next[256];
  unsigned int* prev[256];
};

static void packed_trie_grow (struct packed_trie*, struct packed_trie_data*, unsigned int);
static void packed_trie_place (struct packed_trie*, struct packed_trie_data*, const struct indexed_trie_node*, unsigned int, unsigned int, unsigned int);
static int packed_trie_fits (const struct packed_trie*, const struct indexed_trie_node*, unsigned int, unsigned int);
static void packed_trie_convert (struct packed_trie*, struct packed_trie_data*, const struct indexed_trie*);

struct packed_trie* packed_trie_alloc ()
{
  return (struct packed_trie*)malloc(sizeof(struct packed_trie));
}

void packed_trie_dealloc (struct packed_trie* a_trie)
{
  free(a_trie->nodes);
  free(a_trie);
}

struct packed_trie* packed_trie_init (
    struct packed_trie* a_packed_trie,
    const struct indexed_trie* a_trie)
{
  struct packed_trie_data* data = NULL;
  int i;

  a_packed_trie->count = 0;
  a_packed_trie->nodes = NULL;

  data = (struct packed_trie_data*)malloc(sizeof(struct packed_trie_data));

  for (i = 0; i < 256; ++i) {
    data->next[i] = NULL;
    data->prev[i] = NULL;
  }

  /* Buffer it up */
  packed_trie_grow(a_packed_trie, data, a_trie->count + 256);

  packed_trie_convert(a_packed_trie, data, a_trie);

  for (i = 0; i < 256; ++i) {
    free(data->next[i]);
    free(data->prev[i]);
  }
  
  free(data);

  return a_packed_trie;
}

static void packed_trie_convert (
    struct packed_trie* a_packed_trie,
    struct packed_trie_data* a_data,
    const struct indexed_trie* a_trie)
{
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int* positions = NULL;

  positions = (unsigned int*)calloc(a_trie->count, sizeof(unsigned int));

  for (i = 1; i < a_trie->count; ++i) {
    unsigned int first_used = 256;
    unsigned int last_used = 0;

    for (j = 0; j < 256; ++j) {
      if (a_trie->nodes[i].pointers[j] || a_trie->nodes[i].yes[j]) {
        last_used = j;
        if (first_used == 256) {
          first_used = j;
        }
      }
    }

    for (j = a_data->next[first_used][0];; j = a_data->next[first_used][j]) {
      packed_trie_grow(a_packed_trie, a_data, j + 256);
      if (!a_packed_trie->nodes[j].used && packed_trie_fits(a_packed_trie, &a_trie->nodes[i], first_used, j)) {
        break;
      }
    }

    packed_trie_place(a_packed_trie, a_data, &(a_trie->nodes[i]), first_used, last_used, j);
    positions[i] = j;
  }

  for (i = 0; i < a_packed_trie->count; ++i) {
    a_packed_trie->nodes[i].transition = positions[a_packed_trie->nodes[i].transition];
  }

  free(positions);
}

static void packed_trie_place (
    struct packed_trie* a_trie,
    struct packed_trie_data* a_data,
    const struct indexed_trie_node* a_node,
    unsigned int a_first_used,
    unsigned int a_last_used,
    unsigned int a_position)
{
  unsigned int i;
  unsigned int j;
  unsigned int k;

  a_trie->nodes[a_position].used = 1;

  for (i = a_first_used; i < 1 + a_last_used; ++i) {
    if (a_node->yes[i] || a_node->pointers[i]) {
      a_trie->nodes[a_position + i].value = (unsigned char)i;
      a_trie->nodes[a_position + i].yes = a_node->yes[i];
      a_trie->nodes[a_position + i].transition = a_node->pointers[i];

      /* Optimization section */
      k = 256 < a_position + i ? 256 : a_position + i;
      for (j = 0; j < k; ++j) {
        unsigned int p = a_position + i - j;

        a_data->next[j][a_data->prev[j][p]] = a_data->next[j][p];
        a_data->prev[j][a_data->next[j][p]] = a_data->prev[j][p];
        a_data->prev[j][p] = a_data->next[j][p] = p;
      }
    }
  }
}

static int packed_trie_fits (
    const struct packed_trie* a_trie,
    const struct indexed_trie_node* a_node,
    unsigned int a_first_used,
    unsigned int a_position)
{
  unsigned int i = 0;
  for (i = a_first_used; i < 256; ++i) {
    if (a_trie->nodes[a_position + i].value && (a_node->pointers[i] || a_node->yes[i])) {
      return 0;
    }
  }

  return 1;
}

static void packed_trie_grow (
    struct packed_trie* a_trie,
    struct packed_trie_data* a_data,
    unsigned int a_elements)
{
  unsigned int new_count = a_trie->count ? a_trie->count : 1024;

  while (new_count < a_elements) {
    new_count *= 2;
  }

  if (new_count > a_trie->count) {
    unsigned int i;
    unsigned int j;

    a_trie->nodes = realloc(a_trie->nodes, new_count * sizeof(struct packed_trie_node));
    memset(a_trie->nodes + a_trie->count, 0, (new_count - a_trie->count) * sizeof(struct packed_trie_node));

    for (i = 0; i < 256; ++i) {
      a_data->next[i] = realloc(a_data->next[i], new_count * sizeof(unsigned int));
      a_data->prev[i] = realloc(a_data->prev[i], new_count * sizeof(unsigned int));

      for (j = a_trie->count; j < new_count; ++j) {
        a_data->next[i][j] = j + 1;
        a_data->prev[i][j] = j - 1;
      }
    }

    a_trie->count = new_count;
  }
}

int packed_trie_lookup (const struct packed_trie* a_trie, const char* a_text)
{
  unsigned int index = 1;
  int yes = 0;

  do {
    unsigned char c = (unsigned char)*a_text++;

    if (c != a_trie->nodes[index + c].value) {
      return 0;
    }

    yes = a_trie->nodes[index + c].yes;
    index = a_trie->nodes[index + c].transition;
  } while (index && *a_text);

  return yes && !*a_text;
}
