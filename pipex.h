#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>
# include "get_next_line.h"

typedef struct s_pipe {
    int     ac;
    char    **envp;
    int     **pipefd;
    int     fd1;
    int     fd2;
    char    ***cmd;
    char    **cmd_dir;
    int     heredoc;
    int     fd[2];
    char    *input;
}   t_pipe;

#endif