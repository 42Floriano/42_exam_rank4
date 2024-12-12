/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:14:10 by albertini         #+#    #+#             */
/*   Updated: 2024/12/12 13:56:27 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		ret_err("Enter a least one commands", 1);
		
	int pipes[nComm - 1][2];
	
	for (i = 0; i < nComm - 1; i++){
		if (pipe(pipes[i]) == -1) {ret_err("Error pipes", 1);}
	}
	for (i = 0; i < nComm; i++){
		int pid = fork();
		if (pid == -1){ret_err("Error fork", 1);}
		if (pid == 0){
		if (i > 0){
			if(dup2(pipes[i - 1][0], 0) == -1){ret_err("Error out dup2", 1);}
		}
		if (i < nComm - 1){
			if(dup2(pipes[i][1], 1) == -1){ret_err("Error in dup2", 1);}
		}
		int y;	
		for (y = 0; y < nComm - 1; y++){
			if (close(pipes[y][0]) == -1){ret_err("Error close", 1);}
			if (close(pipes[y][1]) == -1){ret_err("Error close", 1);}
		}
		execvp(cmds[i][0], cmds[i]);
		}
		
	}
	for (i = 0; i < nComm - 1; i++){
		if (close(pipes[i][0]) == -1){ret_err("Error close", 1);}
		if (close(pipes[i][1]) == -1){ret_err("Error close", 1);}
	}
	for (i = 0; i < nComm; i++){
		if (wait(NULL) == -1){ret_err("Error wait", 1);}
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
