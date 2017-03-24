
/*
	CIS 2750 A4
	Author: Kushal Pandya 0888884
	Due Date: Friday March 24 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>


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


int main(int argc, char *argv[]) {
	
	int i;

	for (i=1; i<argc; i++) {
		removeCharFromString(argv[i], '-');
		
		if (strcmp(argv[i], "help") == 0) {
			printf("Flag Usage e.g. ./db -clear\n\n"
			" -clear --- the program removes all of the posts, users, streams and any other information from the tables in the database\n"
		 	"-reset --- the program deletes the tables from the database.\n"
		 	"-posts --- the program prints out all posts stored in the database.\n"
		 	"-users --- the program prints out all user names stored in the database.\n"
		 	"-streams --- the program prints out all stream names stored in the database.\n"
		 	"-help --- the program displays this help message.\n\n"
		 	"Order of the output does not matter for any of the options which print the contents of a table.\n");
		}
		else if (strcmp(argv[i], "clear") == 0) {
			printf("Flag entered: %s\n", argv[i]);
		}
		else if (strcmp(argv[i], "reset") == 0) {
			printf("Flag entered: %s\n", argv[i]);
		}
		else if (strcmp(argv[i], "posts") == 0) {
			printf("Flag entered: %s\n", argv[i]);
		}
		else if (strcmp(argv[i], "users") == 0) {
			printf("Flag entered: %s\n", argv[i]);
		}
		else if (strcmp(argv[i], "streams") == 0) {
			printf("Flag entered: %s\n", argv[i]);
		}
		else {
			printf("Unrecognized Flag, -help for usage\n");
			exit(1);
		}
	}

	return 0;
}