/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertini <albertini@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 23:08:12 by albertini         #+#    #+#             */
/*   Updated: 2024/11/20 23:10:28 by albertini        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "picoshell_multi.c"

int	main(void)
{
	char *cmd1[] = {"ls", "-l", NULL};
	char *cmd2[] = {"grep", ".c", NULL};
	char *cmd3[] = {"wc", "-l", NULL};
	char **ends[] = {cmd1, cmd2, cmd3, NULL};

	if (picoshell(&ends) != 0)
	{
		fprintf(stderr, "picoshell failed\n");
		return (1);
	}

	return (0);
}
