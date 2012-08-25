
SRC=main.c suffix_compression.c trie.c node.c indexed_trie.c
OBJ=$(SRC:%.c=%.o)

CFLAGS=-pedantic -Wall -Wextra -Werror -ansi -Wconversion -Wdouble-promotion -Wdeclaration-after-statement -Wundef -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith -Wbad-function-cast -g
LDFLAGS=-pedantic -Wall -Wextra -Werror -ansi -Wconversion -Wdouble-promotion -Wdeclaration-after-statement -Wundef -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith -Wbad-function-cast -g

all: trie


trie: $(OBJ)
	gcc -o trie $(OBJ) $(LDFLAGS)
