/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: floriano <floriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:18:47 by falberti          #+#    #+#             */
/*   Updated: 2025/02/19 09:28:30 by floriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Allowed functions: pipe, fork, dup2, execvp, close, exit

write the following function:
	
	int	ft_popen(const char *file, char *const argv[], char type)

The function must launch the executable file with the arguments argv (using execvp).
If the type is 'r' the function must return a file descriptor connected to the output of the command.
If the type is 'w' the function must return a file descriptor connected to the input of the command.
In case of error or invalid parameter the function must return -1.

example:

int main() {
	int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');

	char *line;
	while(line = get_next_line(fd))
		ft_putstr(line);
}

Hint: Do not leak file descriptors! */

// r = out w = in

#include <unistd.h>
#include <stdlib.h>


int	ft_popen(const char *file, char *const argv[], char type)
{
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    int fd[2];
    pipe(fd);
    if (type = 'r'){
        if (fork() == 0){
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execvp(file, argv);
            exit(-1);
        }
        close(fd[1]);
        return(fd[0]);
    }
    if (type = 'w'){
        if (fork() == 0){
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            execvp(file, argv);
            exit(-1);
        }
        close(fd[0]);
        return(fd[1]);
    }
    exit (-1);
}


// Two mains for testing
// #include<string.h>
// int main(int ac, char **av)
// {
//     if (ac == 1)
//         return (1);
//     int fd = ft_popen(av[1], av + 1, 'r');
    
//     while (1)
//     {
//             char    buff[1024 + 1];

//             int len = read(fd, buff, 1024);
//             buff[len] = 0;
//             if (!len)
//                 break;
//             printf("%s", buff);
        
//     }
//     return (0);
// }


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