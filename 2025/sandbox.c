/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:21:10 by falberti          #+#    #+#             */
/*   Updated: 2025/01/20 15:53:22 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



/*
Assignment name		:	sandbox
Expected files		:	sandbox.c
Allowed functions	:	fork, waitpid, exit, alarm, sigaction, kill,
						printf, strsignal, errno
===============================================================================

Write the following function:

#include <stdbool.h>
int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)

This function must test if the function f is a nice function or a bad function,
you will return 1 if f is nice , 0 if f is bad or -1 in case of an error in
your function.

A function is considered bad if it is terminated or stopped by a signal
(segfault, abort...), if it exit with any other exit code than 0 or if it
times out.

If verbose is true, you must write the appropriate message among the following:

"Nice function!\n"
"Bad function: exited with code <exit_code>\n"
"Bad function: <signal description>\n"
"Bad function: timed out after <timeout> seconds\n"

You must not leak processes (even in zombie state, this will be checked using
wait).
*/

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

int g_pid;
bool g_tout = false;

void alarm_handler(int sig){
    (void)sig;
    g_tout = true;
    kill(g_pid, SIGKILL);
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    struct sigaction sa;
    sa.sa_handler = &alarm_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mark);
    sigaction(SIGALRM, &sa, NULL);
    g_pid = fork();
    if (g_pid = -1) {return (-1);}
    if (g_pid == 0){
        sa.sa_handler = SIG_DFL;
        f();
        exit(0);
    } else {
        alarm(timeout);
        int wstatus;
        if (waitpid(g_pid, &wstatus, 0) == -1){return (-1);} 
        if (g_tout) {
            if (verbose) {printf("Timeouted: %d\n", timeout);} return (1);
        }
         if (WIFEXITED(wstatus)) {
            if(WEXITSTATUS(wstatus) == 0){ if(verbose){printf("Exited normally");}return (0);}
            if(WEXITSTATUS(wstatus)){ if(verbose) {printf("Exited with status %d\n", WEXITSTATUS(wstatus));} return (1);}
        } else if (WIFSIGNALED(wstatus)){
            if (verbose) {printf("Terminated by signal %d", WTERMSIG(wstatus));}
            return (1);
        }
    }
    return (0);
}

void f_test(){
    printf("f_test\n");
    sleep(5);
}

int main(){
    sandbox(f_test, 3, false);
    return 0;
}