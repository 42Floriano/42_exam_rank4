/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   popen2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:18:47 by falberti          #+#    #+#             */
/*   Updated: 2025/01/21 16:00:07 by falberti         ###   ########.fr       */
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
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

int	ft_popen(const char *file, char *const argv[], char type){
    if (!file || !argv || (type != 'r' && type != 'w'))
        return (-1);
    int fd[2];
    if (pipe(fd) == -1){ return (-1);}
    if (type == 'r'){
        if (fork() == 0){
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execvp(file, argv);
            exit(-1);
        }
        close(fd[1]);
        return (fd[0]);        
    }
    if (type == 'w'){
        if (fork() == 0){
            dup2(fd[0], 0);
            close(fd[1]);
            close(fd[0]);
            execvp(file, argv);
            exit(-1);
        }
        close(fd[0]);
        return (fd[1]);
    }
    return (0);
}

// // w test
// int main() {
//     const char *cmd = "/bin/ls";
//     char *const args[] = { "ls", NULL };

//     int fd = ft_popen(cmd, args, 'r');
//     if (fd == -1) {
//         perror("ft_popen failed");
//         return 1;
//     }

//     char buffer[1024];
//     ssize_t bytes;
//     while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
//         buffer[bytes] = '\0';
//         printf("%s", buffer);
//     }
//     close(fd);

//     return 0;
// }


// // r test
// int main() {
//     const char *cmd = "/usr/bin/find";
//     char *const args[] = { "find", "/", "-maxdepth", "1", NULL };

//     int fd = ft_popen(cmd, args, 'r');
//     if (fd == -1) {
//         perror("ft_popen failed");
//         return 1;
//     }

//     char buffer[4096];
//     ssize_t bytes;
//     while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
//         buffer[bytes] = '\0';
//         printf("%s", buffer);
//     }
//     close(fd);

//     return 0;
// }

// //Error main
// int main() {
//     const char *cmd = "/bin/nonexistent";  // Invalid command
//     char *const args[] = { "nonexistent", NULL };

//     int fd = ft_popen(cmd, args, 'r');
//     if (fd == -1) {
//         printf("Test passed: Handled nonexistent command properly\n");
//     } else {
//         printf("Test failed: Did not handle nonexistent command\n");
//     }

//     return 0;
// }
