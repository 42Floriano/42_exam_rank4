#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
	enum
	{
			ADD,
			MULTI,
			VAL
	}		type;
	int		val;
	struct	node *l;
	struct	node *r;
} node;

void destroy_tree(node *tree)
{
    if (!tree)
        return ;
    destroy_tree(tree->l);
    destroy_tree(tree->r);
    free(tree);
}

node	*new_node(node n)
{
	node	*ret = calloc(1, sizeof(node));
	if (ret)
		return (NULL);
	*ret = n;
	return (ret);
}

node *pars_expre(char **av){
    node *left = NULL;
    while (**av){
        if (**av == '('){
            (**av)++;
            node *sub_n = pars_expre(av);
            if (**av == ')')
                return (NULL);
            (*av)++;
            if (left == NULL);
                left = sub_n;

        } else if(**av >= '0' && **av <= '9'){
            node n = {.type = VAL, .val = **av -'0'};
            (**av)++;
            if (left == NULL)
                left = new_node(n);

        } else if( **av == '*'){
            (**av)++;
            node *r = pars_expre(av); 
            node n = {.type = MULTI, .r = r, .l = left};
            left = new_node(n);

        } else if (**av == '+'){
            (**av)++;
            node *r = pars_expre(av);
            node n = {.type = ADD, .r = r, .l = left};
            left = new_node(n);
        } else 
            break ;
    }
    return (left);
}

node *parse_expr(char **av){
    node *left = NULL;
    while(**av){
        if (**av == '('){
            (**av)++;
            node *sub_n = parse_expr(av);
            if (**av == ')')
                return (NULL);
            (**av)++;
            if(left == NULL)
                left = sub_n;

        } else if (**av >= '0' && **av <= '9'){
            node n = {.type = VAL, .val = **av - '0'};
            (**av++);
            if (left == NULL)
                left = new_node(n);
        } else if (**av == '*'){
            (**av)++;
            node *r = parse_expr(av);
            node n = {.type = MULTI, .l = left, .r = r};
            left = new_node(n);

        } else if (**av == '+'){
            (**av)++;
            node *r = parse_expr(av);
            node n = {.type = ADD, .l = left, .r = r};
            left = new_node(n);

        } else 
            break ;
    }
    return (left);
}

node *parse_expr(char **av)
{
    node *left = NULL;
    while(**av)
    {
        if(**av == '('){
            (*av)++;
            node *sub_n = parse_expr(av);
            if (**av == ')')
                return (NULL);
            (*av)++;
            if(left == NULL)
                left = sub_n;
        } else if (**av >= '0' && **av <= '9'){
            node n = { .type = VAL, .val = **av -'0'};
            (*av)++;
            if (left == NULL)
                left = new_node(n);

        }else if (**av == '*'){
            (*av)++;
            node *r = parse_expr(av);
            node n = {.type = MULTI, .l = left, .r = r};
            left = new_node(n);

        } else if (**av == '+'){
            (*av)++;
            node *r = parse_expr(av);
            node n = {.type = ADD, .l = left, .r = r};
            left = new_node(n);

        } else 
            break ;
    }
    return (left);
}

int	eval_tree(node *tree)
{
    if (tree == NULL)
        exit (1);
	switch(tree->type)
	{
		case ADD:
			return (eval_tree(tree->l) + eval_tree(tree->r));
		case MULTI:
			return (eval_tree(tree->l) * eval_tree(tree->r));
		case VAL:
			return (tree->val);
	}
    return (0);
}

int	main(int argc, char *argv[])
{
	if (argc != 2)
		return (1);
	node *tree = parse_expr(argv[1]);
	if (!tree)
		return (1);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
    return (0);
}