/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:04:01 by albertini         #+#    #+#             */
/*   Updated: 2024/10/11 16:58:07 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void	p_err(char *err)
{
	while (*err)
		write(2, err++, 1);
}

int	cd(char **av, int i)
{
	if (i != 2)
		return (p_err("err: cd: bad argument\n"), 1);
	if (chdir(av[1]) == -1)
		return (p_err("err: cd: cant change dir to "), p_err(av[1]), p_err("\n"), 1);
	return (0);
}

void	set_pipe(int has_pipe, int *fd, int end)
{
	if (has_pipe && (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
	{
		p_err("err: fatal \n");
		exit(1);
	}
}

int	exec(char **av, int i, char **envp)
{
	int	has_pipe;
	int	fd[2];
	int	pid;
	int	status;

	has_pipe = av[i] && !strcmp(av[i], "|");
	if (!has_pipe && !strcmp(*av, "cd"))
	{
		printf("Test1");
		return (cd(av, i));
	}
	if (has_pipe && pipe(fd) == -1)
	{
		p_err("err: fatal \n");
		exit(1);
	}
	if ((pid = fork()) == -1)
	{
		p_err("err: fatal \n");
		exit(1);
	}
	if (!pid)
	{
		printf("Test");
		av[i] = 0;
		set_pipe(has_pipe, fd, 1);
		if (strcmp(*av, "cd"))
			exit(cd(av, i));
		execve(*av, av, envp);
		p_err("err: cannot exec ");
		p_err(*av);
		p_err("\n");
		exit(1);
	}
	waitpid(pid, &status, 0);
	set_pipe(has_pipe, fd, 0);
	return (WIFEXITED(status) && WEXITSTATUS(status));
}

int	main(int ac, char **av, char **envp)
{
	int	i;
	int	status;

	(void)ac;
	i = 0;
	status = 0;
	while (av[i])
	{
		av += i + 1;
		i = 0;
		while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;
		if (i)
			status = exec(av, i, envp);
	}
	return (status);
}
