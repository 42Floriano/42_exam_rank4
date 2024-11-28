/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 23:24:50 by albertini         #+#    #+#             */
/*   Updated: 2024/11/28 17:37:56 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// Error handling function
int ret_err(char *msg, int err) {
    perror(msg);
    exit(err);
}

int picoshell(char **args[]) {
    int i, nCommands = 0;
    // 1. Count the number of commands (args array ends with NULL)
    while (args[nCommands] != NULL) {
        nCommands++;
    }
    if (nCommands == 0) {   ret_err("No commands provided", -1);}
    // 2. Create pipes
    int pipes[nCommands - 1][2];
    for (i = 0; i < nCommands - 1; i++) {
        if (pipe(pipes[i]) == -1) { ret_err("pipe", EXIT_FAILURE);}
    }
    // 3. Fork processes for each command
    for (i = 0; i < nCommands; i++) {
        int pid = fork();
        if (pid == -1) { ret_err("fork", EXIT_FAILURE);}
        if (pid == 0) { // Child process
            // 4. Redirect stdin/stdout as needed
            if (i > 0) { // Not the first command: read from previous pipe
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {ret_err("dup2 input", EXIT_FAILURE);}
            }
            if (i < nCommands - 1) { // Not the last command: write to next pipe
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) { ret_err("dup2 output", EXIT_FAILURE); }
            }

            // 4.5 Close all pipe ends not needed by this child
            for (int j = 0; j < nCommands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // 5. Execute the command
            if (execvp(args[i][0], args[i]) == -1) { ret_err("execvp", EXIT_FAILURE); } // If execvp fails
        }
    }

    // 6. Parent process closes all pipe ends
    for (i = 0; i < nCommands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // 7. Wait for all child processes
        if (wait(NULL) == -1 || errno == ECHILD) {
            ret_err("wait", EXIT_FAILURE);
        }
    
    return 0;
}

// Exemple d'utilisation
int main() {
    // Exemple : ls | grep .c | wc -l
    char *cmd1[] = {"ls", NULL};
    char *cmd2[] = {"grep", ".c", NULL};
    char *cmd3[] = {"wc", "-l", NULL};
    char **commands[] = {cmd1, cmd2, cmd3, NULL};

    // Appeler picoshell
    if (picoshell(commands) == -1) {
        fprintf(stderr, "Erreur dans l'exécution de picoshell.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

