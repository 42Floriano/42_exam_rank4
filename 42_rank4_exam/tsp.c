/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:45:42 by falberti          #+#    #+#             */
/*   Updated: 2024/12/12 15:08:55 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include <float.h>

double	calculate_distance(double x0, double x1, double y0, double y1)
{
	double res;
	double	dx = x1 - x0;
	double	dy = y1 - y0;
	res = sqrt(dx * dx + dy * dy);
	return (res);
}

void	swap(int *a, int *b)
{
	if (b != a)
	{
		*a = *a ^ *b;
		*b = *a ^ *b;
		*a = *a ^ *b;
	}
}

void	find_shortest_path(int *perm, int start, int len, double x[], double y[], double *min_distance)
{
	int	i;
	double	total_distance;
	total_distance = 0;
	if (start == len - 1)
	{
		i = 0;
		while (i < len - 1)
		{
			total_distance += calculate_distance(x[perm[i]], x[perm[i + 1]], y[perm[i]], y[perm[i + 1]]);
			i++;
		}
		total_distance += calculate_distance(x[perm[len - 1]], x[perm[0]], y[perm[len - 1]], y[perm[0]]);
		if (total_distance < *min_distance)
			*min_distance = total_distance;
	}
	else
	{
		i = start;
		while (i < len)
		{
			swap(&perm[start], &perm[i]);
			find_shortest_path(perm, start + 1, len, x, y, min_distance);
			swap(&perm[start], &perm[i]);
			i++;
		}
	}
}

int	main()
{
	int		len;
	int		perm[11];
	double	x[11];
	double	y[11];
	double	min_distance;
	len = 0;
	while (fscanf(stdin, "%lf, %lf", &x[len], &y[len]) == 2)
	{
		perm[len] = len;
		len++;
	}
	min_distance = DBL_MAX;
	find_shortest_path(perm, 0, len, x, y, &min_distance);
	printf("shortest total distance is: %.2f\n", min_distance);
	return (0);
}
