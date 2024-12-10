/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   popen_gpt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:33:43 by falberti          #+#    #+#             */
/*   Updated: 2024/12/10 15:33:45 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ  0
#define WRITE 1

FILE *custom_popen(const char *command, const char *mode) {
    int pipefd[2];
    pid_t pid;
    FILE *stream = NULL;

    // Check for valid mode
    if (strcmp(mode, "r") != 0 && strcmp(mode, "w") != 0) {
        fprintf(stderr, "Invalid mode: use 'r' or 'w'\n");
        return NULL;
    }

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return NULL;
    }

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[READ]);
        close(pipefd[WRITE]);
        return NULL;
    }

    if (pid == 0) { // Child process
        if (strcmp(mode, "r") == 0) {
            // Reading: close read end, redirect stdout to pipe
            close(pipefd[READ]);
            dup2(pipefd[WRITE], STDOUT_FILENO);
            close(pipefd[WRITE]);
        } else {
            // Writing: close write end, redirect stdin to pipe
            close(pipefd[WRITE]);
            dup2(pipefd[READ], STDIN_FILENO);
            close(pipefd[READ]);
        }

        // Execute the command
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        perror("execl");
        exit(EXIT_FAILURE); // Exit if execl fails
    } else { // Parent process
        if (strcmp(mode, "r") == 0) {
            // Reading: close write end, return read stream
            close(pipefd[WRITE]);
            stream = fdopen(pipefd[READ], "r");
        } else {
            // Writing: close read end, return write stream
            close(pipefd[READ]);
            stream = fdopen(pipefd[WRITE], "w");
        }

        if (!stream) {
            perror("fdopen");
            close(pipefd[READ]);
            close(pipefd[WRITE]);
            return NULL;
        }
    }

    return stream;
}

int custom_pclose(FILE *stream) {
    int status;
    pid_t pid;

    // Get the process ID linked to the stream
    pid = waitpid(-1, &status, 0);

    if (pid == -1) {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status); // Return exit status of child
    } else {
        return -1; // Process did not exit normally
    }
}

int main() {
    char buffer[128];
    FILE *fp;

    // Example: Read output of "ls" command
    fp = custom_popen("ls", "r");
    if (!fp) {
        fprintf(stderr, "Failed to open pipe.\n");
        return EXIT_FAILURE;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }

    custom_pclose(fp);
    return 0;
}