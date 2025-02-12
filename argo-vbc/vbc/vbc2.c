/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: floriano <floriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:50:59 by floriano          #+#    #+#             */
/*   Updated: 2025/02/12 15:11:18 by floriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int g_open = 0;
int g_close = 0;
char *input = NULL;


typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node *parse_factor();
node *parse_term();
node *parse_expr();

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of file\n");
}


node *parse_factor(){
    node *left = NULL;
    if(*input == '('){
        input++;
        node *sub_n = parse_expr();
        if (*input != ')') unexpected(*input);
        input++;
        return sub_n;
    } else if (isdigit(*input)){
        node n = {.type = VAL, .val = *input - '0'};
        input++;
        left = new_node(n);
        return (left);
    }
    unexpected(*input);
    return (left);
}

node *parse_term(){
    node *left = parse_factor();
    while (*input == '*'){
        input++;
         // 🛑 Check if the next character is an invalid operator
        if (*input == '+' || *input == '*' || *input == '\0' || *input == ')') {
            unexpected(*input);
            return NULL;
        }
        node *r = parse_factor();
        node n = {.type = MULTI, .l = left, .r = r};
        left = new_node(n);
    }
    return (left);
}

node *parse_expr(){
    node *left = parse_term();
    while (*input == '+'){
        input++;
         // 🛑 Check if the next character is an invalid operator
        if (*input == '+' || *input == '*' || *input == '\0' || *input == ')') {
            unexpected(*input);
            return NULL;
        }
        node *r = parse_term();
        node n = {.type = ADD, .l = left, .r = r};
        left = new_node(n);
    }
    return (left);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);\
    input = argv[1];
    printf("Test: %s\n", input);
    node *tree = parse_expr();
    if (*input){
            //unexpected(*input);
        return (1);
    }
    if (tree == NULL) return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}