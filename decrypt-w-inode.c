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
	int fd = -1;
	ssize_t fd_size;
	size_t write_size = BUFF;
	char i_path[BUFF] = "./encrypted_message.txt";
	char o_path[BUFF] = "./decrypted_message.txt";
	char raw_message[BUFF] = {'\0'};
	ssize_t read_size;
	struct stat stat_buff;
	ino_t ino_key;
	char char_key;
	int is_verbose = 0; //faux-boolean


	while((options = getopt(argc, argv, "i:o:vh")) != -1) {
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
"To use this program, run %s\n", argv[0]);
		printf(
"You can use the following options:\n");
		printf(
"-i ifn\nuse named input file. Not using this option uses the path %s\n\n", i_path);
		printf(
"-o ofn\noutput a named file with the decrypted message\n");
		printf(
"otherwise, outputs a file called %s\n\n", i_path);
		printf(
"-v\nincrease verbosity of stderr messages\n\n");
		printf(
"-h\ndisplay this help message\n\n");
		exit(EXIT_SUCCESS);
	}
	}

	printf("Opening %s file\n", i_path);
	fd = open(i_path, O_RDONLY, S_IRWXU);
	if(fd == -1) {
		if(is_verbose){
			fprintf(stderr, "open() error\n\n");
		}
		exit(EXIT_FAILURE);
	}
	read_size = read(fd, raw_message, BUFF);
	if(read_size == -1) {
		if(is_verbose){
			fprintf(stderr, "read() error\n\n");
		}
		exit(EXIT_FAILURE);
	}

	printf("Decrypting...\n");
	//get inode number and make key from lowest byte
	if(stat(i_path, &stat_buff) == -1) {
		if(is_verbose){
			fprintf(stderr, "stat() failure\n");
		}
		exit(EXIT_FAILURE);
	}
	ino_key = stat_buff.st_ino & 255; //remove any data from beyond the lowest byte
	char_key = (char) ino_key;
	//encrypt with xor
	for(int i = 0; raw_message[i] != '\0'; i++){
		raw_message[i] = raw_message[i] ^ char_key;
	}
	printf("The hidden message reads:\n%s\n", raw_message);
	printf("Writing to %s...\n", o_path);
	close(fd);
	fd = open(o_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if(fd == -1) {
		if(is_verbose){
			fprintf(stderr, "open() error\n");
		}
		exit(EXIT_FAILURE);
	}
	fd_size = write(fd, raw_message, write_size);
	if(fd_size == -1) {
		if(is_verbose){
			fprintf(stderr, "write() error\n");
		}
		exit(EXIT_FAILURE);
	}

	printf("Output file written\n");


	return EXIT_SUCCESS;
}
