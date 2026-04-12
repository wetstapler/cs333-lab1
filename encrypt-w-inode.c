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
	size_t write_size = BUFF;
	ssize_t read_size;
	int fd = -1;
	ssize_t fd_size;
	char i_path[BUFF] = {'\0'};
	char o_path[BUFF] = "./encrypted_message.txt";
	struct stat stat_buff;
	ino_t ino_key; //lowest order byte from the inode number
	char char_key;
	int is_verbose = 0; //faux-boolean

	while((options = getopt(argc, argv, "i:o:vh")) != -1){
	switch(options) {
	case 'i':
		strcpy(i_path, optarg);
		break;
	case 'o':
		strcpy(o_path, optarg);
		break;
	case 'v':
		fprintf(stderr, "Verbosity increased\n");
		is_verbose = 1;
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
"otherwise, outputs a file called %s\n\n", o_path);
		printf(
"-v\nincrease verbosity of stderr messages\n\n");
		printf(
"-h\ndisplay this help message\n\n");
		exit(EXIT_SUCCESS);
	}
	}

	//if the -i option is not used, prompt for input
	if(i_path[0] == '\0') {
		printf("Please type a message to be encrypted: ");
		getline(&message, &write_size, stdin);
	} else {//otherwise read the -i argument into message
		fd = open(i_path, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if(fd == -1) {
			if(is_verbose){
				fprintf(stderr, "cannot open %s\n", i_path);
			}
			exit(EXIT_FAILURE);
		}
		message = malloc(sizeof(char) * BUFF);
		read_size = read(fd, message, BUFF);
		if(read_size == -1) {
			if(is_verbose){
				fprintf(stderr, "read() error on %s\n", i_path);
			}
			exit(EXIT_FAILURE);
		}
	}
	
	//we must first make sure the file exists before we can generate
	//a key from the inode number
	fd = open(o_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH);
	if(fd == -1){
		if(is_verbose) {
			fprintf(stderr, "open() failure\n");
		}
		exit(EXIT_FAILURE);
	}

	printf("Encrypting message...\n");
	//get inode number and make key from lowest byte
	if(stat(o_path, &stat_buff) == -1) {
		if(is_verbose) {
			fprintf(stderr, "stat() failure\n");
		}
		exit(EXIT_FAILURE);
	}
	ino_key = stat_buff.st_ino & 255; //remove any data from beyond the lowest byte
	char_key = (char) ino_key;
	//encrypt with xor
	for(int i = 0; message[i] != '\0'; i++){
		message[i] = message[i] ^ char_key;
	}

	printf("Writing message to file...\n");
	fd_size = write(fd, message, write_size);
	if(fd_size == -1) {
		if(is_verbose){
			fprintf(stderr, "write() failure\n");
		}
		exit(EXIT_FAILURE);
	}

	printf("File written\n");


	free(message);


	return EXIT_SUCCESS;
}
