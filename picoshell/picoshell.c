/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 23:24:50 by albertini         #+#    #+#             */
/*   Updated: 2024/11/20 23:25:58 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int picoshell(char ***ends) {
	int num_commands = 0;

	// Compter le nombre de commandes
	while (ends[num_commands] != NULL) {
		num_commands++;
	}
	if (num_commands == 0) {
		fprintf(stderr, "Erreur : aucune commande fournie.\n");
		return -1;
	}
	// Tableau pour les pipes (chaque commande a un pipe sauf la dernière)
	int pipes[num_commands - 1][2];
	// Création des pipes
	for (int i = 0; i < num_commands - 1; i++) {
		if (pipe(pipes[i]) == -1) {
			perror("pipe");
			return -1;
		}
	}
	// Gestion des processus pour chaque commande
	for (int i = 0; i < num_commands; i++) {
		pid_t pid = fork();
		if (pid == -1) {
			perror("fork");
			return -1;
		}
		if (pid == 0) { // Processus enfant
			// Redirection des pipes
			if (i > 0) { // Pas la première commande, redirige l'entrée
				if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
					perror("dup2 stdin");
					exit(EXIT_FAILURE);
				}
			}
			if (i < num_commands - 1) { // Pas la dernière commande, redirige la sortie
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
					perror("dup2 stdout");
					exit(EXIT_FAILURE);
				}
			}
			// Fermer tous les descripteurs inutiles
			for (int j = 0; j < num_commands - 1; j++) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			// Exécuter la commande
			if (execvp(ends[i][0], ends[i]) == -1) {
				perror("execvp");
				exit(EXIT_FAILURE);
			}
		}
	}
	// Parent : fermer tous les descripteurs inutilisés
	for (int i = 0; i < num_commands - 1; i++) {
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	// Attendre tous les processus enfants
	for (int i = 0; i < num_commands; i++) {
		if (wait(NULL) == -1) {
			perror("wait");
		}
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
