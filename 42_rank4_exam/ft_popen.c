/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:15:48 by floriano          #+#    #+#             */
/*   Updated: 2024/12/12 15:09:16 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int g_pdr = 0;


int ft_popen(const char *file, char *const av[], char type){
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
    if (pid == 0)
    {
       if(type == 'r'){
           dup2(pipes[1], 1);
        } else if (type == 'w'){
            dup2(pipes[0], 0);
        }
        close(pipes[0]);
        close(pipes[1]);
        if (execvp(file, av) == -1){return (-1);}
    } else      
        type == 'r' ? close(pipes[1]) : close(pipes[0]);
    return (type == 'r' ? pipes[0] : pipes[1]);
}

#include<string.h>
/*
int main(int ac, char **av)
{
    if (ac == 1)
        return (1);
    int fd = ft_popen(av[1], av + 1, 'r');
    
    while (1)
    {
            char    buff[1024 + 1];

            int len = read(fd, buff, 1024);
            buff[len] = 0;
            if (!len)
                break;
            printf("%s", buff);
        
    }
    return (0);
}
*/


// int main(int ac, char **av)
// {
//     if (ac == 1)
//         return (1);
//     // Creates a writable fd, piped to stdin of av[1].
//     int fd = ft_popen(av[1], av + 1, 'w');
    
//     while (1)// Pipe main-process (this) stdin to above writable fd;
//     {
//             char    buff[1024 + 1];

//             int len = read(0, buff, 1024);
//             buff[len] = 0;
//             if (!len)
//                 break;
//             write(fd, buff, len);        
//     }
//     return (0);
// }