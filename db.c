
/*
	CIS 2750 A4
	Author: Kushal Pandya 0888884
	Due Date: Friday March 24 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "pandyak"
#define PASSWORD  "0888884"
#define DATABASE  "pandyak"


void error(char *msg, MYSQL *mysql) {
	printf("%s\n%s\n",msg,mysql_error(mysql));
}

void clrstr(char *buf) {
	buf[0] = '\0';
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


int main(int argc, char *argv[]) {
	
	int i;
	int check = 0;

	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char query[MAX_QUERY];
	char *temp = malloc(sizeof(char)*100);
		
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host.%s", mysql_error(&mysql));
	}	
	printf("Connected to database\n");

	if (argc == 1)
		printf("No Flag detected, -help for usage\n");

	for (i=1; i<argc; i++) {
		removeCharFromString(argv[i], '-');
		
		if (strcmp(argv[i], "help") == 0) {
			printf("Flag Usage e.g. ./db -clear\n\n"
			"-clear --- the program removes all of the posts, users, streams and any other information from the tables in the database\n"
		 	"-reset --- the program deletes the tables from the database.\n"
		 	"-posts --- the program prints out all posts stored in the database.\n"
		 	"-users --- the program prints out all user names stored in the database.\n"
		 	"-streams --- the program prints out all stream names stored in the database.\n"
		 	"-help --- the program displays this help message.\n\n"
		 	"Order of the output does not matter for any of the options which print the contents of a table.\n\n");
		}
		else if (strcmp(argv[i], "clear") == 0) {
			clrstr(query);
			strcpy(query, "drop database pandyak");
			if (mysql_query(&mysql, query)) {
			  error("Could not drop database!",&mysql);
			}
			
			clrstr(query);
			strcpy(query, "create database pandyak");
			if (mysql_query(&mysql, query)) {
			  error("Could not create database!",&mysql);
			}
			clrstr(query);

			printf("Cleared all information\n\n");
		}
		else if (strcmp(argv[i], "reset") == 0) {
			clrstr(query);
			strcpy(query, "drop database pandyak");
			if (mysql_query(&mysql, query)) {
			  error("Could not drop database!",&mysql);
			}
			
			clrstr(query);
			strcpy(query, "create database pandyak");
			if (mysql_query(&mysql, query)) {
			  error("Could not create database!",&mysql);
			}
			clrstr(query);

			printf("Deleted all tables\n\n");
		}
		else if (strcmp(argv[i], "posts") == 0) {
			clrstr(query);
			strcpy(query, "show tables like '%%Stream'");

			if(mysql_query(&mysql, query)){
				error("select failed!",&mysql);
			}
			if (!(res = mysql_store_result(&mysql))){
				error("Couldnt find any posts!",&mysql);
			}
			

			check = 1;
			clrstr(query);
			strcpy(query, "");
			while ((row = mysql_fetch_row(res))) {

				for (i=0; i < mysql_num_fields(res); i++){
					sprintf(temp, "select authorName,date,text from %s", row[i]);
					strcat(query, temp);
				}

				if (check != (int) mysql_num_rows(res)) {
					strcat(query, " union ");
					check++;
				}
			}
			strcat(query, " order by date");

			if(mysql_query(&mysql, query)){
				error("select failed!",&mysql);
			}
			if (!(res = mysql_store_result(&mysql))){
				error("store failed!",&mysql);
			}

			printf("Posts:\n");
			while ((row = mysql_fetch_row(res))) {
				for (i=0; i < mysql_num_fields(res); i++){
					printf("%s\n", row[i]);
				}
				printf("\n");
			}
			printf("\n");
		}
		else if (strcmp(argv[i], "users") == 0) {
			clrstr(query);
			strcpy(query, "show tables like '%%StreamUsers'");

			if(mysql_query(&mysql, query)){
				error("select failed!",&mysql);
			}
			if (!(res = mysql_store_result(&mysql))){
				error("Couldnt find any users!",&mysql);
			}
			

			check = 1;
			clrstr(query);
			strcpy(query, "");
			while ((row = mysql_fetch_row(res))) {

				for (i=0; i < mysql_num_fields(res); i++){
					sprintf(temp, "select authorName from %s", row[i]);
					strcat(query, temp);
				}

				if (check != (int) mysql_num_rows(res)) {
					strcat(query, " union ");
					check++;
				}
			}
			strcat(query, " order by authorName");

			if(mysql_query(&mysql, query)){
				error("select failed!",&mysql);
			}
			if (!(res = mysql_store_result(&mysql))){
				error("store failed!",&mysql);
			}

			printf("Users:\n");
			while ((row = mysql_fetch_row(res))) {
				for (i=0; i < mysql_num_fields(res); i++){
					printf("%s\n", row[i]);
				}
			}
			printf("\n");
		}
		else if (strcmp(argv[i], "streams") == 0) {
			clrstr(query);
			strcpy(query, "show tables like '%%Stream'");

			if(mysql_query(&mysql, query)){
				error("select failed!",&mysql);
			}
			if (!(res = mysql_store_result(&mysql))){
				error("Couldnt find any posts!",&mysql);
			}
			
			printf("Streams:\n");
			while ((row = mysql_fetch_row(res))) {
				for (i=0; i < mysql_num_fields(res); i++){
					printf("%s\n", row[i]);
				}
			}
			printf("\n");
		}
		else {
			printf("Unrecognized Flag, -help for usage\n");
			exit(1);
		}
	}

	free(temp);
	return 0;
}