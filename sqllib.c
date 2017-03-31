#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "pandyak"
#define PASSWORD  "0888884"
#define DATABASE  "pandyak"


void connect(char *user);


void error(char *msg, MYSQL *mysql) {
	printf("%s\n%s\n",msg,mysql_error(mysql));
}

void clrstr(char *buf) {
	buf[0] = '\0';
}

void removeSubstring(char *s, const char *toremove) {

	int len = strlen(toremove);
  	while ((s=strstr(s,toremove)))
    	memmove(s,s+len,1+strlen(s+len));
}

void connect(char *user) {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_RES *res2;
	MYSQL_ROW row2;

	int i;
	char query[MAX_QUERY];
	char *temp = malloc(sizeof(char)*100);

	printf("connecting...\n");
	
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host. %s", mysql_error(&mysql));
	}			
	printf("Connected\n");

	strcpy(query, "show tables like '%%StreamUsers'");
	if(mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}

	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}

	while ((row = mysql_fetch_row(res))) {
		clrstr(query);

		for (i=0; i < mysql_num_fields(res); i++){
			sprintf(temp, "select authorName from %s where authorName='%s'", row[i], user);
			strcat(query, temp);

			if (mysql_query(&mysql, query)) {
				error("select failed!",&mysql);
			}
			if (!(res2 = mysql_store_result(&mysql))) {
				error("store failed!",&mysql);
			}
			while ((row2 = mysql_fetch_row(res2))) {
				if (row2[0] != NULL) {
					clrstr(temp);
					strcpy(temp, row[i]);
					removeSubstring(temp, "StreamUsers");
					printf("%s\n", temp);
				}					
			}
			mysql_free_result(res2);
		}
	}

	mysql_free_result(res);
	mysql_free_result(res2);

	mysql_close(&mysql);
	mysql_library_end();

	free(temp);
}