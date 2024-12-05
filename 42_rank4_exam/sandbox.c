/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:53:48 by albertini         #+#    #+#             */
/*   Updated: 2024/12/05 12:38:05 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

// void handle_alarm(int sig){
// 	if (sig == SIGALRM){
// 		printf("Timeout called\n");
// 		exit(10);
// 	}
// }

void ret_err(char *msg, int err){
	printf("%s", msg);
	exit(err);
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	int fd = fork();
	if (fd == -1){return (-1);}
	if (fd == 0){
		// struct sigaction sa;
		// sa.sa_handler = &handle_alarm;
		// sigaction(SIGALRM, &sa, NULL);
		alarm(timeout);
		f();
		//sleep(10);
	}else{
		int wstatus;
		if (waitpid(-1, &wstatus, 0) == -1) {return (-1);};
        if (WIFEXITED(wstatus)) {
			if(WEXITSTATUS(wstatus) == 0){
				if (verbose){printf("Nice function");}
				return (1);
			} else {
				if (verbose){printf("Exited with exit %d", WEXITSTATUS(wstatus));}
				return (0);
			}
        } else if (WIFSIGNALED(wstatus)) {
			if ((WTERMSIG(wstatus) == 14) && verbose){
				printf("Timeout after %d seconds", timeout);
			} else if (verbose){
				printf("Parent process: Child killed by signal %d.\n", WTERMSIG(wstatus));
			}
			return (0);
        }
	}
	return (1);
}

void test(void){
	printf("test function\n");
}

int main (void){
	sandbox(&test, 5, true);
	return (0);
}
