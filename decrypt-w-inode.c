//Marilyn Baumer 4/9/2026
//CS333
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF 100



int main(int argc, char** argv) {
	int fd = -1;
	char filename[BUFF] = "./encrypted_message.txt";
	char raw_message[BUFF] = {'\0'};
	ssize_t read_size;
        printf("there are %d arguments, the first of which is the program name, %s\n\n"
                        , argc, argv[0]);

	printf("Opening %s file\n", filename);
	fd = open(filename, O_RDONLY, S_IRWXU);
	if(fd == -1) {
		fprintf(stderr, "open() error\n\n");
		exit(EXIT_FAILURE);
	}
	read_size = read(fd, raw_message, BUFF);
	if(read_size == -1) {
		fprintf(stderr, "read() error\n\n");
		exit(EXIT_FAILURE);
	}
	printf("file reads: %s\n", raw_message);


	return EXIT_SUCCESS;
}
