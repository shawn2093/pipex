#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

static int	is_sep(char c, char set)
{
	if (c == set)
		return (1);
	return (0);
}

static int	word_count(char const *str, char set)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && is_sep(str[i], set))
			i++;
		if (str[i] && !is_sep(str[i], set))
			count++;
		while (str[i] && !is_sep(str[i], set))
			i++;
	}
	return (count);
}

static char	*fill_letters(char const *str, char set)
{
	char	*newstr;
	int		i;

	i = 0;
	while (!is_sep(str[i], set) && str[i])
		i++;
	newstr = (char *)malloc((i + 1) * sizeof(char));
	if (!newstr)
		return (0);
	i = 0;
	while (!is_sep(str[i], set) && str[i])
	{
		newstr[i] = str[i];
		i++;
	}
	newstr[i] = '\0';
	return (newstr);
}

char	**ft_split(char const *s, char set)
{
	char	**split_str;
	int		count;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	count = word_count(s, set);
	split_str = (char **)malloc((count + 1) * sizeof(char *));
	if (!split_str)
		return (0);
	while (*s)
	{
		while (*s && is_sep(*s, set))
			s++;
		if (*s)
		{
			split_str[i] = fill_letters(s, set);
			i++;
			while (*s && !is_sep(*s, set))
				s++;
		}
	}
	split_str[i] = 0;
	return (split_str);
}

size_t	ft_strlen(const char *s)
{
	size_t	count;

	count = 0;
	while (s[count] != '\0')
		count++;
	return (count);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	char	*str;
	int		i;
	int		j;

	if (s1 && !s2)
		return ((char *)s1);
	if (!s1 && s2)
		return ((char *)s2);
	if (!s1 && !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = (char *)malloc(total_len * sizeof(char));
	if (!str)
		return (NULL);
	i = -1;
	j = -1;
	while (s1[++i])
		str[i] = s1[i];
	while (s2[++j])
		str[i++] = s2[j];
	str[i] = '\0';
	return (str);
}


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

int main(int ac, char **av, char **envp)
{
	int		i;
	int		j;
	int		k;
	char	**cmdpath;
	int		pipe1[2];
	int		pipe2[2];
	(void)	ac;

	cmdpath = NULL;
	i = -1;
	while (envp[++i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			cmdpath = ft_split(envp[i] + 5, ':');
			break ;
		}
	}
	int		fd1 = open(av[1], O_RDONLY);
	int		fd2 = open(av[5], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	char	**cmd1 = ft_split(av[2], ' ');
	char	**cmd2 = ft_split(av[3], ' ');
	char	**cmd3 = ft_split(av[4], ' ');
	char	*cmd1_path = ft_strjoin("/", cmd1[0]);
	char	*cmd2_path = ft_strjoin("/", cmd2[0]);
	char	*cmd3_path = ft_strjoin("/", cmd3[0]);

	i = -1;
	if (cmdpath)
	{
		while (cmdpath[++i])
		{
			if (!(access(ft_strjoin(cmdpath[i], cmd1_path), X_OK)))
			{
				printf("%s\n", ft_strjoin(cmdpath[i], cmd1_path));
				break ;
			}
		}
	}
	j = -1;
	if (cmdpath)
	{
		while (cmdpath[++j])
		{
			if (!(access(ft_strjoin(cmdpath[j], cmd2_path), X_OK)))
			{
				printf("%s\n", ft_strjoin(cmdpath[j], cmd2_path));
				break ;
			}
		}
	}
	k = -1;
	if (cmdpath)
	{
		while (cmdpath[++k])
		{
			if (!(access(ft_strjoin(cmdpath[k], cmd3_path), X_OK)))
			{
				printf("%s\n", ft_strjoin(cmdpath[k], cmd3_path));
				break ;
			}
		}
	}

	pipe(pipe1);
	pipe(pipe2);
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

	int x = 0;

	while (x < 3)
	{
		int pid = fork();
		if (pid == 0)
		{
			if (x == 0)
			{
				close(pipe2[0]);
				close(pipe2[1]);
				close(pipe1[0]);
				printf("This is cmd1 process\n");
				dup2(fd1, 0);
				dup2(pipe1[1], 1);
				execve(ft_strjoin(cmdpath[i], cmd1_path), cmd1, envp);
				return(0);
			}
			else if (x == 1)
			{
				close(pipe1[1]);
				close(pipe2[0]);
				printf("This is cmd2 process\n");
				dup2(pipe1[0], 0);
				dup2(pipe2[1], 1);
				execve(ft_strjoin(cmdpath[j], cmd2_path), cmd2, envp);
				return(0);
			}
			else if (x == 2)
			{
				close(pipe1[0]);
				close(pipe1[1]);
				close(pipe2[1]);
				printf("This is cmd3 process\n");
				dup2(pipe2[0], 0);
				dup2(fd2, 1);
				execve(ft_strjoin(cmdpath[k], cmd3_path), cmd3, envp);
				return(0);
			}
		}
		x++;
	}
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[1]);
	close(pipe2[0]);
	// x = 0;
	// while (x < 2)
	// {
	// 	wait(NULL);
	// 	x++;
	// }
	return (0);
}