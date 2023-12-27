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

// int main(int ac, char **av, char **envp)
// {
// 	int id = fork();
// 	int	n;
// 	(void) ac;
// 	(void) av;

// 	while (*envp)
// 	{
// 		printf("%s\n", *envp);
// 		envp++;
// 	}
// 	if (id == 0)
// 		n = 1;
// 	else
// 		n = 6;
// 	if (id != 0)
// 		wait(NULL);
// 	int i;
// 	for (i = n; i < n + 5; i++)
// 	{
// 		printf("%d ", i);
// 		fflush(stdout);
// 	}
// 	if (id != 0)
// 		printf("\n");
// 	return (0);
// }

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

// int pid1 = fork();

	// if (pid1 == 0)
	// {
	// 	close(pipe1[0]);
	// 	printf("This is cmd1 process\n");
	// 	dup2(fd1, 0);
	// 	dup2(pipe1[1], 1);
	// 	execve(ft_strjoin(cmdpath[i], cmd1_path), cmd1, envp);
	// 	close(pipe1[1]);
	// 	close(pipe2[0]);
	// 	close(pipe2[1]);
	// 	// return (0);
	// }
	// waitpid(pid1, NULL, 0);

	// int	pid2 = fork();
	
	// if (pid2 == 0)
	// {
	// 	close(pipe1[1]);
	// 	close(pipe2[0]);
	// 	printf("This is cmd2 process\n");
	// 	dup2(pipe1[0], 0);
	// 	dup2(pipe2[1], 1);
	// 	execve(ft_strjoin(cmdpath[j], cmd2_path), cmd2, envp);
	// 	// close(pipe1[0]);
	// 	// close(pipe2[1]);
	// 	// return (0);
	// }

	// waitpid(pid2, NULL, 0);

	// int	pid3 = fork();
	// if (pid3 == 0)
	// {
	// 	close(pipe2[1]);
	// 	printf("This is cmd3 process\n");
	// 	dup2(pipe2[0], 0);
	// 	dup2(fd2, 1);
	// 	execve(ft_strjoin(cmdpath[k], cmd3_path), cmd3, envp);
	// 	close(pipe2[0]);
	// 	close(pipe1[0]);
	// 	close(pipe1[1]);
	// 	// return (0);
	// }
	// waitpid(pid3, NULL, 0);
	// close(pipe1[0]);
	// close(pipe1[1]);
	// close(pipe2[0]);
	// close(pipe2[1]);
	// printf("This is parent process\n");
	// // waitpid(pid1, NULL, 0);
	// // waitpid(pid2, NULL, 0);
	
	// // wait(NULL);
	// close(fd1);
	// close(fd2);


// if (m == 2)
// {
// 	closepipe(m, pipefd, ac);
// 	// close(pipefd[1][0]);
// 	// close(pipefd[1][1]);
// 	// close(pipefd[0][0]);
// 	dup2(fd1, 0);
// 	// dup2(pipefd[0][1], 1);
// }
// else if (m == 3)
// {
// 	try(pipefd, fd1, m);
// 	// close(pipefd[0][1]);
// 	// close(pipefd[1][0]);
// 	// dup2(pipefd[0][0], 0);
// 	// dup2(pipefd[1][1], 1);
// }
// else if (m == ac - 2)
// {
// 	close(pipefd[0][0]);
// 	close(pipefd[0][1]);
// 	close(pipefd[1][1]);
// 	dup2(pipefd[1][0], 0);
// 	dup2(fd2, 1);
// }