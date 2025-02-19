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

int 	parse_int(json *dst, FILE *stream){
    int res = 0;
    if (fscanf(stream, "%d", &res) == 0) return -1;
    dst->type = INTEGER;
    dst->integer = res;
    return 1;
}

char 	*parse_str(FILE *stream){
    char *res = calloc(4096, sizeof(char));
    int i = 0;
    char c = getc(stream);

    while(1){
        c = getc(stream);
        if (c == '"')
            break ;
        if (c == EOF){
            unexpected(stream);
            return NULL;
        }
        if (c == '\\')
            c = getc(stream);
        res[i++] = c;
    }
    return res;
}

int parse_map(json *dst, FILE *stream){
    dst->type = MAP;
    dst->map.data = NULL;
    dst->map.size = 0;
    char c = getc(stream);

    if (peek(stream) == '}')
        return 1;
    while(1){
        c = peek(stream);
        if (c != '"')
            return -1;
        dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
        pair *current = &dst->map.data[dst->map.size];
        current->key = parse_str(stream);
        if (current->key == NULL) return -1;
        dst->map.size++;
        if(expect(stream, ':') == 0) return -1;
        if(argo(&current->value, stream) == -1) return -1;
        c = peek(stream);
        if (c == '}'){
            accept(stream, '}');
            break ;
        }
        if (c == ','){
            accept(stream, ',');
        } else {
            unexpected(stream);
            return -1;
        }
    }
    return 1;
}

int 	parser(json *dst, FILE *stream){
    char c = peek(stream);
    if (c == EOF){
        unexpected(stream);
        return -1;
    }
    if(isdigit(c)){
        return parse_int(dst, stream);
    } else if(c == '"'){
        dst->type = STRING;
        dst->string = parse_str(stream);
        if (dst->string == NULL) return -1;
        return 1;
    } else if (c == '{'){
        return parse_map(dst, stream);
    } else {
        unexpected(stream);
        return -1;
    }
    return -1;
}

int		argo(json *dst, FILE *stream){
    return parser(dst, stream);
}

// // Test functions -> run without the main.c for testing.
// // run_test("123abc"); Will succeed even though it is an error. 
// // you should NOT take care of this case in the exam anyway.
// void run_test(const char *input) {
//     FILE *stream = fmemopen((void *)input, strlen(input), "r");
//     if (!stream) {
//         perror("fmemopen");
//         exit(1);
//     }

//     json result;
//     printf("Testing: %s\n", input);
//     if (argo(&result, stream) == 1) {
//         printf("Parsed successfully!\n");
//     } else {
//         printf("Parsing failed!\n");
//     }
//     fclose(stream);
// }

// void run_tests() {
//     // Valid cases
//     run_test("1");
//     run_test("\"hello\"");
//     run_test("\"escape!\\\"\"");
//     run_test("{\"key\":123}");
//     run_test("{\"nested\":{\"inner\":42}}");
//     run_test("{\"multiple\":1,\"values\":\"test\"}");
//     run_test("{\"empty\":{}}");
//     run_test("\"string with spaces\"");
//     run_test("{\"greeting\":\"hello world\"}");

//     // Invalid cases
//     run_test("");
//     run_test("123abc");
//     run_test("\"unterminated");
//     run_test("{\"key\":}");
//     run_test("{\"missing\":42,}");
//     run_test("{\"wrong\":{\"inner\":}}");
//     run_test("{ key:\"value\"}"); // Invalid, missing quotes around key
//     run_test("{\"space invalid\": 42}"); // Invalid space before colon
// }

// int main() {
//     run_tests();
//     return 0;
// }
