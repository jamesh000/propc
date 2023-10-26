all: cpc pfunc ttable

debug: cpcdebug pfuncdebug ttabledebug

cpc: cpc_src/main.c cpc_src/compile.c cpc_src/parse.c cpc_src/bintree.c
	clang -Wall -Wextra -o $@ $^

pfunc: pfunc_src/main.c
	clang -Wall -Wextra -o $@ $^

ttable: ttable_src/main.c
	clang -Wall -Wextra -o $@ $^

cpcdebug: cpc_src/main.c cpc_src/compile.c cpc_src/parse.c cpc_src/bintree.c
	clang -g -Wall -Wextra -o $@ $^

pfuncdebug: pfunc_src/main.c
	clang -g -Wall -Wextra -o $@ $^

ttabledebug: ttable_src/main.c
	clang -g -Wall -Wextra -o $@ $^
