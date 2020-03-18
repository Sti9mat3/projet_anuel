#include <string.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "mysql"
#define DB_NAME "testdb"
#define STRING_SIZE 50

int main(int argc, char **argv)
{ 
	MYSQL_STMT *stmt;  
	MYSQL *con = mysql_init(NULL);
	MYSQL_BIND bind[1];
	my_ulonglong  affected_rows;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
   
	char str_data[STRING_SIZE];
	unsigned long str_length;
	int param_count;
	const char* query;
	unsigned int num_fields;
	unsigned int i;

	if (mysql_library_init(0, NULL, NULL)) {
    		fprintf(stderr, "could not initialize MySQL client library\n");
    		exit(1);
	}

	if (con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, DB_HOST, DB_USER, DB_PASS, 
          DB_NAME, 0, NULL, 0) == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}  


	query = "SELECT * FROM testdb";   
	stmt = mysql_stmt_init(con);
	
	if(!stmt)
	{
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
  
	if(mysql_stmt_prepare(stmt, query, strlen(query)))
	{
		fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}


	fprintf(stdout, " prepare, SELECT successful\n");

	param_count = mysql_stmt_param_count(stmt);
	fprintf(stdout, " total parameters in SELECT: %d\n", param_count);

	if (param_count != 1) 
	{
		fprintf(stderr, " invalid parameter count returned by MySQL\n");
		exit(0);
	}
/*
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (char*)str_data;
	bind[0].buffer.length = STRING_SIZE;
	bind[0].is_null = 0;
	bind[0].length = &str_length; 

	if(mysql_stmt_bind_param(stmt, bind))
	{
		fprintf(stderr, " mysql_stmt_bind_param() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	
	strncpy(str_data, DB_NAME, STRING_SIZE);
	str_length = strlen(str_data);

	if (mysql_stmt_execute(stmt))
	{
  		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
  		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
  		exit(0);
	}*/

	result = mysql_list_tables(con, NULL);
	
	if(result)
	{
		num_fields = mysql_num_fields(result);
		printf("result is empty\n");
	}else{
		fprintf(stderr,"Error: %s\n", mysql_error(&stmt));
	}

	while ((row = mysql_fetch_row(result)))
	{
   		unsigned long *lengths;
   		lengths = mysql_fetch_lengths(result);
   		for(i = 0; i < num_fields; i++)
   		{
       			printf("%.*s ", (int) lengths[i],
              		row[i] ? row[i] : "NULL");
   		}
   		printf("\n");
	}
	
	printf("la requete a été executé\n");

  	mysql_stmt_close(stmt);
  	mysql_close(con);
	mysql_library_end();
  	return EXIT_SUCCESS;
}

