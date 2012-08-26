
SRC=suffix_compression.c trie.c node.c indexed_trie.c packed_trie.c
OBJ=$(SRC:%.c=%.o)

CFLAGS=-pedantic -Wall -Wextra -Werror -ansi -Wconversion -Wdouble-promotion -Wdeclaration-after-statement -Wundef -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith -Wbad-function-cast -g -pg
LDFLAGS=-pedantic -Wall -Wextra -Werror -ansi -Wconversion -Wdouble-promotion -Wdeclaration-after-statement -Wundef -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith -Wbad-function-cast -g -pg

all: trie


trie: libtrie.a main.o
	gcc -o trie main.o $(LDFLAGS) -L. -ltrie

libtrie.a: $(OBJ)
	ar rcs libtrie.a $(OBJ)
