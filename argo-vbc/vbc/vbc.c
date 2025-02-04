
//This file is given at the exam

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int g_open = 0;
int g_close = 0;

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

// int accept(char **s, char c)
// {
//     if (**s)
//     {
//         (*s)++;
//         return (1);
//     }
//     return (0);
// }

// int expect(char **s, char c)
// {
//     if (accept(s, c))
//         return (1);
//     unexpected(**s);
//     return (0);
// }

//...

// node    *parse_expr(char **av)
// {
//     node *left = NULL;
//     while (**av){
//         printf("Test: %c\n", **av);
//         if (**av == ')' && g_open == 0){
//             g_close = 1;
//             return NULL;
//         }
//         if(**av == '('){
//             g_open++;
//             (*av)++;
//             node *sub_n = parse_expr(av);
//             if (**av != ')'){
//                 return NULL;
//                 }
//             (*av)++;
//             g_open--;
//             if (left == NULL)
//                 left = sub_n;
//         } else if (**av >= '0' && **av <= '9'){
//             node n = {.type = VAL, .val = **av - '0'};
//             (*av)++;
//             if (left == NULL)
//                 left = new_node(n);
//         } else if (**av == '*') {
//             (*av)++;
//             node *r = parse_expr(av);
//             node n = {.type = MULTI, .l = left, .r = r};
//             left = new_node(n);
//         } else if (**av == '+'){
//             (*av)++;
//             node *r = parse_expr(av);
//             node n = {.type = ADD, .l = left, .r = r};
//             left = new_node(n);
//         } else {
//             break ;
//         }
//     }
//     return (left);
// }

node *parse_expr(char **av){
    node *left = NULL;
    while (**av){
        if (**av == ')' && g_open == 0){
            g_close = 1;
            return NULL;
        }
        if (**av == '('){
            g_open++;
            (*av)++;
            node *sub_n = parse_expr(av);
            if (**av != ')')
                return NULL;
            (*av)++;
            g_open--;
            if (left == NULL)
                left = sub_n;
        } else if (**av >= '0' && **av <= '9'){
            node n = {.type = VAL, .val = **av - '0'};
            (*av)++;
            if (left == NULL)
                left = new_node(n);
        } else if (**av == '*'){
            (*av)++;
            node *r = parse_expr(av);
            if (r ==  NULL){
                return NULL;
            }
            node n = {.type = MULTI, .l = left, .r = r};
            left = new_node(n);
        } else if (**av == '+'){
            (*av)++;
            node *r = parse_expr(av);
            if (r ==  NULL){
                return NULL;
            }
            node n = {.type = ADD, .l = left, .r = r};
            left = new_node(n);
        } else if (**av == ')'){
            break;
        } else {
            return NULL;
        }
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
        return (1);
    (argv)++;
    printf("Test: %s\n", *argv);
    node *tree = parse_expr(argv);
    if (g_close || g_open){
        unexpected(g_open ? '(' : ')');
        return (1);
    }
    if (!tree){
            unexpected(**argv ? **argv : 0);
        return (1);
    }
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}