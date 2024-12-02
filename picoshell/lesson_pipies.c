/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lesson_pipies.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:31:12 by albertini         #+#    #+#             */
/*   Updated: 2024/12/01 13:17:38 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

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


// int ret_err(char *msg, int err)
// {
// 	perror(msg);
// 	exit(err);
// }

// int main(int ac, char** av)
// {
// 	int pid = fork();
// 	if (pid == -1) {ret_err("pidproblem", 1);}
// 	if (pid == 0){
// 		while (1){
// 			printf("I'm an infinite loop!\n");
// 		}
// 	} else {
// 		kill(pid, SIGSTOP);
// 		sleep(5);
// 		kill(pid, SIGCONT);
// 		sleep(2);
// 		kill(pid, SIGABRT);
// 	}
// 	return (0);
// }

int ret_err(char *msg, int err)
{
	perror(msg);
	exit(err);
}

void handle_stop(int sig)
{
	printf("Stop process not allowed!");
}

int main(int ac, char** av)
{
	struct sigaction sa;
	sa.sa_handler = &handle_stop;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGTSTP, &sa, NULL);
	int x;
	printf("Choose a number to multiply by 5: ");
	scanf("%d", &x);
	printf("The result of %d by 5 is: %d", x, x * 5);
	return (0);
}