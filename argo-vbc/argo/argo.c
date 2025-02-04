#include "argo.h"

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

// int pars_int(json *dst, FILE *stream){
// 	int res;
// 	fscanf(stream, "%d", &res);
// 	dst->type = INTEGER;
// 	dst->integer = res;
// 	return (1);
// }
// char *pars_str(FILE *stream){
// 	char *res = calloc(4096, sizeof(char));
// 	int i = 0;
// 	char c = getc(stream);
// 	while (1){
// 		c = getc(stream);
// 		if (c == '"')
// 			break;
// 		if (c == EOF){
// 			unexpected(stream);
// 			return NULL;
// 		}
// 		if (c = '\\')
// 			getc(stream);
// 		res[i++] = c;
// 	}
// 	return res;
// }

// int pars_map(json *dst, FILE *stream){
// 	dst->type = MAP;
// 	dst->map.size = 0;
// 	dst->map.data = NULL;
// 	char c = getc(stream);

// 	if (peek(stream) == '}')
// 		return (-1);
// 	while(1){
// 		c = peek(stream);
// 		if (c != '"')
// 			return -1;
// 		dst->map.data = realloc(dst->map.data, (dst->map.size +1 * sizeof(pair)));
// 		pair *current = &dst->map.data[dst->map.size];
// 		current->key = pars_str(stream);
// 		if (current->key == NULL)
// 			return -1;
// 		dst->map.size++;
// 		if (expect(stream, ':') == 0)
// 			return -1;
// 		if (argo(&current->value, stream) == -1)
// 			return (-1);
// 		c = peek(stream);
// 		if (c = '}'){
// 			accept(stream, c);
// 			break;
// 		}
// 		if (c == ','){
// 			accept(stream, ',');
// 		} else {
// 			unexpected(stream);
// 			return -1;
// 		}
// ;	}
// 	return (1);
// }

// int parser(json *dst, FILE *stream){
// 	char c = peek(stream);
// 	if (c == EOF){
// 		unexpected(stream);
// 		return -1;
// 	}
// 	if (isdigit(c)){
// 		return (pars_int(dst, stream));
// 	} else if (c == '"'){
// 		dst->type = STRING;
// 		dst->string = pars_str(stream);
// 		if (dst->string == NULL)
// 			return -1;
// 		return 1;
// 	} else if (c == '{'){
// 		return (pars_map(dst, stream));
// 	} else {
// 		unexpected(stream);
// 		return -1;
// 	}
// 	return (1);
// }

// int	argo(json *dst, FILE *stream){
// 	if (parser(dst, stream) == -1)
// 		return (-1);
// 	return (1);
// }

int pars_int(json *dst, FILE *stream){
	int res = 0;
	fscanf(stream, "%d", &res);
	dst->type = INTEGER;
	dst->integer = res;
	return (1);
}

char pars_str(FILE *stream){
	char *res = calloc(4096, sizeof(char));
	int i = 0;
	char c = getc(stream);

	while (1){
		c = getc(stream);
		if (c = '"')
			break;
		if (c = EOF){
			unexpected(stream);
			return NULL;
		}
		if (c == '\\')
			c = getc(stream);
		res[i++] = c;
	}
	return (res);
}

int pars_map(*dst, FILE *stream){
	return (1);
}

int parser(json *dst, FILE *stream){
	return (1);
}
int	argo(json *dst, FILE *stream){
	if (parser(dst, stream) == -1)
		return -1;
	return 1;
}