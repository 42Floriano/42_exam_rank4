/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 23:24:50 by albertini         #+#    #+#             */
/*   Updated: 2024/11/21 11:09:15 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int picoshell(char ***ends) {
    int num_commands = 0;

    // Compter le nombre de commandes avec une boucle while
    while (ends[num_commands] != NULL) {
        num_commands++;
    }

    if (num_commands == 0) {
        fprintf(stderr, "Erreur : aucune commande fournie.\n");
        return -1;
    }

    // Création des pipes
    int pipes[num_commands - 1][2];
    int i = 0;

    while (i < num_commands - 1) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return -1;
        }
        i++;
    }

    // Gestion des processus avec une boucle while
    i = 0;
    while (i < num_commands) 
	{
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return -1;
        }

        if (pid == 0) { // Processus enfant
            // Redirection d'entrée pour toutes sauf la première commande
            if (i > 0) {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                    perror("dup2 stdin");
                    exit(EXIT_FAILURE);
                }
            }

            // Redirection de sortie pour toutes sauf la dernière commande
            if (i < num_commands - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                    perror("dup2 stdout");
                    exit(EXIT_FAILURE);
                }
            }

            // Fermer tous les pipes inutiles dans l'enfant
            int j = 0;
            while (j < num_commands - 1) {
                close(pipes[j][0]);
                close(pipes[j][1]);
                j++;
            }

            // Exécuter la commande
            if (execvp(ends[i][0], ends[i]) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }

        i++;
    }

    // Parent : Fermer tous les pipes inutilisés
    i = 0;
    while (i < num_commands - 1) {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }

    // Attendre tous les processus enfants avec une boucle while
    i = 0;
    while (i < num_commands) {
        if (wait(NULL) == -1) {
            perror("wait");
        }
        i++;
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

