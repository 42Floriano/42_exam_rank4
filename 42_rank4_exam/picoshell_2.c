/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:20:42 by falberti          #+#    #+#             */
/*   Updated: 2024/12/10 15:53:43 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

void ret_err(char *msg, int err)
{
    perror(msg);
    exit(err);
}

int picoshell(char **cmds[])
{
    int nCmd = 0, i = 0;
    while(cmds[nCmd])
        nCmd++;
    if (nCmd == 0){ret_err("Enter at least one command!", -1);}
    int pipes[nCmd - 1][2];
    for (i = 0; i < nCmd - 1; i++){
        if(pipe(pipes[i]) == -1){ret_err("error pipe", -1);}
    }
    for (i = 0; i < nCmd; i++){
        int pid = fork();
        if (pid == -1){ret_err("error pid", -1);}
        if (pid == 0){
            if (i > 0){
                dup2(pipes[i - 1][0], 0);
            }
            if (i < nCmd - 1){
                dup2(pipes[i][1], 1);
            }
            for(int y = 0; y < nCmd - 1; y++){
                close(pipes[y][0]);
                close(pipes[y][1]);
            }
            execvp(cmds[i][0], cmds[i]);
        }
    }
    for(i = 0; i < nCmd; i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    for(i = 0; i < nCmd; i++){
        if (wait(NULL) == -1){ret_err("error wait", -1);}
    }
    return(0);
}


int main() {
	char *cmd1[] = {"ls", "-l", NULL};
	char *cmd2[] = {"grep", ".c", NULL};
	char *cmd3[] = {"wc", "-l", NULL};
	char **commands[] = {cmd1, cmd2, cmd3,  NULL};

	picoshell(commands);

	return 0;
}
