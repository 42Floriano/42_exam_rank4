/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 20:21:01 by albertini         #+#    #+#             */
/*   Updated: 2024/11/28 21:53:00 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>

void	ret_err(char *msg, int err)
{
	perror(msg);
	exit(err);
}

int picoshell(char **cmds[])
{
	int nCommand = 0, i = 0;
	while (cmds[nCommand])
		nCommand++;
	if (nCommand == 0) {ret_err("No commands passed", 1);}
	int pipes[nCommand - 1][2];
	for (i = 0; i < nCommand - 1; i++){
		if (pipe(pipes[i]) == -1){ ret_err("Pipe error", 1);}
	}
	//Create all forks + dup2 + close all pipes in children
	for (i = 0; i < nCommand; i ++){
		int pid = fork();
		if (pid == -1){ret_err("PID error", 1);}
		if (pid == 0){
			if(i > 0){
				if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1){ret_err("Dup2 in error", 1);}
			}
			if(i < nCommand - 1){
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1){ret_err("Dup2 out error", 1);}
			}
			for (int y = 0; y < nCommand - 1; y++){
				close(pipes[y][0]);
				close(pipes[y][1]);
			}
			if (execvp(cmds[i][0], cmds[i]) == -1){ret_err("Execvp error", 1);}
	}
	}
	//Close all parent pipes
	for (i = 0; i < nCommand - 1; i++){
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	//wait all chidren to finish and check return error in children process
	int wstatus;
	for (i = 0; i < nCommand; i++) {
    if (wait(&wstatus) == -1) { ret_err("Wait error", 1); }
	if(WIFEXITED(wstatus)){
		int nstatus = WEXITSTATUS(wstatus);
		if (nstatus == 0){}
		else 
			printf("Faillure, code: %d", nstatus);
	}
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
