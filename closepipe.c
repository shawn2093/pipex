void    dupandclosereadpipe(int index, int **pipefd, int i)
{
    if (i != index - 3)
        close(pipefd[i][0]);
    else
        dup2(pipefd[i][0], 0);
}

void    dupandclosewritepipe(int index, int **pipefd, int i)
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
            dupandclosewritepipe(index, pipefd, i);
        }
        else if (index == ac - 2)
        {
            close(pipefd[i][1]);
            dupandclosereadpipe(index, pipefd, i);
        }
        else
        {
            dupandclosereadpipe(index, pipefd, i);
            dupandclosewritepipe(index, pipefd, i);
        }
    }
}
