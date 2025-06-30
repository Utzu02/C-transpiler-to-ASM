#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Starting point of the program, write anything here
 */
int count_words(char **words)
{
	int i = 0;
	while (words[i])
		i++;
	return i;
}

char **get_words(char *exp)
{
	int size = 0;
	char **words = (char **)malloc(1 * sizeof(char *));
	char *temp = strtok(exp, "() \t;\n");
	while (temp)
	{
		words[size] = (char *)malloc((strlen(temp) + 1) * sizeof(char));
		strcpy(words[size], temp);
		size++;
		words = (char **)realloc(words, (size + 1) * sizeof(char *));
		temp = strtok(NULL, "() ;\t\n");
	}
	words[size] = NULL;
	free(temp);
	return words;
}

char *reg_shortcut(char *word)
{
	if (word[0] == 'a')
		return "eax";
	else if (word[0] == 'b')
		return "ebx";
	else if (word[0] == 'c')
		return "ecx";
	else if (word[0] == 'd')
		return "edx";
	else
		return word;
}

void eval_for(char **words)
{
	printf("MOV %s, %s\n", reg_shortcut(words[1]), reg_shortcut(words[3]));
	printf("start_loop:\n");
	printf("CMP %s, %s\n", reg_shortcut(words[1]), reg_shortcut(words[6]));
	if (strcmp(words[5], ">=") == 0)
		printf("JL end_label\n");
	else if (strcmp(words[5], "==") == 0)
		printf("JNE end_label\n");
	else if (strcmp(words[5], "<=") == 0)
		printf("JG end_label\n");
	else if (strcmp(words[5], "<") == 0)
		printf("JGE end_label\n");
	else if (strcmp(words[5], ">") == 0)
		printf("JLE end_label\n");
}

void eval_if(char **words)
{
	printf("CMP %s, %s\n", reg_shortcut(words[1]), reg_shortcut(words[3]));
	if (strcmp(words[2], ">=") == 0)
		printf("JL end_label\n");
	else if (strcmp(words[2], "==") == 0)
		printf("JNE end_label\n");
	else if (strcmp(words[2], "<=") == 0)
		printf("JG end_label\n");
	else if (strcmp(words[2], "<") == 0)
		printf("JGE end_label\n");
	else if (strcmp(words[2], ">") == 0)
		printf("JLE end_label\n");
}

void eval_while(char **words)
{
	printf("start_loop:\n");
	printf("CMP %s, %s\n", reg_shortcut(words[1]), reg_shortcut(words[3]));
	if (strcmp(words[2], ">=") == 0)
		printf("JL end_label\n");
	else if (strcmp(words[2], "==") == 0)
		printf("JNE end_label\n");
	else if (strcmp(words[2], "<=") == 0)
		printf("JG end_label\n");
	else if (strcmp(words[2], "<") == 0)
		printf("JGE end_label\n");
	else if (strcmp(words[2], ">") == 0)
		printf("JLE end_label\n");
}

void eval_simple_exp(char **words)
{
	int size = count_words(words);
	if (size == 3)
	{
		printf("MOV %s, %s\n", reg_shortcut(words[0]), reg_shortcut(words[2]));
		return;
	} else if (size > 3)
	{
		switch (words[3][0])
		{
		case '+':
			printf("ADD");
			break;
		case '-':
			printf("SUB");
			break;
		case '&':
			printf("AND");
			break;
		case '|':
			printf("OR");
			break;
		case '^':
			printf("XOR");
			break;
		case '<':
			printf("SHL");
			break;
		case '>':
			printf("SHR");
			break;
		default:
			break;
		}
		if (strchr("+-&|^><", words[3][0])) {
			printf(" %s, %s\n", reg_shortcut(words[2]), reg_shortcut(words[4]));
		} else {
			if (words[4][0] == '0' && words[3][0] == '/')
			{
				printf("Error\n");
				return;
			}
			if (strchr("0123456789", words[4][0]) && words[0][0] == 'a') {
				if (words[3][0] == '*') {
					printf("MUL");
					printf(" %s\n", reg_shortcut(words[4]));
				} else {
					printf("MOV eax, a\n");
					printf("DIV %s\n", reg_shortcut(words[4]));
					printf("MOV a, eax\n");
				}
			} else {
				if (words[3][0] == '*') {
					printf("MOV eax, %s\n", reg_shortcut(words[2]));
					printf("MUL %s\n", reg_shortcut(words[4]));
					printf("MOV %s, eax\n", reg_shortcut(words[2]));
				} else {
					printf("MOV eax, %s\n", reg_shortcut(words[2]));
					printf("DIV %s\n", reg_shortcut(words[4]));
					printf("MOV %s, eax\n", reg_shortcut(words[2]));
				}
			}
		}
	}
}

void check_exp(char **words, int *is_for, int *is_if, int *is_while, char *cnt)
{
	if (words[0][0] == '}') {
		if (*is_for) {
			*is_for = 0;
			char temp[2];
			temp[0] = *cnt;
			temp[1] = '\0';
			printf("ADD %s, 1\n", reg_shortcut(temp));
			printf("JMP start_loop\n");
			printf("end_loop:\n");
		} else if (*is_while) {
			*is_while = 0;
			printf("JMP start_loop\n");
			printf("end_label:\n");
		} else {
			*is_if = 0;
			printf("end_label:\n");
		}
	} else if (strcmp(words[0], "if") == 0) {
		*is_if = 1;
		eval_if(words);
	} else if (strcmp(words[0], "for") == 0) {
		*cnt = words[1][0];
		*is_for = 1;
		eval_for(words);
	} else if (strcmp(words[0], "while") == 0) {
		*is_while = 1;
		eval_while(words);
	} else {
		eval_simple_exp(words);
	}
}

void transpile(char *exp, int *is_for, int *is_if, int *is_while, char *cnt)
{
	char **words = get_words(exp);
	check_exp(words, is_for, is_if, is_while, cnt);
	int size = count_words(words);
	int i = 0;
	for (i = 0; i < size; ++i)
		free(words[i]);
	free(words);
}

int main(void)
{
	int is_for = 0, is_if = 0, is_while = 0;
	char cnt = 'a';
	char *exp = (char *)malloc(100 * sizeof(char));
	while (fgets(exp, 100, stdin))
	{
		if (strchr(exp, '\n'))
			exp[strlen(exp)] = '\0';
		transpile(exp, &is_for, &is_if, &is_while, &cnt);
	}
	free(exp);
	return 0;
}
