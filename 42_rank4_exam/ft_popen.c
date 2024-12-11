/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: floriano <floriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:15:48 by floriano          #+#    #+#             */
/*   Updated: 2024/12/11 16:49:53 by floriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int g_pdr = 0;

int ft_popen(const char *file, char *const av[], char type)
{
    int pipes[2];
    if (type != 'r' && type != 'w')
        return (-1);
    if (pipe(pipes) == -1){return (-1);}
    int pid = fork();
    if (pid == -1){
        close(pipes[0]);
        close(pipes[1]);
        return (-1);
        }
    if (pid == 0){
        if(type == 'r'){
           g_pdr = dup2(pipes[1], 1);
        } else if (type == 'w'){
            g_pdr = dup2(pipes[0], 0);
        }
        close(pipes[0]);
        close(pipes[1]);
        if (execvp(file, av) == -1){return (-1);}
    } else {
        close(pipes[0]);
        close(pipes[1]);
        if (g_pdr == -1){return (-1);}
    }
    return (g_pdr);
}
