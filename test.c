#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
 
// int main()
// {

// 	int fd;
 
// 	printf("fd: %d\n", STDOUT_FILENO);
//     fd = open("example.txt", O_WRONLY | O_CREAT, 0644);
// 	dup2(fd, STDOUT_FILENO);
// 	close(fd);
//     printf("fd: %d\n", fd);
//     printf("fd: %d\n", STDOUT_FILENO);
// 	printf("This is printed in example.txt!\n");
 
// 	return (0);
// }

int main(int ac, char **av, char **envp)
{
	int id = fork();
	int	n;
	(void) ac;
	(void) av;

	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
	if (id == 0)
		n = 1;
	else
		n = 6;
	if (id != 0)
		wait(NULL);
	int i;
	for (i = n; i < n + 5; i++)
	{
		printf("%d ", i);
		fflush(stdout);
	}
	if (id != 0)
		printf("\n");
	return (0);
}

// int main(void)
// {
// 	int	id1 = fork();
// 	int id2 = fork();
// 	if (id1 == 0)
// 	{
// 		if (id2 == 0)
// 			printf("We process y\n");
// 		else
// 			printf("We process x\n");
// 	}
// 	else
// 	{
// 		if (id2 == 0)
// 			printf("We process z\n");
// 		else
// 			printf("We process parent process\n");
// 	}
// 	while (wait(NULL) != -1 || errno != ECHILD)
// 		printf("Waited for a child to finish\n");
// 	return (0);
// }

// int main(void)
// {
// 	int arr[] = { 1,2,3,4,1,2 };
// 	int start, end;
// 	int fd[2];
// 	int arrSize = sizeof(arr) / sizeof(int);

// 	if (pipe(fd) == -1)
// 		return 1;

// 	int id = fork();
// 	if (id == -1)
// 		return 2;

// 	// if (id != 0)
// 	// 	wait(NULL);
// 	if (id == 0)
// 	{
// 		start = 0;
// 		end = start + arrSize / 2;
// 	}
// 	else
// 	{
// 		start = arrSize / 2;
// 		end = arrSize;
// 	}

// 	int sum = 0;
// 	int i;
// 	for (i = start; i< end; i++)
// 		sum += arr[i];

// 	printf("Calculated partial sum: %d\n", sum);

// 	if (id == 0)
// 	{
// 		close(fd[0]);
// 		if (write(fd[1], &sum, sizeof(sum)) == -1)
// 			return 3;
// 		close(fd[1]);
// 	}
// 	else
// 	{
// 		int sumFromChild;
// 		close(fd[1]);
// 		if (read(fd[0], &sumFromChild, sizeof(sumFromChild)) == -1)
// 			return 4;
// 		close(fd[0]);

// 		int totalSum = sum + sumFromChild;
// 		printf("Total sum is %d\n", totalSum);
// 		wait(NULL);
// 	}
// }

