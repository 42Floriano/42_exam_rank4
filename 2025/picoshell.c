/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: floriano <floriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:14:10 by albertini         #+#    #+#             */
/*   Updated: 2025/01/22 15:08:15 by floriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Allowed functions:
pipe, fork, dup2, execvp, close, exit, wait

Write the following function:

	int picoshell(char **cmds[])

It has to simulate the pipe. cmds[i] contains the command with its arguments.
Use execvp(cmds[i][0], cmds) to execute a command.
It has to return 1, if a command fail. Do not leak file descriptors.
In the exam, a main file will be given to test your picoshell
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

void	ret_err(char *msg, int err){
	perror(msg);
	exit(err);
}

int picoshell(char **cmds[])
{
	int i = 0, nComm = 0;
	while (cmds[nComm])
		nComm++;
	if (nComm < 1)
		exit(1);
		
	int pipes[nComm - 1][2];
	
	for (i = 0; i < nComm - 1; i++){
		if (pipe(pipes[i]) == -1) {exit(1);}
	}
	for (i = 0; i < nComm; i++){
		int pid = fork();
		if (pid == -1){exit(1);}
		if (pid == 0){
		if (i > 0){
			if(dup2(pipes[i - 1][0], 0) == -1){
				for (i = 0; i < nComm - 1; i++){
					if (close(pipes[i][0]) == -1){exit(1);}
					if (close(pipes[i][1]) == -1){exit(1);}
				}
				exit(1);
			}
		}
		if (i < nComm - 1){
			if(dup2(pipes[i][1], 1) == -1){
				for (i = 0; i < nComm - 1; i++){
					if (close(pipes[i][0]) == -1){exit(1);}
					if (close(pipes[i][1]) == -1){exit(1);}
				}
				exit(1);
			}
		}
		int y;	
		for (y = 0; y < nComm - 1; y++){
			if (close(pipes[y][0]) == -1){exit(1);}
			if (close(pipes[y][1]) == -1){exit(1);}
		}
		execvp(cmds[i][0], cmds[i]);
		exit(1);
		}
		
	}
	for (i = 0; i < nComm - 1; i++){
		if (close(pipes[i][0]) == -1){exit(1);}
		if (close(pipes[i][1]) == -1){exit(1);}
	}
	for (i = 0; i < nComm; i++){
		if (wait(NULL) == -1){exit(1);}
	}
	return (0);
}

int main() {
	char *cmd1[] = {"ls", "-l", NULL};
	char *cmd2[] = {"grep", ".c", NULL};
	char *cmd3[] = {"wc", "-l", NULL};
	char **commands[] = {cmd1, cmd2, cmd3,  NULL};

	picoshell(commands);

	return 0;
}
