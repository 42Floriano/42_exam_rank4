/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:21:13 by falberti          #+#    #+#             */
/*   Updated: 2025/01/20 14:21:14 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
Allowed functions:
pipe, fork, dup2, execvp, close, exit, wait

Write the following function:

	int picoshell(char **cmds[])

It has to simulate the pipe. cmds[i] contains the command with its arguments.
Use execvp(cmds[i][0], cmds) to execute a command.
It has to return 1, if a command fail. Do not leak file descriptors.
In the exam, a main file will be given to test your picoshell
*/