/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:04:01 by albertini         #+#    #+#             */
/*   Updated: 2024/11/20 16:21:04 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

// Prototypes
int create_pipe(int pipes[2]);
void close_pipe(int pipes[2]);
void setup_redirections(int i, int pipes[2][2], int has_next, int has_prev);
void execute_command(char **command);
void wait_for_children();

int picoshell(char ***ends[]) {
    int i = 0;
    int pipes[2][2]; // Alternating pipes
    pid_t pid;

    while ((*ends)[i] != NULL) {
        int has_next = ((*ends)[i + 1] != NULL);
        int has_prev = (i > 0);

        // Create a pipe if not the last command
        if (has_next && create_pipe(pipes[i % 2]) != 0) {
            return 1;
        }

        // Fork a new process
        pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) { // Child process
            setup_redirections(i, pipes, has_next, has_prev);
            execute_command((*ends)[i]);
        }

        // Parent process: Close unused pipes
        if (has_prev) {
            close_pipe(pipes[(i + 1) % 2]);
        }
        if (has_next) {
            close(pipes[i % 2][1]); // Close write end of the current pipe
        }

        i++;
    }

    // Wait for all children to finish
    wait_for_children();

    return 0;
}

// Function to create a pipe
int create_pipe(int pipes[2]) {
    if (pipe(pipes) == -1) {
        perror("pipe");
        return 1;
    }
    return 0;
}

// Function to close a pipe
void close_pipe(int pipes[2]) {
    close(pipes[0]);
    close(pipes[1]);
}

// Function to set up redirections for a command
void setup_redirections(int i, int pipes[2][2], int has_next, int has_prev) {
    if (has_next) { // Redirect stdout to the pipe's write end
        if (dup2(pipes[i % 2][1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            exit(1);
        }
        close_pipe(pipes[i % 2]);
    }

    if (has_prev) { // Redirect stdin to the previous pipe's read end
        if (dup2(pipes[(i + 1) % 2][0], STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            exit(1);
        }
        close_pipe(pipes[(i + 1) % 2]);
    }
}

// Function to execute a command
void execute_command(char **command) {
    execvp(command[0], command);
    perror("execvp");
    exit(1);
}

// Function to wait for all children to finish
void wait_for_children() {
    while (waitpid(-1, NULL, 0) > 0);
}
