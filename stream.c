
#include "stream.h"
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

void performAdd(char *token, char *name) {

	char *temp = malloc(sizeof(char)*100);
	char *filename = malloc(sizeof(char)*100);		

	MYSQL mysql;
	char query[MAX_QUERY];
		
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host.%s", mysql_error(&mysql));
	}			

	strcpy(temp, token);
	removeCharFromString(temp, '\n');
	strcpy(filename, strcat(temp, "StreamUsers"));

	clrstr(query);
	clrstr(temp);
	sprintf(query, "create table %s (", filename);
	strcat(query, "authorName char(30),");
	strcat(query, "postRead int,");
	strcat(query, "primary key(authorName) )");
	
	if(mysql_query(&mysql, query)) {
	  error("Could not create table!",&mysql);
	}
	clrstr(query);

	char records[100];
	sprintf(records, "insert into %s (authorName,postRead) values ('%s','0')", filename, name);
	
	if(mysql_query(&mysql, records)) {
		printf("Failure to insert: %s\n",records);
		error("Could not insert record",&mysql);
	}

	free(temp);
	free(filename);
}

void performRemove(char *token, char *name) {

	char *temp = malloc(sizeof(char)*100);
	char *filename = malloc(sizeof(char)*100);		

	MYSQL mysql;
	char query[MAX_QUERY];
		
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host.%s", mysql_error(&mysql));
	}	

	strcpy(temp, token);
	removeCharFromString(temp, '\n');
	strcpy(filename, strcat(temp, "StreamUsers"));
	clrstr(query);

	char records[100];
	sprintf(records, "delete from %s where %s.authorName='%s'", filename, filename, name);
	
	if(mysql_query(&mysql, records)) {
		printf("Failure to remove: %s\n",records);
		error("Could not remove record",&mysql);
	}

	free(temp);
	free(filename);
}


void addUser(char *username, char *list) {

	char *streamName = calloc(100, sizeof(char));
	char *token;

	if (strchr(list, ',') != NULL) {
		token = strtok(list, ",");
		while(token != NULL) {
			strcpy(streamName, token);	
			performAdd(streamName, username);
			token = strtok(NULL, ",");
		}
	}
	else {
		performAdd(list, username);
	}
	free(streamName);
}

void removeUser(char *username, char *list) {
	
	char *streamName = malloc(sizeof(char)*100);
	char *token;

	if (strchr(list, ',') != NULL) {
		token = strtok(list, ",");
		while(token != NULL) {
			strcpy(streamName, token);			
			performRemove(streamName, username);
			token = strtok(NULL, ",");
		}
	}
	else {
		performRemove(list, username);
	}
	free(streamName);
}

void updateStream(struct userPost *st) {

	char *temp = malloc(sizeof(char)*100);
	char *streamFilename = malloc(sizeof(char)*100);
	char *streamUsersFilename = malloc(sizeof(char)*100);

	MYSQL mysql;
	char query[MAX_QUERY];
	MYSQL_RES *res;
	MYSQL_ROW row;
	int hasPermission = 0;
	int i = 0;

	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
		DATABASE, 0, NULL, 0)) {
	   printf("Could not connect to host.%s", mysql_error(&mysql));
	}			

	strcpy(streamFilename, st->streamname);
	removeCharFromString(streamFilename, '\n');
	strcat(streamFilename, "Stream");

	strcpy(streamUsersFilename, st->streamname);
	removeCharFromString(streamUsersFilename, '\n');
	strcat(streamUsersFilename, "StreamUsers");

	/*Checking to see if user has permission*/
	removeCharFromString(st->username, '\n');
	clrstr(query);
	sprintf(query, "select * from %s where %s.authorName='%s'", streamUsersFilename,
		streamUsersFilename, st->username);

	if(mysql_query(&mysql, query)){
		error("select failed!",&mysql);
	}

	if (!(res = mysql_store_result(&mysql))){
		error("store failed!",&mysql);
	}
	
	while ((row = mysql_fetch_row(res))) {
		for (i=0; i < mysql_num_fields(res); i++){
			if (row == NULL)
				hasPermission = 0;
			else
				hasPermission = 1;
		}
	}

	if (hasPermission) {
		/*Creating stream table name*/
		clrstr(query);
		clrstr(temp);
		sprintf(temp, "create table %s (id int not null auto_increment,", streamFilename);
		strcat(query, temp);
		strcat(query, "authorName char(30),");
		strcat(query, "date datetime NOT NULL,");
		strcat(query, "text varchar(255),");
		strcat(query, "primary key(id) )");

		if(mysql_query(&mysql, query)) {
		  error("Could not create table!",&mysql);
		}
		clrstr(query);
		clrstr(temp);

		char records[100];
		sprintf(records, "insert into %s (authorName,date,text) values ('%s',NOW(),'%s')", streamFilename, 
			st->username, st->text);
		
		if(mysql_query(&mysql, records)) {
			printf("Failure to insert: %s\n",records);
			error("Could not insert record",&mysql);
		}
	}

	free(temp);
	free(streamFilename);
	free(streamUsersFilename);
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
