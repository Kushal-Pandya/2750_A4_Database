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
int printPost(char *stream, char *user);
void markAllRead(char *stream, char *user);
void previousPost(char *stream, char *user, int postRead);
int nextPost(char *stream, char *user, int postRead);
void sortByName(char *stream);


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
	
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host. %s", mysql_error(&mysql));
	}			

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

	mysql_close(&mysql);
	mysql_library_end();

	free(temp);
}

int printPost(char *stream, char *user) {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	int i;
	int id;
	int maxid;
	char query[MAX_QUERY];
	
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host. %s", mysql_error(&mysql));
	}	

	sprintf(query, "select id from %sStream order by id desc", stream);
	if (mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}
	row = mysql_fetch_row(res);
	maxid = atoi(row[0]);

	id = 0;
	mysql_free_result(res);

	clrstr(query);
	sprintf(query, "select postRead from %sStreamUsers where authorName='%s'", stream, user);
	if (mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}

	while ((row = mysql_fetch_row(res))) {
		id = atoi(row[0]);
		if (id == 0)
			id = 1;
	}
	mysql_free_result(res);

	if (id > maxid)
		id = maxid;


	clrstr(query);
	sprintf(query, "select authorName,date,text from %sStream where id=%d", stream, id);
	if(mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}

	while ((row = mysql_fetch_row(res))) {
		for (i=0; i < mysql_num_fields(res); i++){
			printf("%s\n", row[i]);
		}
		printf("\n");
	}
	printf("\n");
	mysql_free_result(res);


	clrstr(query);
	sprintf(query, "update %sStreamUsers set postRead=%d where authorName='%s'", stream, id+1, user);
	if (mysql_query(&mysql, query)) {
		error("select failed!",&mysql);
	}

	mysql_close(&mysql);
	mysql_library_end();

	return id+1;
}

void markAllRead(char *stream, char *user) {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	int maxid;
	char query[MAX_QUERY];
	
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host. %s", mysql_error(&mysql));
	}			

	sprintf(query, "select id from %sStream order by id desc", stream);
	if (mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}
	row = mysql_fetch_row(res);
	maxid = atoi(row[0]);

	mysql_free_result(res);

	clrstr(query);
	sprintf(query, "update %sStreamUsers set postRead=%d where authorName='%s'", stream, maxid, user);
	if (mysql_query(&mysql, query)) {
		error("select failed!",&mysql);
	}

	mysql_close(&mysql);
	mysql_library_end();
}


void previousPost(char *stream, char *user, int postRead) {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	int i;
	char query[MAX_QUERY];
	
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host. %s", mysql_error(&mysql));
	}			

	sprintf(query, "select authorName,date,text from %sStream where id=%d", stream, postRead);
	if(mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}

	while ((row = mysql_fetch_row(res))) {
		for (i=0; i < mysql_num_fields(res); i++){
			printf("%s\n", row[i]);
		}
		printf("\n");
	}
	printf("\n");

	mysql_free_result(res);
	mysql_close(&mysql);
	mysql_library_end();
}


int nextPost(char *stream, char *user, int postRead) {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	int i, maxid, oldPostRead;
	char query[MAX_QUERY];
	
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host. %s", mysql_error(&mysql));
	}			

	sprintf(query, "select id from %sStream order by id desc", stream);
	if (mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}
	row = mysql_fetch_row(res);
	maxid = atoi(row[0]);

	mysql_free_result(res);


	if (postRead > maxid)
		postRead--;

	clrstr(query);
	sprintf(query, "select authorName,date,text from %sStream where id=%d", stream, postRead);
	if(mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}

	while ((row = mysql_fetch_row(res))) {
		for (i=0; i < mysql_num_fields(res); i++){
			printf("%s\n", row[i]);
		}
		printf("\n");
	}
	printf("\n");
	mysql_free_result(res);


	clrstr(query);
	sprintf(query, "select postRead from %sStreamUsers where authorName='%s'", stream, user);
	if (mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}
	while ((row = mysql_fetch_row(res))) {
		oldPostRead = atoi(row[0]);
		if (oldPostRead == 0)
			oldPostRead = 1;
	}
	mysql_free_result(res);


	postRead++;
	if (postRead > oldPostRead) {
		clrstr(query);
		sprintf(query, "update %sStreamUsers set postRead=%d where authorName='%s'", stream, postRead, user);
		if (mysql_query(&mysql, query)) {
			error("select failed!",&mysql);
		}
	}

	mysql_close(&mysql);
	mysql_library_end();

	return postRead;
}


void sortByName(char *stream) {
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	int i;
	char query[MAX_QUERY];
	
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host. %s", mysql_error(&mysql));
	}			

	sprintf(query, "select authorName,date,text from %sStream order by authorName", stream);
	if(mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}
	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}

	while ((row = mysql_fetch_row(res))) {
		for (i=0; i < mysql_num_fields(res); i++){
			printf("%s\n", row[i]);
		}
		printf("\n");
	}
	printf("\n");

	mysql_free_result(res);
	mysql_close(&mysql);
	mysql_library_end();

}