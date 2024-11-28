/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lesson_pipies.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:31:12 by albertini         #+#    #+#             */
/*   Updated: 2024/11/28 16:40:17 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

// int	main(int ac, char **av)
// {
// 	int id = fork();
// 	int id2;
// 	if (id != 0)
// 		wait(NULL);
// 	if (id == 0){
// 		printf("Child process\n");
// 		id2 = fork();
// 		wait(NULL);
// 		if (id2 != 0){
// 			printf("Still child \n");
// 		} else {
// 			printf("Grand child process \n");
// 		}
// 	}
// 	if (id != 0)
// 	{
// 		printf("Parent \n");
// 	}
// 	return (0);
// }

// int	main(int ac, char **av)
// {
// 	int id = fork();
// 	if (id != 0)
// 		sleep(1);
// 	printf("Currrent ID: %d - Parent ID: %d\n", getpid(), getppid());
// 	while (wait(NULL) != -1 || errno != ECHILD);
// 	return (0);
// }

// int	main(int ac, char **av)
// {
// 	int fd[2];

// 	if (pipe(fd) == -1){
// 		perror("pipe error");
// 		return (1);
// 	}
// 	int id = fork();
// 	if (id== 0){
// 		int x;
// 		printf("input a number: ");
// 		scanf("%d", &x);
// 		if (write(fd[1], &x, sizeof(int)) == -1)
// 		{
// 			perror("write error!");
// 			return (1);
// 		}
// 		close(fd[1]);
// 		close(fd[0]);
// 	} else {
// 		int y;
// 		if (read(fd[0], &y, sizeof(int))== -1)
// 		{
// 			perror("read error");
// 			return (1);
// 		}
// 		printf("Result from child: %d \n", y);
// 		close(fd[0]);
// 		close(fd[1]);
// 	}
// 	return (0);
// }


// int	main(int ac, char **av)
// {
// 	int p1[2];
// 	int p2[2];
	
// 	if (pipe(p1) == -1) { return 1;}
// 	if (pipe(p2) == -1) { return 1;}

// 	int id = fork();
// 	if (id == -1) { return 2;}
	
// 	if (id == 0)
// 	{
// 		close(p1[0]);
// 		close(p2[1]);
// 		int y;
// 		if (read(p2[0], &y, sizeof(int)) == -1) {return 3;}
// 		y *= 2;
// 		if (write(p1[1], &y, sizeof(int)) == -1){return 3;}
// 		close(p1[1]);
// 		close(p2[0]);
// 	} else {
// 		close(p1[1]);
// 		close(p2[0]);
// 		int x = 10;
// 		if (write(p2[1], &x, sizeof(int)) == -1) {return 3;}
// 		if (read(p1[0], &x, sizeof(int)) == -1) {return 3;}
// 		printf("Test: %d \n", x);
// 		close(p1[0]);
// 		close(p2[1]);
// 	}
// 	return (0);
// }


int ret_err(char *msg, int err)
{
	perror(msg);
	exit(err);
}

int main(int ac, char** av)
{
	char *cmd1[] = {"ls", "-l", NULL};
    char *cmd2[] = {"grep", ".c", NULL};
    char *cmd3[] = {"wc", "-l", NULL};
    char **commands[] = {cmd1, cmd2, cmd3, NULL};

	int p1[2];
	int p2[2];
	if (pipe(p1) == -1) { return 2;}
	if (pipe(p2) == -1) { return 2;}

	int id = fork();
	if (id == -1) {return 3;}
	
	if (id == 0)
	{
		if (execvp(commands[0][0], commands[0]) == -1) {ret_err("exevp error", 5);}
	} else {
		int wstatus;
		wait(&wstatus);
		if (WIFEXITED(wstatus))
		{
		int status_code = WEXITSTATUS(wstatus);
		if (status_code == 0)
			printf("Succes! withcode %d \n", status_code);
		} else {
			printf("Faillure");
		}
	}
	
	return (0);
}