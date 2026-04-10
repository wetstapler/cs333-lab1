//Marilyn Baumer 4/9/2026
//CS333
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFF 100



int main(int argc, char** argv) {
	int fd = -1;
	char path[BUFF] = "./encrypted_message.txt";
	char raw_message[BUFF] = {'\0'};
	ssize_t read_size;
	struct stat stat_buff;
	ino_t ino_key;
	char char_key;

        printf("there are %d arguments, the first of which is the program name, %s\n\n"
                        , argc, argv[0]);

	printf("Opening %s file\n", path);
	fd = open(path, O_RDONLY, S_IRWXU);
	if(fd == -1) {
		fprintf(stderr, "open() error\n\n");
		exit(EXIT_FAILURE);
	}
	read_size = read(fd, raw_message, BUFF);
	if(read_size == -1) {
		fprintf(stderr, "read() error\n\n");
		exit(EXIT_FAILURE);
	}

	printf("Decrypting...\n");
	//get inode number and make key from lowest byte
	if(stat(path, &stat_buff) == -1) {
		fprintf(stderr, "stat() failure\n");
	}
	ino_key = stat_buff.st_ino & 255; //remove any data from beyond the lowest byte
	char_key = (char) ino_key;
	//encrypt with xor
	for(int i = 0; raw_message[i] != '\0'; i++){
		raw_message[i] = raw_message[i] ^ char_key;
	}
	printf("The hidden message reads:\n%s\n", raw_message);


	return EXIT_SUCCESS;
}
