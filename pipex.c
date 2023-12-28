#include "pipex.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	int		j;
	char	*t;

	i = 0;
	j = -1;
	while (s1[i])
		i++;
	t = malloc((i + 1) * sizeof(char));
	if (!t)
		return (0);
	while (s1[++j])
	{
		t[j] = s1[j];
	}
	t[j] = '\0';
	return (t);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	count;

	count = 0;
	while (s1[count] != '\0' && s2[count] != '\0')
	{
		if (s1[count] == s2[count])
			count++;
		else
			return (s1[count] - s2[count]);
	}
	return (s1[count] - s2[count]);
}

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

static char	*fill_letter(char const *str, char set)
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
			split_str[i] = fill_letter(s, set);
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

void    dupnclosereadpipe(int index, t_pipe **a, int i)
{
    if (i != index - 3)
        close((*a)->pipefd[i][0]);
    else
        dup2((*a)->pipefd[i][0], 0);
}

void    dupnclosewritepipe(int index, t_pipe **a, int i)
{
    if (i != index - 2 - (*a)->heredoc)
        close((*a)->pipefd[i][1]);
    else
        dup2((*a)->pipefd[i][1], 1);
}

void    closepipe(int index, t_pipe **a)
{
    int i;

    i = -1;
    while (++i < (*a)->ac - 4 - (*a)->heredoc)
    {
        if (index == 2 + (*a)->heredoc)
        {
            close((*a)->pipefd[i][0]);
            dupnclosewritepipe(index, a, i);
        }
        else if (index == (*a)->ac - 2)
        {
            close((*a)->pipefd[i][1]);
            dupnclosereadpipe(index, a, i);
        }
        else
        {
            dupnclosereadpipe(index, a, i);
            dupnclosewritepipe(index, a, i);
        }
    }
	// if ((*a)->heredoc)
	// 	close((*a)->fd[0]);
	// if ((*a)->heredoc)
	// 	close((*a)->fd[1]);
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

void	initpipefd(t_pipe **a)
{
	int		totalpipefd;

	totalpipefd = (*a)->ac - 4 - (*a)->heredoc;
	(*a)->pipefd = (int **)malloc(sizeof(int *) * totalpipefd);
	if (!(*a)->pipefd)
		failedexit("Malloc for pipefd failed\n");
	while (--totalpipefd >= 0)
	{
		(*a)->pipefd[totalpipefd] = (int *)malloc(sizeof(int) * 2);
		if (!(*a)->pipefd[totalpipefd])
			failedexit("Malloc for pipefd[totalpipefd] failed\n");
		if (pipe((*a)->pipefd[totalpipefd]) == -1)
			failedexit("Pipe error\n");
	}
	if ((*a)->heredoc)
	{
		if (pipe((*a)->fd) == -1)
			failedexit("Pipe error\n");
	}
}

int	heredocprocess(t_pipe **a)
{
	int i;

    i = -1;
	while (++i < (*a)->ac - 4 - (*a)->heredoc)
	{
		close((*a)->pipefd[i][0]);
		close((*a)->pipefd[i][1]);
	}
	close((*a)->fd[0]);
	dup2((*a)->fd1, 1);
	printf("%s", (*a)->input);
	return (0);
}

int	forkprocess(t_pipe **a, char **envp, char **av)
{
	int	m;
	int	pid;

	m = 2;
	while (m < (*a)->ac - 1)
	{
		pid = fork();
		if (pid == -1)
			failedexit("Fork error.\n");
		if (pid == 0)
		{
			if (m == 2 && (*a)->heredoc == 1)
				return (heredocprocess(a));
			closepipe(m, a);
			if (m == 3 && (*a)->heredoc == 1)
				(*a)->fd1 = open("sample.txt", O_RDONLY);
			if (m == (*a)->ac - 2 && (*a)->heredoc == 1)
				(*a)->fd2 = open(av[(*a)->ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (m == 2 + (*a)->heredoc)
				dup2((*a)->fd1, 0);
			else if (m == (*a)->ac - 2)
				dup2((*a)->fd2, 1);
			execve((*a)->cmd_dir[m - 2 - (*a)->heredoc], (*a)->cmd[m - 2 - (*a)->heredoc], envp);
			return(0);
		}
		m++;
	}
	return (0);
}

void freepipe(t_pipe **a)
{
	free((*a)->envp);
	free((*a)->cmd_dir);
	free((*a)->cmd);
	free((*a)->pipefd);
	if ((*a)->heredoc)
		free((*a)->input);
}

void freencloseall(t_pipe **a)
{
	int	i;
	int	j;

	i = -1;
	while ((*a)->envp[++i])
		free((*a)->envp[i]);
	i = -1;
	while (++i < (*a)->ac - 3 - (*a)->heredoc)
		free((*a)->cmd_dir[i]);
	while (--i >= 0)
	{
		j = -1;
		while ((*a)->cmd[i][++j])
			free((*a)->cmd[i][j]);
		free((*a)->cmd[i]);
	}
	while (++i < (*a)->ac - 4 - (*a)->heredoc)
	{
		close((*a)->pipefd[i][0]);
		close((*a)->pipefd[i][1]);
		free((*a)->pipefd[i]);
	}
	freepipe(a);
}

void prepcmd(t_pipe **a, int ac, char **av)
{
	int		m;
	int		i;
	char	*cmd_path;

	m = 2 + (*a)->heredoc;
	while (m < ac - 1)
	{
		(*a)->cmd[m - 2 - (*a)->heredoc] = ft_split(av[m], ' ');
		cmd_path = ft_strjoin("/", (*a)->cmd[m - 2 - (*a)->heredoc][0]);
		i = -1;
		while ((*a)->envp[++i])
		{
			(*a)->cmd_dir[m - 2 - (*a)->heredoc] = ft_strjoin((*a)->envp[i], cmd_path);
			if (!(access((*a)->cmd_dir[m - 2 - (*a)->heredoc], X_OK)))
				break ;
			free((*a)->cmd_dir[m - 2 - (*a)->heredoc]);
		}
		free(cmd_path);
		m++;
	}
}

void	initallvar(t_pipe **a, int ac, char **av, char **envp)
{
	(*a)->heredoc = 0;
	if (ft_strcmp("here_doc", av[1]) == 0)
		(*a)->heredoc = 1;
	if ((ac < 6 && (*a)->heredoc) || ac < 5)
		failedexit("Insufficient input error");
	(*a)->ac = ac;
	(*a)->envp = initenvp(envp);
	initpipefd(a);
	if ((*a)->heredoc)
		(*a)->fd1 = open("sample.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		(*a)->fd1 = open(av[1], O_RDONLY);
	if ((*a)->fd1 == -1)
		failedexit("Input file error");
	if ((*a)->heredoc)
		(*a)->fd2 = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
	else
		(*a)->fd2 = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	(*a)->cmd = (char ***)malloc(sizeof(char **) * (ac - 3 - (*a)->heredoc));
	if (!(*a)->cmd)
		failedexit("Malloc for cmd failed\n");
	(*a)->cmd_dir = (char **)malloc(sizeof(char *) * (ac - 3 - (*a)->heredoc));
	if (!(*a)->cmd_dir)
		failedexit("Malloc for cmd_str failed\n");
	prepcmd(a, ac, av);
}

void	initinput(t_pipe **a, char **av)
{
	char	*str;
	char	*limiter;
	char	*buffer;
	char	*tmp;
	
	write(1, "pipe heredoc> ", 14);
	str = get_next_line(0);
	limiter = ft_strjoin(av[2], "\n");
	buffer = ft_strdup("");
	while (ft_strcmp(str, limiter))
	{
		tmp = ft_strjoin(buffer, str);
		free(str);
		free(buffer);
		buffer = ft_strdup(tmp);
		free(tmp);
		write(1, "pipe heredoc> ", 14);
		str = get_next_line(0);
	}
	free(str);
	(*a)->input = buffer;
}

int main(int ac, char **av, char **envp)
{
	t_pipe	*a;

	a = (t_pipe *) malloc(sizeof(t_pipe));
	if (!a)
		failedexit("Malloc for t_pipe failed.\n");
	initallvar(&a, ac, av, envp);
	if (a->heredoc)
		initinput(&a, av);
	forkprocess(&a, envp, av);
	wait(NULL);
	freencloseall(&a);
	free(a);
	return (0);
}
