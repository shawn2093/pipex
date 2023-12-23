#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
 
int main()
{
	int fd;
 
	printf("fd: %d\n", STDOUT_FILENO);
    fd = open("example.txt", O_WRONLY | O_CREAT, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
    printf("fd: %d\n", fd);
    printf("fd: %d\n", STDOUT_FILENO);
	printf("This is printed in example.txt!\n");
 
	return (0);
}