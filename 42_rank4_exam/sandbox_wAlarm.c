/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox_wAlarm.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:10:53 by falberti          #+#    #+#             */
/*   Updated: 2024/12/10 17:38:16 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal.h"
#include "unistd.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/wait.h"
#include "stdlib.h"

int g_pid;
bool g_tout = false;

void alarm_handler(int sig)
{
    (void)sig;
    g_tout = true;
    kill(g_pid, SIGKILL);
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    struct sigaction sa;
    sa.sa_handler = &alarm_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    g_pid = fork();
    if (g_pid == -1) {return (-1);}
    if (g_pid == 0){
        sa.sa_handler = SIG_DFL;
        f();
        exit(0);
    } else {
        alarm(timeout);
        int wstatus;
        if (waitpid(g_pid, &wstatus, 0) == -1){return (-1);}
        if (g_tout)
        {
            if(verbose) {printf("Timeouted: %d\n", timeout);} return (1);
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

void f_test()
{
    printf("F_test called \n");
    sleep(10);
}

int main()
{
    return(sandbox(&f_test, 2, true));
}