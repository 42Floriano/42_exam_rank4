/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calculator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falberti <falberti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 15:56:38 by falberti          #+#    #+#             */
/*   Updated: 2024/12/12 17:07:44 by falberti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct t_data {
    int g_pos;
    char *g_input;
    int o_par;
    int c_par;
} s_data;

s_data g_data;

void   init_data(){
    g_data.g_pos = 0;
    g_data.g_input = NULL;
    g_data.o_par = 0;
    g_data.c_par = 0;
}

int check_par(char *str){
    int i = 0;
    while(str[i]){
        if (str[i] == '(')
            g_data.o_par += 1;
        if (str[i] == ')')
            g_data.c_par += 1;
        i++;
    }
    return g_data.o_par == g_data.c_par ? 1 : 0;
}

char *rm_space(char *str)
{
    int i = 0, y = 0;
    char *ret = malloc(sizeof(char) * strlen(str) + 1);
    if (ret == NULL)
        return NULL;
    while(str[i])
    {
        while (isspace(str[i]) && str[i])
            i++;
        while(str[i] && !isspace(str[i])){
            ret[y] = str[i];
            y++;
            i++;
        }
    }
    ret[y] = '\0';
    return (ret);
}

double rec(char *str){
    double res;
    return res;
}

double calculator(char *str){
    double res = 0;
    init_data();
    printf("%s\n", str);
    if (check_par(str)){g_data.g_input = rm_space(str);}
    printf("%s\n", g_data.g_input);
    return (res);
}

int main (int ac, char **av)
{
    double res;
    if (ac != 2)
        return (1);
    res = calculator(av[1]);
    printf("%lf", res);
    return (0);
}