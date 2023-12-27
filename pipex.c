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

void    dupnclosereadpipe(int index, int **pipefd, int i)
{
    if (i != index - 3)
        close(pipefd[i][0]);
    else
        dup2(pipefd[i][0], 0);
}

void    dupnclosewritepipe(int index, int **pipefd, int i)
{
    if (i != index - 2)
        close(pipefd[i][1]);
    else
        dup2(pipefd[i][1], 1);
}

void    closepipe(int index, int **pipefd, int ac)
{
    int i;

    i = -1;
    while (++i < ac - 4)
    {
        if (index == 2)
        {
            close(pipefd[i][0]);
            dupnclosewritepipe(index, pipefd, i);
        }
        else if (index == ac - 2)
        {
            close(pipefd[i][1]);
            dupnclosereadpipe(index, pipefd, i);
        }
        else
        {
            dupnclosereadpipe(index, pipefd, i);
            dupnclosewritepipe(index, pipefd, i);
        }
    }
}

char	**initenvp(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
	}
	return (0);
}

void	failedexit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

int		**initpipefd(int ac)
{
	int		totalpipefd;
	int		**pipefd;

	totalpipefd = ac - 4;
	pipefd = (int **)malloc(sizeof(int *) * totalpipefd);
	if (!pipefd)
		failedexit("Malloc for pipefd failed\n");
	while (--totalpipefd >= 0)
	{
		pipefd[totalpipefd] = (int *)malloc(sizeof(int) * 2);
		if (!pipefd[totalpipefd])
			failedexit("Malloc for pipefd[totalpipefd] failed\n");
		if (pipe(pipefd[totalpipefd]) == -1)
			failedexit("Pipe error\n");
	}
	return (pipefd);
}

int	forkprocess(int ac, char **envp, int **pipefd, int fd1, int fd2, char **cmd_str, char ***cmd)
{
	int	m;
	int	pid;

	m = 2;
	while (m < ac - 1)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("Fork error\n");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			closepipe(m, pipefd, ac);
			if (m == 2)
				dup2(fd1, 0);
			else if (m == ac - 2)
				dup2(fd2, 1);
			execve(cmd_str[m - 2], cmd[m - 2], envp);
			return(0);
		}
		m++;
	}
	return (0);
}

void freencloseall(char **cmdpath, char	**cmd_str, char	***cmd, int **pipefd, int ac)
{
	int	i;
	int	j;

	i = -1;
	while (cmdpath[++i])
		free(cmdpath[i]);
	free(cmdpath);
	i = -1;
	while (cmd_str[++i])
		free(cmd_str[i]);
	free(cmd_str);
	i = -1;
	while (cmd[++i])
	{
		j = -1;
		while (cmd[i][++j])
			free(cmd[i][j]);
		free(cmd[i]);
	}
	free(cmd);
	i = -1;
	while (++i < ac - 4)
	{
		close(pipefd[i][0]);
		close(pipefd[i][1]);
		free(pipefd[i]);
	}
	free(pipefd);
}

void prepcmd(char ***cmd, char **cmd_str, char **av, int ac, char **cmdpath)
{
	int		m;
	int		i;
	char	*cmd_path;

	m = 2;
	while (m < ac - 1)
	{
		cmd[m - 2] = ft_split(av[m], ' ');
		cmd_path = ft_strjoin("/", cmd[m - 2][0]);
		i = -1;
		while (cmdpath[++i])
		{
			cmd_str[m - 2] = ft_strjoin(cmdpath[i], cmd_path);
			if (!(access(cmd_str[m - 2], X_OK)))
				break ;
			free(cmd_str[m - 2]);
		}
		free(cmd_path);
		m++;
	}
}

int main(int ac, char **av, char **envp)
{
	char	**cmdpath;
	int		**pipefd;
	int		fd1;
	int		fd2;
	char	***cmd;
	char	**cmd_str;

	cmdpath = initenvp(envp);
	pipefd = initpipefd(ac);
	fd1 = open(av[1], O_RDONLY);
	if (fd1 == -1)
		failedexit("Input file error");
	fd2 = open(av[5], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	cmd = (char ***)malloc(sizeof(char **) * (ac - 3));
	if (!cmd)
		failedexit("Malloc for cmd failed\n");
	cmd_str = (char **)malloc(sizeof(char *) * (ac - 3));
	if (!cmd_str)
		failedexit("Malloc for cmd_str failed\n");
	prepcmd(cmd, cmd_str, av, ac, cmdpath);
	forkprocess(ac, envp, pipefd, fd1, fd2, cmd_str, cmd);
	wait(NULL);
	freencloseall(cmdpath, cmd_str, cmd, pipefd, ac);
	return (0);
}
