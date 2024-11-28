/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lesson_pipies.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:31:12 by albertini         #+#    #+#             */
/*   Updated: 2024/11/28 12:08:25 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

// int	main(int ac, char **av)
// {
// 	int id = fork();
// 	int id2;
// 	if (id != 0)
// 		wait(NULL);
// 	if (id == 0){
// 		printf("Child process\n");
// 		id2 = fork();
// 		wait(NULL);
// 		if (id2 != 0){
// 			printf("Still child \n");
// 		} else {
// 			printf("Grand child process \n");
// 		}
// 	}
// 	if (id != 0)
// 	{
// 		printf("Parent \n");
// 	}
// 	return (0);
// }

int	main(int ac, char **av)
{
	int id = fork();
	if (id != 0)
		sleep(1);
	printf("Currrent ID: %d - Parent ID: %d\n", getpid(), getppid());
	wait(NULL);
	return (0);
}