#Makefile

my-malloc: my-malloc.c
	gcc -g -Wall -pedantic -rdynamic -shared -fPIC -o my-malloc.so my-malloc.c

malloc-test: malloc-test.c
	gcc -Wall -pedantic -o malloc-test malloc-test.c

.PHONY: clean
clean:
	rm -f my-malloc
	rm -f malloc-test

.PHONY: debug
debug:
	gdb --args env LD_PRELOAD=./my-malloc.so ./malloc-test

.PHONY: test
test:
	LD_PRELOAD=./my-malloc.so ./malloc-test

.PHONY: test-ls
test-ls:
	LD_PRELOAD=./my-malloc.so ls
