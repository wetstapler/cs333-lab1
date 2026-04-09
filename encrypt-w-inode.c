//Marilyn Baumer 4/9/2026
//CS333
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF 100



int main(int argc, char** argv) {
	char* message = NULL;
	size_t readsize = BUFF;
	int fd = -1;
	ssize_t fd_size;
	printf("there are %d arguments, the first of which is the program name, %s\n\n"
			, argc, argv[0]);
	printf("Please type a message to be encrypted: ");
	getline(&message, &readsize, stdin);

	printf("Message reads: %s\n", message);

	printf("Writing message to file...\n");
	fd = open("./encrypted_message.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	fd_size = write(fd, message, readsize);
	if (fd_size == -1) {
		fprintf(stderr, "write() failure\n");
		exit(EXIT_FAILURE);
	}

	printf("File written\n");




	return EXIT_SUCCESS;
}
