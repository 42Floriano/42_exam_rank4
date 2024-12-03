/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:53:48 by albertini         #+#    #+#             */
/*   Updated: 2024/12/03 17:30:10 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

void handle_alarm(int sig){
	if (sig == SIGALRM){
		printf("Timeout called\n");
		exit(10);
	}
}

void ret_err(char *msg, int err){
	printf("%s", msg);
	exit(err);
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	(void)verbose;
	(void)f;
	int fd = fork();
	if (fd == -1){ret_err("error fork", -1);}
	if (fd == 0){
		struct sigaction sa;
		sa.sa_handler = &handle_alarm;
		sigaction(SIGALRM, &sa, NULL);
		alarm(timeout);
		printf("Child starts a long process\n");
		sleep(10);
		printf("Child finishes a long process\n");
		exit(0);
	}else{
		int wstatus;
		waitpid(-1, &wstatus, 0);
        if (WIFEXITED(wstatus)) {
            printf("Parent process: Child exited with status %d.\n", WEXITSTATUS(wstatus));
        } else if (WIFSIGNALED(wstatus)) {
            printf("Parent process: Child killed by signal %d.\n", WTERMSIG(wstatus));
        }
	}
	return (1);
}

void test(void){
	printf("tes\n");
}

int main (void){
	sandbox(&test, 5, true);
	return (0);
}
