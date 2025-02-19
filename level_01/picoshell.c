/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: floriano <floriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:14:10 by albertini         #+#    #+#             */
/*   Updated: 2025/01/22 16:42:00 by floriano         ###   ########.fr       */
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

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int picoshell(char **cmds[]){
    int nComm = 0, i = 0;
    while(cmds[nComm]){nComm++;}
    if (nComm < 1){printf("No command"); exit(1);}
    int fd[nComm - 1][2];
    for (i = 0; i < nComm - 1; i++){pipe(fd[i]);}
    for (i = 0; i < nComm; i++){
        if (fork() == 0){
            if(i > 0){
                if (dup2(fd[i - 1][0], 0) == -1){
                    for (int y = 0; y < nComm - 1; y++){
                        close(fd[y][0]);
                        close(fd[y][1]);
                     }
                    exit(1);
                }
            }
            if (i < nComm - 1){
                if (dup2(fd[i][1], 1) == -1){
                    for (int y = 0; y < nComm - 1; y++){
                        close(fd[y][0]);
                        close(fd[y][1]);
                     }
                    exit(1);
                }
            }
            for (int y = 0; y < nComm - 1; y++){
            close(fd[y][0]);
            close(fd[y][1]);
            }
            if (execvp(cmds[i][0], cmds[i]) == -1){printf("error exec"); exit(1);}
        }

    }
    for (i = 0; i < nComm - 1; i++){
        close(fd[i][0]);
        close(fd[i][1]);
        }
    for (i = 0; i < nComm; i++){wait(NULL);}
    return (0);
}
// Main for test
// int main() {
//     // Test 1: Simple pipeline (ls | grep "test")
//     char *cmd1[] = {"ls", NULL};
//     char *cmd2[] = {"grep", "test", NULL};
//     char **commands1[] = {cmd1, cmd2, NULL};
//     printf("Test 1: ls | grep \"test\"\n");
//     picoshell(commands1);

//     // Test 2: Single command (echo "hello")
//     char *cmd3[] = {"echo", "hello", NULL};
//     char **commands2[] = {cmd3, NULL};
//     printf("Test 2: echo \"hello\"\n");
//     picoshell(commands2);

//     // Test 3: Invalid command
//     char *cmd4[] = {"invalidcommand", NULL};
//     char **commands3[] = {cmd4, NULL};
//     printf("Test 3: Invalid Command\n");
//     picoshell(commands3);

//     // Test 4: Long pipeline (echo "hello" | grep "h" | grep "e")
//     char *cmd5[] = {"echo", "hello", NULL};
//     char *cmd6[] = {"grep", "h", NULL};
//     char *cmd7[] = {"grep", "e", NULL};
//     char **commands4[] = {cmd5, cmd6, cmd7, NULL};
//     printf("Test 4: echo \"hello\" | grep \"h\" | grep \"e\"\n");
//     picoshell(commands4);

//     return 0;
// }
