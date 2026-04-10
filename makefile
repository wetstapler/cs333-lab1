comp = gcc -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -Wno-return-local-addr -Wunsafe-loop-optimizations -Wuninitialized -Werror -Wextra -o encrypt-w-inode

all: encrypt-w-inode decrypt-w-inode encrypt-w-symlink decrypt-w-symlink

encrypt-w-inode: encrypt-w-inode.o
	$(comp) -o encrypt-w-inode encrypt-w-inode.o

encrypt-w-inode.o: encrypt-w-inode.c
	$(comp) -c -o encrypt-w-inode.o encrypt-w-inode.c

decrypt-w-inode: decrypt-w-inode.o
	$(comp) -o decrypt-w-inode decrypt-w-inode.o

decrypt-w-inode.o: decrypt-w-inode.c
	$(comp) -c -o decrypt-w-inode.o decrypt-w-inode.c

encrypt-w-symlink: encrypt-w-symlink.o
	$(comp) -o encrypt-w-symlink

encrypt-w-symlink.o: encrypt-w-symlink.c
	$(comp) -c -o encrypt-w-inode.o

decrypt-w-symlink: decrypt-w-symlink.o
	$(comp) -o encrypt-w-inode

decrypt-w-symlink.o: decrypt-w-symlink.c
	$(comp) -c -o decrypt-w-symlink.o

clean: o-clean
	rm -f encrypt-w-inode
	rm -f decrypt-w-inode
	rm -f encrypt-w-symlink
	rm -f decrypt-w-symlink
	rm -f *.txt

o-clean:
	rm -f *.o
