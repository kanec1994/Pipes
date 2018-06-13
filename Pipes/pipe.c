#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
	pid_t pid;
	int fd;
	int status;
	int pipeDesc[2];
	char buffer[30];
	
	if(pipe(pipeDesc) == -1){
		perror("Pipe failed!\n");
		exit(1);
	}

	if((pid = fork()) < 0) {
		perror("Fork failed!\n");
		exit(1);
	}
	
	if(pid == 0){
		close(pipeDesc[1]);
		if(read(pipeDesc[0], buffer, 12) <= 0 ) {
			perror("Child read failed!\n");
			exit(1);
		}
		printf("In child process: %s\n", buffer);
		close(pipeDesc[0]);
		exit(0);
	}
	else{
		close(pipeDesc[0]);
		fd = open("data.txt", O_RDONLY);
		read(fd, buffer, 12);
		buffer[strlen(buffer)-1] = '\0';
		if(write(pipeDesc[1], buffer, 12) <= 0) {
			perror("Parent write failed!\n");
			exit(1);
		}
		close(fd);
		wait(pid, &status, 0);
		close(pipeDesc[1]);
	}
}
