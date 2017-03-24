/*
	CIS 2750
	A3
	Author: Kushal Pandya
	Due Date: March 10 2017

	Configuration File Convertor
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int getIndexOfChar(char * string, char c);
int getInfo(FILE *infile, char tag);
char *getAttribute(FILE *infile, char *tag);
int removeCharFromString(char * string, char c);


char **getArgs(char **args, char *argString) {

	int i;
	char *newStr;
	char *token;

	strtok(argString, "(");
	newStr = strtok(NULL, "(");
	removeCharFromString(newStr, ')');

	if (strchr(newStr, ',') == NULL) {
		strcpy(args[0], newStr);
		return args;
	}

	i = 0;
	token = strtok(newStr, ",");
	while(token != NULL) {

		strcpy(args[i], token);
		token = strtok(NULL, ",");
		i++;
	}
	
	return args;
}


void tConvert(char **args) {

	int i;
	char *attribute;
	char *value;
	char buffer[500];

	for (i=0; i<10; i++) {
		attribute = strtok(args[i], "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			if (strcmp(attribute, "text") == 0)
				printf("%s", value);
			else if (strcmp(attribute, "file") == 0) {

				value[strcspn(value, "\r\n")] = 0;
				FILE *fp = fopen(value, "r");
				if (fp == NULL) {
					printf("ERROR FILE NOT FOUND '%s'\n", value);
					exit(0);
				}

				while (fgets(buffer, 500, fp) != NULL) {
					printf("%s", buffer);
				}
			}	
		}
	}
}

void hConvert(char **args) {

	int i;
	char *attribute;
	char *value;
	int size = 3;
	char *text = calloc(100, sizeof(char));

	for (i=0; i<10; i++) {
		attribute = strtok(args[i], "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if (strcmp(attribute, "text") == 0)
				strcpy(text, value);

			else if (strcmp(attribute, "size") == 0) {
				size = atoi(value);
			}	
		}
	}
	printf("<h%d>%s</h%d>\n", size, text, size);
	free(text);
}

void pConvert(char **args) {

	int i;
	int height = 100;
	int width = 100;

	char *token;
	char *attribute;
	char *value;
	char *imageName = calloc(100, sizeof(char));

	for (i=0; i<10; i++) {
		attribute = strtok(args[i], "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if (strcmp(attribute, "image") == 0)
				strcpy(imageName, value);

			else if (strcmp(attribute, "size") == 0) {
				token = strtok(value, "x");
				width = atoi(token);
				token = strtok(NULL, "x");
				height = atoi(token);
			}	
		}
	}
	printf("<img src=\"%s\" style=\"width:%dpx;height:%dpx;\">\n", imageName, width, height);
	free(imageName);
}

void lConvert(char **args) {

	int i;
	char *attribute;
	char *value;
	char *text = calloc(100, sizeof(char));
	char *url = calloc(500, sizeof(char));

	for (i=0; i<10; i++) {
		attribute = strtok(args[i], "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if (strcmp(attribute, "text") == 0)
				strcpy(text, value);

			else if (strcmp(attribute, "link") == 0) {
				strcpy(url, value);
			}	
		}
	}
	printf("<a href=\"%s\">%s</a>\n", url, text);
	
	free(text);
	free(url);
}

void bConvert(char **args) {

	int i;
	int check = 0;
	char *attribute;
	char *value;
	char *text = calloc(100, sizeof(char));
	char *link = calloc(500, sizeof(char));

	for (i=0; i<10; i++) {
		attribute = strtok(args[i], "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if ((strcmp(attribute, "name") == 0) && check == 0) {
				strcpy(text, value);
				check = 1;
			}
			else if (strcmp(attribute, "link") == 0) {
				strcpy(link, value);
			}	
		}
	}
	printf("<form method=\"POST\" action=\"%s\">\n", link);
	printf("\t<input type=\"submit\" value=\"%s\" name=\"%s\"/>\n", text, text);
	printf("</form>\n");
	
	free(text);
	free(link);
}

void iConvert(char **args) {

	int i;
	char *attribute;
	char *value;

	char *temp = calloc(100, sizeof(char));
	char *text = calloc(100, sizeof(char));
	char *name = calloc(100, sizeof(char));
	char *inputValue = calloc(100, sizeof(char));
	
	for (i=0; i<10; i++) {
		strcpy(temp, args[i]);
		attribute = strtok(temp, "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if (strcmp(attribute, "action") == 0) 
				printf("<form method=\"POST\" action=\"%s\" id=\"usrform\">\n", value);
		}
	}

	for (i=0; i<10; i++) {
		strcpy(temp, args[i]);
		attribute = strtok(temp, "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if (strcmp(attribute, "value") == 0) 
				strcpy(inputValue, value);
			else if (strcmp(attribute, "text") == 0) 
				strcpy(text, value);
			else if (strcmp(attribute, "name") == 0) 
				strcpy(name, value);

			if (strcmp(name, "") != 0 && strcmp(text, "") != 0 && strcmp(inputValue, "") != 0) {
				printf("\t%s<input type=\"text\" name=\"%s\" value=\"%s\"/>\n", text, name, inputValue);	
				strcpy(name, "");
				strcpy(text, "");
				strcpy(inputValue, "");
			}
		}
		else {

			if (strcmp(temp, "value") == 0) 
				strcpy(inputValue, "");
			else if (strcmp(temp, "text") == 0) 
				strcpy(text, "");
			else if (strcmp(temp, "name") == 0) 
				strcpy(name, "");

			if (strcmp(name, "") != 0 && strcmp(text, "") != 0 && strcmp(inputValue, "") == 0) {
				printf("\t%s<input type=\"text\" name=\"%s\" value=\"%s\"/>\n", text, name, inputValue);	
				strcpy(name, "");
				strcpy(text, "");
				strcpy(inputValue, "");
			}
		}
	}
	printf("\t<input type=\"submit\" value=\"Submit\"/>\n");
	printf("</form>\n");
	
	free(text);
	free(name);
	free(inputValue);
	free(temp);
}

void rConvert(char **args) {

	int i;
	char *attribute;
	char *value;
	char *temp = calloc(100, sizeof(char));
	char *name = calloc(100, sizeof(char));

	for (i=0; i<10; i++) {
		strcpy(temp, args[i]);
		attribute = strtok(temp, "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if (strcmp(attribute, "action") == 0) 
				printf("<form method=\"POST\" action=\"%s\">\n", value);

			else if (strcmp(attribute, "name") == 0)
				strcpy(name, value);
		}
	}

	for (i=0; i<10; i++) {
		strcpy(temp, args[i]);
		attribute = strtok(temp, "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if (strcmp(attribute, "value") == 0) 
				printf("\t<input type=\"radio\" name=\"%s\" value=\"%s\"/> %s\n", name, value, value);	
		}
	}
	printf("\t<input type=\"submit\" value=\"Submit\"/>\n");
	printf("</form>\n");
	
	free(temp);
	free(name);
}

void eConvert(char **args) {

	int i;
	char *attribute;
	char *value;
	char *file = calloc(100, sizeof(char));

	for (i=0; i<10; i++) {
		attribute = strtok(args[i], "=");
		value = strtok(NULL, "=");

		if (value != NULL) {
			value[strcspn(value, "\r\n")] = 0;

			if (strcmp(attribute, "exe") == 0)
				strcpy(file, value);
		}
	}
	printf("<?php\n");
	printf("exec(\"%s\", $output, $status);\n", file);
	printf("if ($status)\n");
	printf("\techo 'exec failed';\n");
	printf("else {\n");
	printf("\tforeach ($output as $line)\n");
	printf("\t\techo $line;\n");
	printf("}\n?>\n");
	
	free(file);
}

void parseFile() {
	
	int i;
	char buffer[500];
	char tag;

	FILE *infile = fopen("format.txt", "r");

	/*Creating args array*/
	char **args = malloc(sizeof(char*)*10);
	for (i=0; i<10; i++)
		args[i] = calloc(100, sizeof(char));

	while (fgets(buffer, 500, infile) != NULL) {

		/*If buffer[1] is not ( then we know its a unspecified tag*/
		if (strcmp(buffer, "\n") != 0 && buffer[1] == '(') {
			tag = buffer[0];

			args = getArgs(args, buffer);

			switch(tag) {
				case 'd':
					printf("<hr>\n");
					break;
				case 't':
					tConvert(args);
					break;
				case 'h':
					hConvert(args);
					break;
				case 'l':
					lConvert(args);
					break;
				case 'p':
					pConvert(args);
					break;
				case 'b':
					bConvert(args);
					break;
				case 'r':
					rConvert(args);
					break;
				case 'i':
					iConvert(args);
					break;
				case 'e':
					eConvert(args);
					break;
				default:
					break;
			}
		}
		else
			printf("%s\n", buffer);
	}

	for (i=0; i<10; i++) {
		free(args[i]);
	}
	free(args);

	fclose(infile);
}


void formatFile(FILE *infile) {
	char c;
	int inBrackets = 0;
	int inString = 0;
	FILE *tempfile = fopen("format.txt", "w");

	while ((c = fgetc(infile)) != EOF) {

		switch(c) {
			case '.':
				if (inBrackets == 0 && inString == 0)
					fprintf(tempfile, "\n");
				if (inString == 1 && inBrackets == 1)
					fprintf(tempfile, ".");					
				break;
			case '(': 
				if (inString == 0)
					inBrackets = 1;
				fprintf(tempfile, "%c", c);
				break;
			case '"':
				if (inString == 1 && inBrackets == 1)
					inString = 0;
				else if (inBrackets == 1)
					inString = 1;

				if (inBrackets == 0)
					fprintf(tempfile, "%c", c);
				break;
			case ')':
				if (inString == 0)
					inBrackets = 0;
				fprintf(tempfile, "%c", c);
				break;
			default:
				fprintf(tempfile, "%c", c);
				break;
		}
	}
	fclose(tempfile);
}

int main(int argc, char *argv[]) {

	if (argc != 2)
		exit(1);

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("FILE NOT FOUND: %s\n", argv[1]);
		exit(1);
	}

	printf("<!DOCTYPE html>\n<html>\n");
	formatFile(fp);
	parseFile();

	printf("</html>\n");
	fclose(fp);

	return 0;
}

int getIndexOfChar(char * string, char c) {
	const char *ptr = strchr(string, c);
	int index;

	if (ptr) 
		return index = ptr - string;
	return -1;
}

int removeCharFromString(char * string, char c) {
	int result = getIndexOfChar(string, c);

	if (result >= 0) {
		memmove(&string[result], &string[result + 1], strlen(string) - result);
		return 1;
	}
	return 0;
}