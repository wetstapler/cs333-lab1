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
	int options;
	char* message = NULL;
	size_t readsize = BUFF;
	int fd = -1;
	ssize_t fd_size;
	char path[BUFF] = "./encrypted_message.txt";
	struct stat stat_buff;
	ino_t ino_key; //lowest order byte from the inode number
	char char_key;

	printf("there are %d arguments, the first of which is the program name, %s\n\n"
			, argc, argv[0]);

	while((options = getopt(argc, argv, "i:o:vh")) != -1){
	switch(options) {
	case 'i':
		strcpy(path, optarg);
		break;
	case 'o':
		strcpy(path, optarg);
		break;
	case 'v':
		fprintf(stderr, "Verbosity increased\n");
		break;
	case 'h':
		printf(
"To use this program, run %s, and enter a message to encrypt\n", argv[0]);
		printf(
"when prompted. You can use the following options:\n");
		printf(
"-i ifn\nuse input file instead of being prompted for input message\n\n");
		printf(
"-o ofn\noutput a file with a particular name.\n");
		printf(
"otherwise, outputs a file called %s\n\n", path);
		printf(
"-v\nincrease verbosity of stderr messages\n\n");
		printf(
"-h\ndisplay this help message\n\n");
		exit(EXIT_SUCCESS);
	}
	}

	printf("Please type a message to be encrypted: ");
	getline(&message, &readsize, stdin);
	
	//we must first make sure the file exists before we can generate
	//a key from the inode number
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH);
	if(fd == -1){
		fprintf(stderr, "open() failure\n");
		exit(EXIT_FAILURE);
	}

	printf("Encrypting message...\n");
	//get inode number and make key from lowest byte
	if(stat(path, &stat_buff) == -1) {
		fprintf(stderr, "stat() failure\n");
		exit(EXIT_FAILURE);
	}
	ino_key = stat_buff.st_ino & 255; //remove any data from beyond the lowest byte
	char_key = (char) ino_key;
	//encrypt with xor
	for(int i = 0; message[i] != '\0'; i++){
		message[i] = message[i] ^ char_key;
	}

	printf("Writing message to file...\n");
	fd_size = write(fd, message, readsize);
	if(fd_size == -1) {
		fprintf(stderr, "write() failure\n");
		exit(EXIT_FAILURE);
	}

	printf("File written\n");


	free(message);


	return EXIT_SUCCESS;
}
